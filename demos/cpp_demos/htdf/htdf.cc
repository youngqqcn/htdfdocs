/************************************************************************
author: yqq
date: 2019-05-08  14:25
descriptions: htdf transaction signature 
************************************************************************/

#include "htdf.h"
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

using namespace htdf;

int htdf::sign(
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
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszIn)
    {
        strcpy(pszErrMsg, "pszIn is null.");
        return htdf::ARGS_ERROR;
    }

    if (0 == uInLen)
    {
        strcpy(pszErrMsg, "uInLen is 0.");
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszPrivKey)
    {
        strcpy(pszErrMsg, "pszPrivKey is null.");
        return htdf::ARGS_ERROR;
    }

    if (UINT_PRIV_KEY_LEN != uPrivKeyLen)
    {
        sprintf(pszErrMsg, "priv-key len is not %d bytes.", UINT_PRIV_KEY_LEN);
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszOut)
    {
        strcpy(pszErrMsg, "pszOut is null.");
        return htdf::ARGS_ERROR;
    }

    if (uOutBufLen < UINT_SIG_RS_LEN)
    {
        sprintf(pszErrMsg, "uOutBufLen less than %d. Must more than %d.", UINT_SIG_RS_LEN, UINT_SIG_RS_LEN);
        return htdf::ARGS_ERROR;
    }

    if (NULL == puOutDataLen)
    {
        strcpy(pszErrMsg, "puOutDataLen is null");
        return htdf::ARGS_ERROR;
    }

    auto *ctx = getCtx();

    secp256k1_ecdsa_recoverable_signature rawSig;
    memset(&rawSig.data, 0, 65);
    if (!secp256k1_ecdsa_sign_recoverable(ctx, &rawSig, pszIn, pszPrivKey, nullptr, nullptr))
    {
        strcpy(pszErrMsg, "secp256k1_ecdsa_sign_recoverable  failed.");
        return htdf::ECCSIGN_STEP1_ERROR;
    }

    int iRecid = 0;
    unsigned char uszSigRSData[UINT_SIG_RS_LEN] = {0};
    memset(uszSigRSData, 0, sizeof(uszSigRSData));
    secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, uszSigRSData, &iRecid, &rawSig);

    memcpy(pszOut, uszSigRSData, UINT_SIG_RS_LEN);
    *puOutDataLen = UINT_SIG_RS_LEN;

    // no need to destroy
    // secp256k1_context_destroy(const_cast<secp256k1_context*>(ctx));
    return htdf::NO_ERROR;
}

//输入: 十六进制字符串形式的私钥
//输出: 十六进制字符串形式的公钥
int htdf::PrivateKeyToCompressPubKey(const string &_strPrivKey, string &strPubKey)
{
    string strPrivKey = HexToBin(_strPrivKey);

    secp256k1_pubkey pubkey;
    memset(pubkey.data, 0, sizeof(pubkey.data));

    auto *ctx = getCtx();

    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, (unsigned char *)strPrivKey.data()))
    {
        return 1;
    }

    unsigned char output[1024] = {0};
    memset(output, 0, sizeof(output));
    size_t outputlen = 33;
    secp256k1_ec_pubkey_serialize(ctx, output, &outputlen, &pubkey, SECP256K1_EC_COMPRESSED);

    if (33 != outputlen)
    {
        return 1;
    }

    strPubKey = Bin2HexStr(output, outputlen);
    return 0;
}

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
        strErrMsg = boost::str(boost::format("invalid `account_number`: %lu , must between %lu and %lu.") % uAccountNumber % 0 % INTMAX_MAX);
        return false;
    }

    if (!(0 == strcmp(STR_MAINCHAIN, szChainId) || 0 == strcmp(STR_TESTCHAIN, szChainId)))
    {
        strErrMsg = boost::str(boost::format("invalid `chain_id`: %s, must be '%s'or '%s' . ") % szChainId % STR_MAINCHAIN % STR_TESTCHAIN);
        return false;
    }

    if (UINT_MAX_FEE_AMOUNT < uFeeAmount || uFeeAmount < UINT_MIN_FEE_AMOUNT)
    {
        strErrMsg = boost::str(boost::format("invalid `fee amount`, must between %d and %d.") % UINT_MIN_FEE_AMOUNT % UINT_MAX_FEE_AMOUNT);
        return false;
    }

    if (UINT_MAX_GAS_AMOUNT < uGas || uGas < UINT_MIN_GAS_AMOUNT)
    {
        strErrMsg = boost::str(boost::format("invalid `fee gas` : %lu, must between %lu and %lu.") % uGas % UINT_MIN_GAS_AMOUNT % UINT_MAX_GAS_AMOUNT);
        return false;
    }

    if (!(0 == strcmp(STR_SATOSHI, szFeeDenom)))
    {
        strErrMsg = boost::str(boost::format("invalid `fee denom` : %s, must be `%s`.") % szFeeDenom % STR_SATOSHI);
        return false;
    }

    if (!(0 == strcmp(STR_SATOSHI, szMsgDenom)))
    {
        strErrMsg = boost::str(boost::format("invalid `msgs amount denom` : %s, must be `%s`.") % szMsgDenom % STR_SATOSHI);
        return false;
    }

    int nAddrLen = UINT_ADDR_LEN;
    if (nAddrLen != strlen(szMsgFrom))
    {
        strErrMsg = boost::str(boost::format("invalid address `msg From`:%s, address length must be %d.") % szMsgFrom % nAddrLen);
        return false;
    }

    if (nAddrLen != strlen(szMsgTo))
    {
        strErrMsg = boost::str(boost::format("invalid address `msg To`:%s, address length must be %d.") % szMsgTo % nAddrLen);
        return false;
    }

    if (!boost::starts_with(szMsgFrom, string(STR_HTDF) + "1"))
    {
        strErrMsg = boost::str(boost::format("invalid address `From`:%s.") % szMsgFrom);
        return false;
    }

    if (!boost::starts_with(szMsgTo, string(STR_HTDF) + "1"))
    {
        strErrMsg = boost::str(boost::format("invalid address `To`:%s.") % szMsgTo);
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
    boost::trim(strJson);

    //value
    strJson += "\"value\":{";

    //msg
    strJson += boost::str(boost::format("\
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
			}],") % strMsgType %
                          rtx.szMsgFrom %
                          rtx.szMsgTo % rtx.szMsgDenom % rtx.uMsgAmount % rtx.szData % rtx.uFeeAmount % rtx.uGas);

    //fee
    strJson += boost::str(boost::format("\
				\"fee\":{\
					\"gas_price\":\"%lu\",\
					\"gas_wanted\":\"%lu\"\
				},") % rtx.uFeeAmount %
                          rtx.uGas);

    //signatures
    strJson += boost::str(boost::format("\
			\"signatures\":[{\
				\"pub_key\":{\
					\"type\":\"%s\",\
					\"value\":\"%s\"\
				},\
				\"signature\":\"%s\"\
			}],") % strPubKeyType %
                          strPubkeyValue % strSignature);

    strJson = boost::algorithm::erase_all_copy(strJson, " ");
    strJson = boost::algorithm::erase_all_copy(strJson, "\t");
    strJson = boost::algorithm::erase_all_copy(strJson, "\n");
    strJson = boost::algorithm::erase_all_copy(strJson, "\r");

    strJson += boost::str(boost::format("\"memo\":\"%s\"") % rtx.szMemo);

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
        strErrMsg = boost::str(boost::format("invalid `msg type` : '%s', must be '%s' or '%s'.") % strMsgType % STR_BROADCAST_MSG_TYPE % STR_BROADCAST_MSG_TYPE_HET);
        return false;
    }

    if (STR_BROADCAST_PUB_KEY_TYPE != strPubKeyType)
    {
        strErrMsg = boost::str(boost::format("invalid `pub_key type` : '%s', must be '%s'.") % strPubKeyType % STR_BROADCAST_PUB_KEY_TYPE);
        return false;
    }

    if (strPubkeyValue.empty())
    {

        strErrMsg = boost::str(boost::format("invalid `pub_key value` is empty, must be base64(pubkey).") % strPubKeyType % STR_BROADCAST_PUB_KEY_TYPE);
        return false;
    }

    string strTmpDecode;
    strTmpDecode = Base64Decode(strPubkeyValue.data(), strPubkeyValue.size(), false);
    if (UINT_PUB_KEY_LEN != strTmpDecode.size())
    {
        strErrMsg = boost::str(boost::format("invalid `pub_key value` length %d is not %d. After base64 decode, pubkey's length must be %d.") % strTmpDecode.size() % UINT_PUB_KEY_LEN % UINT_PUB_KEY_LEN);
        return false;
    }

    strTmpDecode.clear();
    strTmpDecode = Base64Decode(strSignature.data(), strSignature.size(), false);
    if (UINT_SIG_RS_LEN != strTmpDecode.size())
    {
        strErrMsg = boost::str(boost::format("invalid `signature` length is not %d. After base64 decode, signature's length must be %d.") % UINT_SIG_RS_LEN % UINT_SIG_RS_LEN);
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
        strRet += boost::str(boost::format("%02x") % ((int)strHex[i]));
    }
    strOut = strRet;

    return true;
}
