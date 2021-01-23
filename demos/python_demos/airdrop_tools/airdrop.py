#coding:utf8
#author: yqq
#date: 2021-01-20
#descriptions: HRC20批量转账示例, 请根据需要自行修改

import json
import time
import random
from pprint import pprint
from htdfsdk import to_checksum_address, remove_0x_prefix
from htdfsdk import Address, HtdfRPC, HtdfPrivateKey, HtdfTxBuilder, htdf_to_satoshi
from htdfsdk import HtdfContract

from .hrc20_tokens import HRC20_TOKENS

# 自行修改
COMMON = {
    'CHAINID': 'mainchain',
    'RPC_HOST': '192.168.0.62',
    'RPC_PORT': 1317,
    'ABI_FILE':'./hrc20.abi',
    'ADDRESS_FILE': 'address.txt',
}

# 自行修改
TOKEN_CFG = {
    'DFW': {
        'ADDRESS': '--------TODO:地址---------', # from地址
        'PRIVATE_KEY': '-------TODO:私钥--------', # from地址私钥
        'MEMO':'糖果',  # 转账备注
        'RANDOM_START': 100,  
        'RANDOM_END': 2000,  # 和 RANDOM_START配合, 生成 100~2000之间的随机数
        'ADDRESS_START_INDEX':1,
        'ADDRESS_END_INDEX': 3752, #和 ADDRESS_START_INDEX 截取地址文件的内容
    }
}


def get_random_amount(start, end):
    random.seed(a=time.time())
    n = random.randint(start, end)
    f = random.random()
    f = f - int(f)
    return n + f


def get_addresses(path, start_indx = 0, end_index = 0):
    ret = []
    with open(path) as infile:
        lines = infile.readlines()
        for line in lines:
            if '#' in line: continue
            splits = line.split('\t')
            for item in splits:
                if 'htdf1' in item:
                    ret.append(item.strip())

    if start_indx == 0 and end_index == 0:
        return ret
    return ret[start_indx : end_index]


def hrc20_batch_transfer( token_name: str, contract_addr: str,  addrs_list: list, cfg:dict ):
    """
    hrc20批量转账
    """

    with open('./hrc20.abi', 'r') as abifile:
        # abi = abifile.readlines()
        abijson = abifile.read()
        # print(abijson)
        abi = json.loads(abijson)

    contract_address = Address(contract_addr)
    htdfrpc = HtdfRPC(chaid_id=COMMON['CHAINID'], rpc_host=COMMON['RPC_HOST'], rpc_port=COMMON['RPC_PORT'])
    hc = HtdfContract(rpc=htdfrpc, address=contract_address, abi=abi)

    from_addr = Address(cfg['ADDRESS'])
    private_key = HtdfPrivateKey(cfg['PRIVATE_KEY'])
    from_acc = htdfrpc.get_account_info(address=from_addr.address)

    current_sequence = from_acc.sequence
    count = 0
    for address in addrs_list:

        addr = Address(address)
        amount = get_random_amount( cfg['RANDOM_START'], cfg['RANDOM_END'] )

        transfer_tx = hc.functions.transfer(
            _to=to_checksum_address(addr.hex_address),
            _value=int(amount * 10 ** 18)). \
            buildTransaction_htdf()

        data = remove_0x_prefix(transfer_tx['data'])

        signed_tx = HtdfTxBuilder(
            from_address=from_addr,
            to_address=contract_address,
            amount_satoshi=0,
            sequence=current_sequence + count,
            account_number=from_acc.account_number,
            chain_id=htdfrpc.chain_id,
            gas_price=100,
            gas_wanted=200000,
            data=data,
            memo=cfg['MEMO']
        ).build_and_sign(private_key=private_key)

        tx_hash = htdfrpc.broadcast_tx(tx_hex=signed_tx)
        print('{}, {}, tx_hash: {}'.format(token_name, count , tx_hash))
        # self.assertTrue( len(tx_hash) == 64)

        # tx = htdfrpc.get_tranaction_until_timeout(transaction_hash=tx_hash)
        # pprint(tx)

        # cfnBalanceOf = hc.functions.balanceOf(_owner=to_checksum_address(new_to_addr.hex_address))
        # balance = hc.call(cfn=cfnBalanceOf)
        # print(balance)

        count += 1
        if count % 40 == 0:
            time.sleep(4.5)

    pass


def main():
    # print(get_random_amount(10, 20))

    # addrs = get_addresses(COMMON['ADDRESS_FILE'])
    # print(len(addrs))

    for name, cfg in TOKEN_CFG.items():
        contract_address = HRC20_TOKENS[name]
        addrs = get_addresses(COMMON['ADDRESS_FILE'],
                 cfg['ADDRESS_START_INDEX'], 
                 cfg['ADDRESS_END_INDEX'])
        hrc20_batch_transfer( name, contract_address,  addrs, cfg)

    pass


if __name__ == '__main__':
    main()
    pass
