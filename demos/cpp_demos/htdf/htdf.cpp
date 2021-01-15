// author: yqq
// date: 2021-01-15
// descriptions: htdf transaction signature 

#include <map>
#include <exception>
#include <ctime>
// #include <time.h>

#include "htdf.h"
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include "crypto/strencodings.h"
#include "crypto/hash.h"
#include "bech32/bech32.h"
#include "crypto/strencodings.h"
#include "crypto/tinyformat.h"
#include "crypto/string.h"
#include "crypto/tinyformat.h"

#include "json.hpp"
using json = nlohmann::json;

// using Client=httplib::Client;

using namespace htdf;


CRawTx::CRawTx()
{
    uAccountNumber = INTMAX_MAX;
    memset(szChainId, 0, sizeof(szChainId));
    uFeeAmount = 0;
    memset(szFeeDenom, 0, sizeof(szFeeDenom));
    uGas = 0;
    memset(szMemo, 0, sizeof(szMemo));
    uMsgAmount = 0;
    memset(szMsgDenom, 0, sizeof(szMsgDenom));
    memset(szMsgFrom, 0, sizeof(szMsgFrom));
    memset(szMsgTo, 0, sizeof(szMsgTo));
    uSequence = INTMAX_MAX;

    memset(szData, 0, sizeof(szData));
}

CRawTx::CRawTx(const CRawTx &other)
{
    uAccountNumber = other.uAccountNumber;
    memcpy(szChainId, other.szChainId, sizeof(szChainId));
    uFeeAmount = other.uFeeAmount;
    memcpy(szFeeDenom, other.szFeeDenom, sizeof(szFeeDenom));
    uGas = other.uGas;
    memcpy(szMemo, other.szMemo, sizeof(szMemo));
    uMsgAmount = other.uMsgAmount;

    memcpy(szMsgDenom, other.szMsgDenom, sizeof(szMsgDenom));
    memcpy(szMsgFrom, other.szMsgFrom, sizeof(szMsgFrom));
    memcpy(szMsgTo, other.szMsgTo, sizeof(szMsgTo));
    uSequence = other.uSequence;

    memcpy(szData, other.szData, sizeof(szData));
}

CRawTx &CRawTx::operator=(const CRawTx &other)
{
    if (this == &other)
        return *this;

    uAccountNumber = other.uAccountNumber;
    memcpy(szChainId, other.szChainId, sizeof(szChainId));
    uFeeAmount = other.uFeeAmount;
    memcpy(szFeeDenom, other.szFeeDenom, sizeof(szFeeDenom));
    uGas = other.uGas;
    memcpy(szMemo, other.szMemo, sizeof(szMemo));
    uMsgAmount = other.uMsgAmount;

    memcpy(szMsgDenom, other.szMsgDenom, sizeof(szMsgDenom));
    memcpy(szMsgFrom, other.szMsgFrom, sizeof(szMsgFrom));
    memcpy(szMsgTo, other.szMsgTo, sizeof(szMsgTo));
    uSequence = other.uSequence;

    memcpy(szData, other.szData, sizeof(szData));
    return *this;
}

bool CRawTx::toString(string &strOut)
{
    if (false == checkParams(strOut))
    {
        return false;
    }
    strOut.clear();

    string strJson;

    strJson += "{"; //root

    char buf[2046] = {0};

    //account_number
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"account_number\":\"%lu\"", uAccountNumber);
    strJson += buf;
    strJson += ",";

    //chain_id
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"chain_id\":\"%s\"", szChainId);
    strJson += buf;
    strJson += ",";

    //fee
    strJson += "\"fee\":{";
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"gas_price\":\"%lu\",\"gas_wanted\":\"%lu\"", uFeeAmount, uGas);
    strJson += buf;
    strJson += "}";
    strJson += ",";

    //memo
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"memo\":\"%s\"", szMemo);
    strJson += buf;
    strJson += ",";

    //msgs
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"msgs\":[{\"Amount\":[{\"amount\":\"%lu\",\"denom\":\"%s\"}],\"Data\":\"%s\",\"From\":\"%s\",\"GasPrice\":%lu,\"GasWanted\":%lu,\"To\":\"%s\"}]",
            uMsgAmount, szMsgDenom, szData, szMsgFrom, uFeeAmount, uGas, szMsgTo);
    strJson += buf;
    strJson += ",";

    //sequence
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "\"sequence\":\"%lu\"", uSequence);
    strJson += buf;

    strJson += "}"; //root

    strOut = strJson;
    return true;
}

bool CRawTx::checkParams(string &strErrMsg)
{
    if (INTMAX_MAX == uAccountNumber || 0 == uAccountNumber)
    {
        // strErrMsg = tfm::format("invalid `account_number`: %lu , must between %lu and %lu.") % uAccountNumber % 0 % INTMAX_MAX);
        strErrMsg = tfm::format("invalid `account_number`: %lu , must between %lu and %lu.", uAccountNumber, 0, INTMAX_MAX);
        return false;
    }

    if (!(0 == strcmp(STR_MAINCHAIN, szChainId) || 0 == strcmp(STR_TESTCHAIN, szChainId)))
    {
        strErrMsg = tfm::format("invalid `chain_id`: %s, must be '%s'or '%s' . ", szChainId, STR_MAINCHAIN, STR_TESTCHAIN);
        return false;
    }

    if (UINT_MAX_FEE_AMOUNT < uFeeAmount || uFeeAmount < UINT_MIN_FEE_AMOUNT)
    {
        strErrMsg = tfm::format("invalid `fee amount`, must between %d and %d.", UINT_MIN_FEE_AMOUNT, UINT_MAX_FEE_AMOUNT);
        return false;
    }

    if (UINT_MAX_GAS_AMOUNT < uGas || uGas < UINT_MIN_GAS_AMOUNT)
    {
        strErrMsg = tfm::format("invalid `fee gas` : %lu, must between %lu and %lu.", uGas, UINT_MIN_GAS_AMOUNT, UINT_MAX_GAS_AMOUNT);
        return false;
    }

    if (!(0 == strcmp(STR_SATOSHI, szFeeDenom)))
    {
        strErrMsg = tfm::format("invalid `fee denom` : %s, must be `%s`.", szFeeDenom, STR_SATOSHI);
        return false;
    }

    if (!(0 == strcmp(STR_SATOSHI, szMsgDenom)))
    {
        strErrMsg = tfm::format("invalid `msgs amount denom` : %s, must be `%s`.", szMsgDenom, STR_SATOSHI);
        return false;
    }

    int nAddrLen = UINT_ADDR_LEN;
    if (nAddrLen != strlen(szMsgFrom))
    {
        strErrMsg = tfm::format("invalid address `msg From`:%s, address length must be %d.", szMsgFrom, nAddrLen);
        return false;
    }

    if (nAddrLen != strlen(szMsgTo))
    {
        strErrMsg = tfm::format("invalid address `msg To`:%s, address length must be %d.", szMsgTo, nAddrLen);
        return false;
    }

    if (0 != string(szMsgFrom).find(STR_HTDF "1"))
    {
        strErrMsg = tfm::format("invalid address `From`:%s.", szMsgFrom);
        return false;
    }

    if (0 != string(szMsgTo).find(STR_HTDF "1"))
    {
        strErrMsg = tfm::format("invalid address `To`:%s.", szMsgTo);
        return false;
    }

    if (INTMAX_MAX <= uSequence)
    {
        strErrMsg = "invalid `sequence` , TOO LARGE, this is tx number of the address in the node.";
        return false;
    }

    return true;
}

CBroadcastTx::CBroadcastTx()
{
    strType = STR_BROADCAST_TYPE; //"auth/StdTx";
    rtx = CRawTx();
    strMsgType = STR_BROADCAST_MSG_TYPE;        // "htdfservice/send";
    strPubKeyType = STR_BROADCAST_PUB_KEY_TYPE; //"tendermint/PubKeySecp256k1";
    strPubkeyValue = "";
    strSignature = "";
}

CBroadcastTx::CBroadcastTx(const CBroadcastTx &other)
{
    strType = other.strType;
    rtx = other.rtx;
    strMsgType = other.strMsgType;
    strPubKeyType = other.strPubKeyType;
    strPubkeyValue = other.strPubkeyValue;
    strSignature = other.strSignature;
}

CBroadcastTx &CBroadcastTx::operator=(const CBroadcastTx &other)
{
    strType = other.strType;
    rtx = other.rtx;
    strMsgType = other.strMsgType;
    strPubKeyType = other.strPubKeyType;
    strPubkeyValue = other.strPubkeyValue;
    strSignature = other.strSignature;

    return *this;
}

bool CBroadcastTx::toString(string &strRet)
{
    string strErrMsg;
    if (false == checkParams(strErrMsg))
    {
        strRet = strErrMsg;
        return false;
    }

    string strJson;
    strJson += "{"; //root

    //type
    strJson += "\"type\":\"" + strType + "\",";

    //value
    strJson += "\"value\":{";

    //msg
    strJson += tfm::format("\
			\"msg\":[{\
				\"type\":\"%s\",\
				\"value\":{\
					\"From\":\"%s\",\
					\"To\":\"%s\",\
					\"Amount\":[{\
						\"denom\":\"%s\",\
						\"amount\":\"%lu\"\
					}],\
					\"Data\":\"%s\",\
					\"GasPrice\":\"%lu\",\
					\"GasWanted\":\"%lu\"\
				  }\
			}],",
                           strMsgType,
                           rtx.szMsgFrom,
                           rtx.szMsgTo, rtx.szMsgDenom, rtx.uMsgAmount, rtx.szData, rtx.uFeeAmount, rtx.uGas);

    //fee
    strJson += tfm::format("\
				\"fee\":{\
					\"gas_price\":\"%lu\",\
					\"gas_wanted\":\"%lu\"\
				},",
                           rtx.uFeeAmount,
                           rtx.uGas);

    //signatures
    strJson += tfm::format("\
			\"signatures\":[{\
				\"pub_key\":{\
					\"type\":\"%s\",\
					\"value\":\"%s\"\
				},\
				\"signature\":\"%s\"\
			}],",
                           strPubKeyType,
                           strPubkeyValue, strSignature);

    strJson = RemoveSpace(strJson);
    strJson += tfm::format("\"memo\":\"%s\"", rtx.szMemo);

    strJson += "}"; //value
    strJson += "}"; //root

    strRet = strJson;
    return true;
}

bool CBroadcastTx::checkParams(string &strErrMsg)
{

    if (false == rtx.checkParams(strErrMsg))
    {
        strErrMsg += "HtdfBroadcastTx::ParamsCheck:";
        return false;
    }

    if (STR_BROADCAST_MSG_TYPE != strMsgType && STR_BROADCAST_MSG_TYPE_HET != strMsgType)
    {
        strErrMsg = tfm::format("invalid `msg type` : '%s', must be '%s' or '%s'.", strMsgType, STR_BROADCAST_MSG_TYPE, STR_BROADCAST_MSG_TYPE_HET);
        return false;
    }

    if (STR_BROADCAST_PUB_KEY_TYPE != strPubKeyType)
    {
        strErrMsg = tfm::format("invalid `pub_key type` : '%s', must be '%s'.", strPubKeyType, STR_BROADCAST_PUB_KEY_TYPE);
        return false;
    }

    if (strPubkeyValue.empty())
    {
        strErrMsg = tfm::format("invalid `pub_key value`: is empty, must be base64(pubkey).");
        return false;
    }

    string strTmpDecode;

    strTmpDecode = DecodeBase64(strPubkeyValue);
    if (UINT_PUB_KEY_LEN != strTmpDecode.size())
    {
        strErrMsg = tfm::format("invalid `pub_key value` length %d is not %d. After base64 decode, pubkey's length must be %d.", strTmpDecode.size(), UINT_PUB_KEY_LEN, UINT_PUB_KEY_LEN);
        return false;
    }

    strTmpDecode.clear();
    strTmpDecode = DecodeBase64(strSignature);
    if (UINT_SIG_RS_LEN != strTmpDecode.size())
    {
        strErrMsg = tfm::format("invalid `signature` length is not %d. After base64 decode, signature's length must be %d.", UINT_SIG_RS_LEN, UINT_SIG_RS_LEN);
        return false;
    }

    return true;
}

bool CBroadcastTx::toHexStr(string &strOut)
{
    string strErrMsg;
    if (false == checkParams(strErrMsg))
    {
        strOut = strErrMsg;
        return false;
    }

    string strRet;
    string strHex;
    if (false == toString(strErrMsg))
    {
        strOut = strErrMsg;
        return false;
    }

    toString(strHex);

    strRet = "";
    for (size_t i = 0; i < strHex.size(); i++)
    {
        strRet += tfm::format("%02x", ((int)strHex[i]));
    }
    strOut = strRet;

    return true;
}

Amount::Amount()
    : _m_amount(0), _m_amountSatoshi(0)
{
}

Amount::Amount(double htdf) : _m_amount(htdf)
{
    _m_amountSatoshi = (uint64_t)(htdf * 100000000.0);
}

Amount::Amount(uint64_t satoshi) : _m_amountSatoshi(satoshi)
{
    _m_amount = (double)(satoshi / 100000000.0);
}

Amount Amount::fromStringHtdf(string htdf)
{
    double amount;
    if (!ParseDouble(htdf, &amount))
    {
        amount = 0;
    }
    return std::move(Amount(amount));
}
Amount Amount::fromStringSatoshi(string satoshi)
{
    uint64_t amount;
    if (!ParseUInt64(satoshi, &amount))
    {
        amount = 0;
    }
    return std::move(Amount(amount));
};

double Amount::GetHtdf() const
{
    return _m_amount;
}

uint64_t Amount::GetSatoshi() const
{
    return _m_amountSatoshi;
}


CRpc::CRpc(string strNodeHost, string chainid, int port)
    : _m_pHttp(new httplib::Client(strNodeHost, port)),
      _m_port(port),
      _m_host(strNodeHost)
{
}

string CRpc::GetURL(ApiName apiName, string arg1, string arg2, string arg3)
{
    string url;

    string host = tfm::format("http://%s:%d", _m_host, _m_port);
    switch (apiName)
    {
    case ACCOUNT_INFO:
        url = tfm::format(host + "/auth/accounts/%s", arg1);
        break;
    case BROADCAST:
        url = tfm::format(host + "/hs/broadcast");
        break;
    case TXS:
        url = tfm::format(host + "/txs/%s", arg1);
        break;
    case LATEST_BLOCK:
        url = tfm::format(host + "/blocks/latest");
        break;
    case BLOCK_DETAILS:
        url = tfm::format(host + "/block_detail/%s", arg1);
        break;
    default:
        break;
    }

    // cout << "URL: " << url << endl;
    return url;
}

CActInfo CRpc::GetAccountInfo(string strAddress)
{
    CActInfo act;
    string url = GetURL(ACCOUNT_INFO, strAddress);
    auto res = _m_pHttp->Get(url.c_str());
    // cout << res->body << endl;

    if (200 != res->status)
    {
        cerr << "error" << res->status << endl;
        throw runtime_error("get account" + strAddress + " failed ");
    }

    auto jrsp = json::parse(res->body);
    act.address = jrsp["value"]["address"];
    assert(ParseUInt32(jrsp["value"]["account_number"], &act.account_number));
    assert(ParseUInt64(jrsp["value"]["sequence"], &act.sequence));
    assert(ParseDouble(jrsp["value"]["coins"][0]["amount"], &act.balance));
    act.active = true;
    return act;
}

CTx CRpc::GetTransaction(string strTxHash)
{
    CTx tx;
    string url = GetURL(TXS, strTxHash);
    auto res = _m_pHttp->Get(url.c_str());
    // cout << res->body << endl;
    if (200 != res->status)
    {
        cerr << "error" << res->status << endl;
        throw runtime_error("get tx " + strTxHash + " failed ");
    }

    auto jrsp = json::parse(res->body);

    auto logs = jrsp["logs"][0];
    tx.log = logs["log"];
    tx.success = logs["success"];
    assert(ParseUInt32(jrsp["height"], &tx.height));
    tx.hash = jrsp["txhash"];
    assert(ParseUInt32(jrsp["gas_used"], &tx.gasUsed));
    auto txv = jrsp["tx"]["value"];
    tx.timstamp = jrsp["timestamp"];

    auto msg = txv["msg"][0]; // we only handle single msg, do not handle multiple msg
    tx.msgType = msg["type"];
    auto msgValue = msg["value"];
    if ("htdfservice/send" == tx.msgType )
    {
        tx.from = msgValue["From"];
        tx.to = msgValue["To"];
        tx.amount = Amount::fromStringSatoshi(msgValue["Amount"][0]["amount"]);
        tx.data = msgValue["Data"];
        assert(ParseUInt32(msgValue["GasPrice"], &tx.gasPrice));
        assert(ParseUInt32(msgValue["GasWanted"], &tx.gasWanted));
    }
    else
    {
        // TODO: parse other msg type
    }

    tx.memo = txv["memo"];
    return std::move(tx);
}

CBroadcastRsp CRpc::Broadcast(string strSignedTx)
{
    CBroadcastRsp ret;
    string url = GetURL(BROADCAST);

    string data = tfm::format(R"({"tx":"%s"})", strSignedTx);
    auto res = _m_pHttp->Post(url.c_str(), data, "application/json");

    if (200 != res->status)
    {
        cerr << "error" << res->status << endl;
        throw runtime_error( "reponse body: " + res->body);
    }

    auto jrsp = json::parse(res->body);
    ret.tx_hash = jrsp["txhash"];
    // ret.raw_log = jrsp["raw_log"];
    if (jrsp.contains("raw_log"))
    {
        ret.raw_log = jrsp["raw_log"];
    }

    return ret;
}

CBlock CRpc::GetBlock(uint32_t height)
{
    CBlock block;
    string url = GetURL(BLOCK_DETAILS, ToString(height));
    auto res = _m_pHttp->Get(url.c_str());
    if (200 != res->status)
    {
        cerr << "error" << res->status << endl;
        throw runtime_error( "get block " + ToString(height) + " failed");
    }

    auto jrsp = json::parse(res->body);
    assert(ParseUInt32(jrsp["block_meta"]["header"]["height"], &block.height));
    assert(ParseInt32(jrsp["block_meta"]["header"]["num_txs"], &block.num_txs));
    block.hash = jrsp["block_meta"]["block_id"]["hash"];

    string strTime = jrsp["block_meta"]["header"]["time"];
    strTime = strTime.substr(0, strTime.find("."));
    strTime = strTime.replace(strTime.find("T"), 1, " ");
    block.blocktime = strTime;

    // tm ts;
    // memset(&ts, 0, sizeof(ts));
    // char *pret = strptime(strTime.c_str(), "%Y-%m-%d %H:%M:%S", &ts);
    // cerr << "strptime error" << pret << endl;
    // time_t tim = mktime(&ts);
    // cout << tim << endl;
    // block.blocktime = (int64_t)tim;
    // cout << "blocktime:" << block.blocktime << endl;

    auto txs = jrsp["block"]["txs"];
    if (!txs.is_null())
    {
        for (size_t i = 0; i < txs.size(); i++)
        {
            auto tx = txs[i];

            // Because we could not get tx status from block response
            // So we get the transaction details by API `/txs/{tx_hash}`
            block.txs.push_back(std::move(GetTransaction(tx["Hash"])));
        }
    }

    return std::move(block);
}

CBlock CRpc::GetLatestBlock()
{
    CBlock block;

    string url = GetURL(LATEST_BLOCK);
    auto res = _m_pHttp->Get(url.c_str());
    if (200 != res->status)
    {
        throw runtime_error("get latest block failed");
    }

    auto jrsp = json::parse(res->body);
    uint32_t height;
    assert(ParseUInt32(jrsp["block_meta"]["header"]["height"], &height));

    // 'txs' of "/blocks/latest" reponse is not be deserialized,
    //  so we should get txs by "/block_detail/"
    return GetBlock(height);
}

CTxBuilder::CTxBuilder(
    string chainid,
    string from,
    string to,
    uint64_t amountSatoshi,
    uint64_t sequence,
    uint32_t accountNumber,
    string memo,
    string data,
    uint32_t gasWanted,
    uint32_t gasPrice)
    : _m_chainid(chainid), _m_from(from), _m_to(to),
      _m_sequence(sequence), _m_accountNumber(accountNumber),
      _m_gasPrice(gasPrice), _m_gasWanted(gasWanted),
      _m_data(data), _m_amountSatoshi(amountSatoshi)
{
}

string CTxBuilder::Build()
{
    string ret;
    CRawTx &rtx = _m_rtx;
    rtx.uAccountNumber = _m_accountNumber;
    strcpy(rtx.szChainId, _m_chainid.c_str());
    strcpy(rtx.szFeeDenom, STR_SATOSHI);
    strcpy(rtx.szMemo, _m_memo.c_str());
    strcpy(rtx.szMsgDenom, STR_SATOSHI);
    strcpy(rtx.szMsgFrom, _m_from.c_str());
    strcpy(rtx.szMsgTo, _m_to.c_str());
    rtx.uMsgAmount = _m_amountSatoshi;
    rtx.uGas = _m_gasWanted;
    rtx.uFeeAmount = _m_gasPrice;
    rtx.uSequence = _m_sequence;

    if (rtx.toString(ret))
    {
        _m_unsignedTx = ret;
    }
    else
    {
        throw runtime_error("toString error: " + ret);
    }

    return ret;
}

string CTxBuilder::Sign(const string& hexPrivKey)
{
    string ret;

    CBroadcastTx &csBTx = _m_csBTx;
    csBTx.rtx = _m_rtx;
    csBTx.strType = STR_BROADCAST_TYPE;
    csBTx.strMsgType = STR_BROADCAST_MSG_TYPE;
    csBTx.strPubKeyType = STR_BROADCAST_PUB_KEY_TYPE;

    htdf::CPrivateKey privKey(hexPrivKey);
    if (!privKey.isValid())
    {
        throw runtime_error(" private key is invalid  error");
    }

    string pubKey = privKey.getPubkey().get();
    csBTx.strPubkeyValue = EncodeBase64(pubKey);

    unsigned char uszShaData[CSHA256::OUTPUT_SIZE] = {0};
    memset(uszShaData, 0, sizeof(uszShaData));
    CSHA256 sh256;
    sh256.Write((unsigned char *)_m_unsignedTx.data(), _m_unsignedTx.size());
    sh256.Finalize(uszShaData);

    std::string strSha256 = Bin2HexStr(uszShaData, sizeof(uszShaData));
    std::string strPrivKey = privKey.get() ;//HexToBin(privateKey);
    unsigned char uszSigOut[64] = {0};
    memset(uszSigOut, 0, sizeof(uszSigOut));
    unsigned int uSigOutLen = 0;
    char szErrMsg[256] = {0};
    memset(szErrMsg, 0, sizeof(szErrMsg));

    int iRet = CPrivateKey::sign(uszShaData, sizeof(uszShaData),
                    (unsigned char *)strPrivKey.data(),
                    strPrivKey.size(),
                    uszSigOut,
                    sizeof(uszSigOut),
                    &uSigOutLen,
                    szErrMsg);
    if (0 != iRet)
    {
        ret = string(szErrMsg);
        return ret;
    }

    csBTx.strSignature = EncodeBase64(Bin2ByteArray(uszSigOut, sizeof(uszSigOut)));
    csBTx.toHexStr(ret);
    return ret;
}


string CTx::ToString() const
{
    return tfm::format(
        "hash:%s\n"
        "from:%s\n"
        "to:%s\n"
        "amount:%.8f htdf\n"
        "memo:%s\n"
        "data:%s\n"
        "gasWanted:%u\n"
        "gasPrice:%u\n"
        "gasUsed:%u\n"
        "height:%u\n"
        "timestamp:%s\n"
        "success:%d\n"
        "log:%s\n"
        "msgtype:%s\n"
        ,hash, from, to, amount.GetHtdf(),
        memo, data, gasWanted, gasPrice, gasUsed, 
        height, timstamp, (success)?("true"):("false"),
        log, msgType
    );
}

ostream& operator << (ostream& os, const htdf::CTx &tx)
{
    os << "===============" << endl;
    os << tx.ToString();
    os << "===============" << endl;
    return os;
}



CPublickey::CPublickey(const string& hexPubkey)
:_m_pubkey(HexToBin(hexPubkey)),_m_fValid(false)
{
}

CPublickey::CPublickey(unsigned char* pbuf, int len)
:_m_pubkey(string((char *)pbuf, len)),_m_fValid(false)
{
}

string CPublickey::hexString() const
{
    return Bin2HexStr((const unsigned char*)_m_pubkey.data(), _m_pubkey.size());
}

string CPublickey::get() const
{
    return _m_pubkey;
}


string CPublickey::getBech32Address() const
{
    CHash160 hash160;
    vector<unsigned char> pubk(_m_pubkey.begin(), _m_pubkey.end());
    vector<unsigned char> out(CRIPEMD160::OUTPUT_SIZE);
    hash160.Write(pubk);
    hash160.Finalize(out);

    vector<unsigned char> conv;
    bech32::convertbits<8, 5, true>(conv, out);
    string strBech32Addr = bech32::encode(STR_HTDF, conv);
    return strBech32Addr;
}

string CPublickey::getHexAddress() const
{
    CHash160 hash160;
    vector<unsigned char> pubk(_m_pubkey.begin(), _m_pubkey.end());
    vector<unsigned char> out(CRIPEMD160::OUTPUT_SIZE);
    hash160.Write(pubk);
    hash160.Finalize(out);

    vector<unsigned char> conv;
    bech32::convertbits<8, 5, true>(conv, out);
    return HexStr(conv);
}

bool CPublickey::checkPubKey(const string& pubkey)
{
    // TODO:
    return true;
}

string CPublickey::convertBech32ToHex(const string& bech32Addr)
{
    auto ret = bech32::decode(bech32Addr);
    return HexStr( ret.second );
}

string CPublickey::convertHexToBech32(const string& hexAddr, const string& hrp)
{
    string data = HexToBin(hexAddr);
    bech32::data vctData(data.begin(), data.end());
    string strBech32Addr = bech32::encode(hrp, vctData);
    return strBech32Addr;
}


CPrivateKey::CPrivateKey(string hexPrivkey)
:_m_fValid(false)
{
    string tmpkey = HexToBin(hexPrivkey);
    if(checkPrivkey(tmpkey))
    {
        _m_privkey = std::move(tmpkey);
        _m_fValid = true;
    }
}

CPrivateKey::CPrivateKey(unsigned char* pkey, int len)
:_m_fValid(false)
{
    string tmpkey((const char *)pkey, len);
    // do not throw exceptions
    if(checkPrivkey(tmpkey))
    {
        _m_privkey = std::move(tmpkey);
        _m_fValid = true;
    }
}


bool CPrivateKey::isValid()const
{
    return _m_fValid;
}

CPublickey CPrivateKey::getPubkey()const
{
    if(!_m_fValid)
    {
        throw runtime_error("invalid private key");
    }

    secp256k1_pubkey pubkey;
    memset(pubkey.data, 0, sizeof(pubkey.data));

    auto *ctx = GetSecp256k1Ctx();

    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, (unsigned char *)_m_privkey.data()))
    {
        throw runtime_error("secp256k1_ec_pubkey_create error");
    }

    unsigned char output[CPublickey::SIZE * 2] = {0};
    memset(output, 0, sizeof(output));
    size_t outputlen = CPublickey::SIZE;
    secp256k1_ec_pubkey_serialize(ctx, output, &outputlen, &pubkey, SECP256K1_EC_COMPRESSED);
    if (CPublickey::SIZE != outputlen)
    {
        throw runtime_error("secp256k1_ec_pubkey_serialize  error");
    }
    return std::move(CPublickey(output, outputlen));
}


string CPrivateKey::hexString() const
{
    return Bin2HexStr((const unsigned char *)_m_privkey.data(), CPrivateKey::SIZE);
}

string CPrivateKey::get() const
{
    return _m_privkey;
}

CPrivateKey CPrivateKey::newRandomPrivKey()
{
    string privkey(32, 0);
    do
    {
        GetRandom32Bytes((unsigned char*)privkey.data());
    } while (!checkPrivkey(privkey));
    return std::move(CPrivateKey(privkey));
}

bool CPrivateKey::checkPrivkey(const unsigned char *vch)
{
    auto *ctx = GetSecp256k1Ctx();
    return secp256k1_ec_seckey_verify(ctx, vch);
}

bool CPrivateKey::checkPrivkey(const string& privkey)
{
    return checkPrivkey((const unsigned char *)privkey.data());
}


int CPrivateKey::sign(
    unsigned char *pszIn,
    unsigned int uInLen,
    unsigned char *pszPrivKey,
    unsigned int uPrivKeyLen,
    unsigned char *pszOut,
    unsigned int uOutBufLen,
    unsigned int *puOutDataLen,
    char *pszErrMsg)
{
    if (NULL == pszErrMsg)
    {
        return 1;
    }

    if (NULL == pszIn)
    {
        strcpy(pszErrMsg, "pszIn is null.");
        return 1;
    }

    if (0 == uInLen)
    {
        strcpy(pszErrMsg, "uInLen is 0.");
        return 1;
    }

    if (NULL == pszPrivKey)
    {
        strcpy(pszErrMsg, "pszPrivKey is null.");
        return 1;
    }

    if (UINT_PRIV_KEY_LEN != uPrivKeyLen)
    {
        sprintf(pszErrMsg, "priv-key len is not %d bytes.", UINT_PRIV_KEY_LEN);
        return 1;
    }

    if (NULL == pszOut)
    {
        strcpy(pszErrMsg, "pszOut is null.");
        return 1;
    }

    if (uOutBufLen < UINT_SIG_RS_LEN)
    {
        sprintf(pszErrMsg, "uOutBufLen less than %d. Must more than %d.", UINT_SIG_RS_LEN, UINT_SIG_RS_LEN);
        return 1;
    }

    if (NULL == puOutDataLen)
    {
        strcpy(pszErrMsg, "puOutDataLen is null");
        return 1;
    }

    auto *ctx = GetSecp256k1Ctx();

    secp256k1_ecdsa_recoverable_signature rawSig;
    memset(&rawSig.data, 0, 65);
    if (!secp256k1_ecdsa_sign_recoverable(ctx, &rawSig, pszIn, pszPrivKey, nullptr, nullptr))
    {
        strcpy(pszErrMsg, "secp256k1_ecdsa_sign_recoverable  failed.");
        return 1;
    }

    int iRecid = 0;
    unsigned char uszSigRSData[UINT_SIG_RS_LEN] = {0};
    memset(uszSigRSData, 0, sizeof(uszSigRSData));
    secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, uszSigRSData, &iRecid, &rawSig);

    memcpy(pszOut, uszSigRSData, UINT_SIG_RS_LEN);
    *puOutDataLen = UINT_SIG_RS_LEN;

    // no need to destroy
    // secp256k1_context_destroy(const_cast<secp256k1_context*>(ctx));
    return 0;
}
