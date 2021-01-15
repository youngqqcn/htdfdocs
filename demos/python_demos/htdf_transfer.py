#!coding:utf8
# author:yqq
# date:2019/5/6 0006 18:07   create demo
# date:2019/9/29 0029 11:03  python2 -> python3
# date: 2020/10/10   refactor


import json
import hashlib
import time
import ecdsa
import traceback
import coincurve
import base64
import requests
import logging
from typing import Tuple, Dict
from binascii import hexlify, unhexlify
from bech32 import bech32_decode
from func_timeout import func_set_timeout

#  configurations for testchain
g_chainid = 'testchain'
g_fromaddr = 'htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml'
g_privkey = '279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8'
g_node_ip_port = 'htdf2020-test01.orientwalt.cn:1317'
# g_node_ip_port = '192.168.0.171:1317'
g_gaslimit = 30000
g_gasprice = 100

# TODO: configurations for mainchain
# g_chainid = 'mainchain'
# g_strFrom = 'YOUR_HTDF_ADDRESS'
# g_PrivKey = 'YOUR_HTDF_ADDRESS_PRIVATE_KEY'
# g_node_ip_port = 'YOUR_HTDF_FULL_NODE_IP_ADDRESS:1317'
# g_gaslimit = 30000
# g_gasprice = 100


g_unsigned_rawtx = """{\
    "account_number": "%d",\
	"chain_id": "%s",\
	"fee": {\
			"gas_price": "%d",\
			"gas_wanted": "%d"\
	},\
    "memo": "%s",\
	"msgs": [{\
		"Amount": [{\
			"amount": "%d",\
            "denom": "satoshi"\
		}],\
        "Data": "",\
        "From": "%s",\
        "GasPrice": %s,\
        "GasWanted": %s,\
		"To": "%s"\
	}],\
    "sequence": "%d"\
    }"""

g_broadcast_str = """{
        "type": "auth/StdTx",
        "value":{
            "msg": [{
                "type": "htdfservice/send",  
                "value":{
                    "From": "%s",
                    "To": "%s",
                    "Amount": [{
                        "denom": "satoshi",
                        "amount": "%d"
                    }],
                    "Data": "",
                    "GasPrice": "%d",
                    "GasWanted": "%d"
                }
            }],
            "fee": {
                "gas_wanted": "%d",
                "gas_price": "%d"
            },
            "signatures": [{
                "pub_key": {
                    "type": "tendermint/PubKeySecp256k1",
                    "value": "%s"
                },
                "signature": "%s"
            }],
            "memo": "%s"
        }
    }"""


def sign(rawhash, key):
    if coincurve and hasattr(coincurve, 'PrivateKey'):
        pk = coincurve.PrivateKey(key)
        signature = pk.sign_recoverable(rawhash, hasher=None)
        # v = safe_ord(signature[64]) + 27
        r = signature[0:32]
        s = signature[32:64]
        return r, s

def privkey_to_pubkey(privkey: str) -> str:
    sk = ecdsa.SigningKey.from_string(unhexlify(privkey), curve=ecdsa.SECP256k1)
    s = sk.get_verifying_key().to_string(encoding='compressed')
    return hexlify(s).decode('latin')


def get_account_info(address: str) -> Tuple[int, int]:
    url = 'http://{0}/auth/accounts/{1}'.format(g_node_ip_port.strip(), address.strip())
    rsp = requests.get(url)

    if rsp.status_code != 200:
        if rsp.status_code == 204:
            raise Exception('not found any account info ')
        raise Exception('get account info error: {}'.format(rsp.status_code))

    rsp = rsp.json()
    account_number = int(rsp['value']['account_number'])
    sequence = int(rsp['value']['sequence'])
    return account_number, sequence


def is_valid_address(address: str) -> bool:
    if not (len(address) == 43 and address.islower()):
        return False

    prefix, data = bech32_decode(address)
    if prefix is None or data is None:
        return False

    if prefix == 'htdf' and len(data) == 32:
        return True

    return False


def htdf_to_satoshi(amount_htdf: float) -> int:
    return int(amount_htdf * 10 ** 8)


def broadcast_tx(json_data: dict) -> str:
    assert 'tx' in json_data, 'invalid json_data to broadcast'

    post_data = json.dumps(json_data)
    url = 'http://%s/hs/broadcast' % (g_node_ip_port)
    rsp = requests.post(url, post_data)

    if rsp.status_code != 200:
        raise Exception(rsp.text)

    rsp_json = rsp.json()
    if 'code' in rsp_json or 'raw_log' in rsp_json:
        raise Exception(rsp.text)

    txhash = str(rsp_json['txhash'])

    return txhash


def transfer(from_address: str, to_address: str, amount_satoshi: int, memo: str) -> str:
    # step 1 : arguments check
    if not is_valid_address(from_address):
        raise Exception("invalid from_address address : {}".format(from_address))

    if not is_valid_address(to_address):
        raise Exception("invalid to_address address : {}".format(to_address))

    if amount_satoshi < 1 or amount_satoshi > htdf_to_satoshi(6000_0000):
        raise Exception("amount_satoshi {} is invalid".format(amount_satoshi))

    # step 2 : get account details
    account_number, sequence = get_account_info(from_address)
    logging.info('account_number : {}, sequence: {} '.format(account_number, sequence))

    # step 3 : format raw transaction
    fmt_unsigned_txstr = g_unsigned_rawtx.replace(' ', '').replace('\t', '').replace('\n', '')
    fmt_unsigned_txstr = fmt_unsigned_txstr % (
        account_number, g_chainid, g_gasprice, g_gaslimit, memo,
        amount_satoshi, from_address, g_gasprice, g_gaslimit,
        to_address, sequence)

    logging.info("formatted raw transaction str: {}".format(fmt_unsigned_txstr))

    # step 4 : make signature
    shadata = hashlib.sha256(fmt_unsigned_txstr.encode('utf-8')).digest()
    logging.info("sha256(fmt_unsigned_txstr): {}".format(hexlify(shadata)))

    r, s = sign(shadata, unhexlify(g_privkey))
    logging.info('r:' + hexlify(r).decode(encoding='utf8'))
    logging.info('s:' + hexlify(s).decode(encoding='utf8'))

    b64sig = base64.b64encode(r + s).decode(encoding='utf8')
    logging.info("base64encode(signature) : {}".format(b64sig))

    # step 5 : format broadcast string
    pubkey = privkey_to_pubkey(privkey=g_privkey)
    b64pubkey = base64.b64encode(unhexlify(pubkey)).decode(encoding='utf8')
    logging.info("base64encode(public key) :" + b64pubkey)

    fmt_broadcast_str = g_broadcast_str.replace(' ', '').replace('\n', '').replace('\t', '')
    fmt_broadcast_str = fmt_broadcast_str % (
        from_address, to_address, amount_satoshi,
        g_gasprice, g_gaslimit, g_gaslimit, g_gasprice, b64pubkey, b64sig, memo)
    logging.info("broadcast str: {}".format(fmt_broadcast_str))

    broadcast_data = hexlify(bytes(fmt_broadcast_str, encoding='utf8')).decode(encoding='utf8')
    broadcast_data = {'tx': broadcast_data}
    print(broadcast_data)

    # step 6 : broadcast
    # tx_hash = broadcast_tx(broadcast_data)
    # logging.info("txhash : {}".format(tx_hash))
    # return tx_hash


@func_set_timeout(30)
def get_tranaction(tx_hash: str) -> Dict:
    while True:
        url = 'http://{0}/txs/{1}'.format(g_node_ip_port.strip(), tx_hash.strip())
        rsp = requests.get(url)
        if rsp.status_code == 404:
            time.sleep(3)
            continue
        tx = rsp.json()
        return tx
    pass


if __name__ == '__main__':

    logging.basicConfig(level=logging.INFO,
                        format='[%(asctime)s |  %(levelname)s | %(filename)s |%(funcName)s:%(lineno)d] %(message)s')

    try:
        from_address = g_fromaddr
        to_address = 'htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2'
        amount_satoshi = htdf_to_satoshi(0.12345678)
        memo = 'transaction demo in python'

        tx_hash = transfer(from_address, to_address, amount_satoshi, memo)

        # get transaction details
        tx = get_tranaction(tx_hash=tx_hash)
        logging.info(json.dumps(tx, indent=4))

    except Exception as e:
        traceback.print_exc()

    pass
