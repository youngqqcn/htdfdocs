/************************************************************************
author: yqq
date: 2019-05-08  14:25
decriptions:  HTDF transaction signature wrapper
************************************************************************/

#ifndef __CPP_DEMOS_HTDF_H_
#define __CPP_DEMOS_HTDF_H_

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

// // secp256k1
// #include <secp256k1.h>
// #include <secp256k1_recovery.h>

// //sha256
// #include <openssl/sha.h>

// //base64
// #include "openssl/evp.h"
// #include "openssl/bio.h"
// #include "openssl/buffer.h"

// bech32
#include "utils.h"
// #include "bech32/bech32.h"

using namespace std;

#define UINT_MAX_FEE_AMOUNT (2000000000)
#define UINT_MIN_FEE_AMOUNT (10)
#define UINT_MAX_GAS_AMOUNT (200000000)
#define UINT_MIN_GAS_AMOUNT (30000)
#define UINT_ADDR_LEN (43)
#define UINT_PRIV_KEY_LEN (32)
#define UINT_PUB_KEY_LEN (33)
#define UINT_SIG_RS_LEN (64)
#define UINT_TXID_CHAR_LEN (64)

#define STR_MAINCHAIN "mainchain"
#define STR_TESTCHAIN "testchain"

#define STR_HTDF "htdf"
#define STR_SATOSHI "satoshi"

#define STR_BROADCAST_TYPE "auth/StdTx"
#define STR_BROADCAST_MSG_TYPE "htdfservice/send"
#define STR_BROADCAST_MSG_TYPE_HET "hetservice/send"
#define STR_BROADCAST_PUB_KEY_TYPE "tendermint/PubKeySecp256k1"



namespace htdf
{
    int sign(
        unsigned char *pszIn,
        unsigned int uInLen,
        unsigned char *pszPrivKey,
        unsigned int uPrivKeyLen,
        unsigned char *pszOut,
        unsigned int uOutBufLen,
        unsigned int *puOutDataLen,
        char *pszErrMsg);

    int PrivateKeyToCompressPubKey(
        const std::string &_strPrivKey, 
        std::string &strPubKey);


    enum ErrorCode
    {
        NO_ERROR = 0,
        ARGS_ERROR = 1001,
        ECCSIGN_STEP1_ERROR = 1005,
        ECCSIGN_STEP2_ERROR = 1006
    };

   
    // inline secp256k1_context const *getCtx()
    // {
    //     static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
    //         secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
    //         &secp256k1_context_destroy};
    //     return s_ctx.get();
    // }

    int PrivateKeyToCompressPubKey(const std::string &_strPrivKey, std::string &strPubKey);
  
   
    struct CRawTx
    {
        uint64_t uAccountNumber;
        char szChainId[128];
        uint64_t uFeeAmount;
        char szFeeDenom[128];
        uint64_t uGas;
        char szMemo[256];
        uint64_t uMsgAmount;
        char szMsgDenom[128];
        char szMsgFrom[128];
        char szMsgTo[128];
        uint64_t uSequence;

        char szData[1024];

        CRawTx()
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

        CRawTx(const CRawTx &other)
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

        CRawTx &operator=(const CRawTx &other)
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

        

        bool toString(std::string &strOut)
        {
            if (false == checkParams(strOut))
            {
                return false;
            }
            strOut.clear();

            std::string strJson;

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

        bool checkParams(std::string &strErrMsg)
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

            if (!boost::starts_with(szMsgFrom, std::string(STR_HTDF) + "1"))
            {
                strErrMsg = boost::str(boost::format("invalid address `From`:%s.") % szMsgFrom);
                return false;
            }

            if (!boost::starts_with(szMsgTo, std::string(STR_HTDF) + "1"))
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
    };

    struct CBroadcastTx
    {
        std::string strType; //"auth/StdTx"

        CRawTx rtx;
        std::string strMsgType;     //"htdfservice/send"
        std::string strPubKeyType;  //type
        std::string strPubkeyValue; //value
        std::string strSignature;   //signature

        CBroadcastTx()
        {
            strType = STR_BROADCAST_TYPE; //"auth/StdTx";
            rtx = CRawTx();
            strMsgType = STR_BROADCAST_MSG_TYPE;        // "htdfservice/send";
            strPubKeyType = STR_BROADCAST_PUB_KEY_TYPE; //"tendermint/PubKeySecp256k1";
            strPubkeyValue = "";
            strSignature = "";
        }

        CBroadcastTx(const CBroadcastTx &other)
        {
            strType = other.strType;
            rtx = other.rtx;
            strMsgType = other.strMsgType;
            strPubKeyType = other.strPubKeyType;
            strPubkeyValue = other.strPubkeyValue;
            strSignature = other.strSignature;
        }

        CBroadcastTx &operator=(const CBroadcastTx &other)
        {
            strType = other.strType;
            rtx = other.rtx;
            strMsgType = other.strMsgType;
            strPubKeyType = other.strPubKeyType;
            strPubkeyValue = other.strPubkeyValue;
            strSignature = other.strSignature;

            return *this;
        }

       
        bool toString(std::string &strRet)
        {
            std::string strErrMsg;
            if (false == checkParams(strErrMsg))
            {
                strRet = strErrMsg;
                return false;
            }

            std::string strJson;
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
			}],") % strMsgType % rtx.szMsgFrom %
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

        bool checkParams(std::string &strErrMsg)
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

      
        bool toHexStr(std::string &strOut)
        {
            std::string strErrMsg;
            if (false == checkParams(strErrMsg))
            {
                strOut = strErrMsg;
                return false;
            }

            std::string strRet;
            std::string strHex;
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

    }; // struct HtdfBroadcastTx

} //namespace htdf
#endif // __CPP_DEMOS_HTDF_H_
