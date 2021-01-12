/************************************************************************
author: yqq
date: 2019-05-08  14:25
decriptions:  HTDF transaction signature wrapper
************************************************************************/

#ifndef _HTDF_H_
#define _HTDF_H_

#include <iostream>
#include <string>
#include <vector>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

// secp256k1
// #include "secp256k1/secp256k1.h"
// #include "secp256k1/secp256k1_recovery.h"
#include <secp256k1.h>
#include <secp256k1_recovery.h>

//sha256
// #include "openssl/sha.h"  
#include <openssl/sha.h>

//base64
#include "openssl/evp.h"
#include "openssl/bio.h"
#include "openssl/buffer.h"

// bech32
#include "bech32/bech32.h"  

using namespace std;


#define  UINT_MAX_FEE_AMOUNT	(2000000000)
#define  UINT_MIN_FEE_AMOUNT	(10)		
#define  UINT_MAX_GAS_AMOUNT	(200000000) 
#define  UINT_MIN_GAS_AMOUNT	(30000)		
#define  UINT_ADDR_LEN    (43)
#define  UINT_PRIV_KEY_LEN		(32)		
#define  UINT_PUB_KEY_LEN		(33)		
#define  UINT_SIG_RS_LEN		(64)		
#define  UINT_TXID_CHAR_LEN          (64)       

#define  STR_MAINCHAIN			"mainchain"
#define  STR_TESTCHAIN			"testchain"

#define  STR_HTDF				"htdf"
#define  STR_SATOSHI            "satoshi"

#define  STR_BROADCAST_TYPE		"auth/StdTx"
#define  STR_BROADCAST_MSG_TYPE "htdfservice/send"
#define  STR_BROADCAST_MSG_TYPE_HET "hetservice/send"
#define  STR_BROADCAST_PUB_KEY_TYPE "tendermint/PubKeySecp256k1"

typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;

namespace htdf
{

int Sign(
	unsigned char *pszIn, 
	unsigned int uInLen, 
	unsigned char *pszPrivKey,
	unsigned int  uPrivKeyLen,
	unsigned char *pszOut,
	unsigned int uOutBufLen, 
	unsigned int *puOutDataLen, 
	char *pszErrMsg);


enum ErrorCode
	{
		NO_ERROR  = 0,
		ARGS_ERROR = 1001,
		ECCSIGN_STEP1_ERROR = 1005,
		ECCSIGN_STEP2_ERROR = 1006
	};


inline std::string Base64Encode(const char *input, int length, bool with_new_line)
{
    BIO * bmem = NULL;
    BIO * b64 = NULL;
    BUF_MEM * bptr = NULL;

    b64 = BIO_new(BIO_f_base64());
    if (!with_new_line) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char * buff = (char *)malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;
    BIO_free_all(b64);


    std::string  strRet(buff);
    free(buff); buff = NULL;
    return strRet;
}

inline std::string Base64Decode(const char * input, int length, bool with_new_line)
{
    BIO * b64 = NULL;
    BIO * bmem = NULL;
    char * buffer = (char *)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    if (!with_new_line) {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new_mem_buf((void *)input, length);
    bmem = BIO_push(b64, bmem);
    int iDecodeLen = BIO_read(bmem, buffer, length);

    BIO_free_all(bmem);

    std::string strRet;
    strRet.append(buffer, iDecodeLen);

    free(buffer); buffer = NULL;
    return strRet;
}

inline  std::string Bin2HexStr(const unsigned char *pBin, unsigned int len)
{
    std::string  strHex;
    for (unsigned int i = 0; i < len; i++)
    {
        char buf[10] = { 0 };
        memset(buf, 0, sizeof(buf));
        sprintf_s(buf, "%02x", pBin[i]);
        strHex += buf;

    }
    return strHex;
}

inline string HexToBin(const string &strHexIn)
{
    if (strHexIn.size() % 2 != 0) return "";

    std::string strHex = (boost::starts_with(strHexIn, "0x")) ? (strHexIn.substr(2)) : (strHexIn);

    string strBin;
    strBin.resize(strHex.size() / 2);
    for (size_t i = 0; i < strBin.size(); i++)
    {
        uint8_t cTemp = 0;
        for (size_t j = 0; j < 2; j++)
        {
            char cCur = strHex[2 * i + j];
            if (cCur >= '0' && cCur <= '9')
            {
                cTemp = (cTemp << 4) + (cCur - '0');
            }
            else if (cCur >= 'a' && cCur <= 'f')
            {
                cTemp = (cTemp << 4) + (cCur - 'a' + 10);
            }
            else if (cCur >= 'A' && cCur <= 'F')
            {
                cTemp = (cTemp << 4) + (cCur - 'A' + 10);
            }
            else
            {
                return "";
            }
        }
        strBin[i] = cTemp;
    }

    return strBin;
}



inline secp256k1_context const* getCtx()
{
    static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
        secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
        &secp256k1_context_destroy
    };
    return s_ctx.get();
}


inline int PrivKeyToCompressPubKey(const std::string &_strPrivKey, std::string &strPubKey)
{
    std::string strPrivKey = HexToBin(_strPrivKey);

    secp256k1_pubkey  pubkey;
    memset(pubkey.data, 0, sizeof(pubkey.data));

    static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
        secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
        &secp256k1_context_destroy
    };

    auto* ctx = getCtx();

    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, (unsigned char *)strPrivKey.data()))
    {
        return 1;
    }

    unsigned char output[1024] = { 0 }; memset(output, 0, sizeof(output));
    size_t  outputlen = 33;
    secp256k1_ec_pubkey_serialize(ctx, output, &outputlen, &pubkey, SECP256K1_EC_COMPRESSED);

    if (33 != outputlen)
    {
        return 1;
    }

    strPubKey = Bin2HexStr(output, outputlen);
    return 0;
}


bool  Bech32AddrToHexStrAddr(const std::string &strBech32AddrIn, std::string &strHexStrAddrOut) noexcept(false)
{
    std::string strErrMsg = "";

    if(strBech32AddrIn.length() < 40)
    {
        strErrMsg = std::string(" bech32addr's length less 40 or bech32addr is not htdf address.  ");
        throw runtime_error(strErrMsg);
    }

    std::pair<std::string , std::vector<uint8_t>> retpair = bech32::decode( strBech32AddrIn );
    if(std::string("htdf") != retpair.first  ||  retpair.second.size() < 30)
    {
        strErrMsg = std::string(" hrp is not `htdf` or decode data's size less than 30 ");
        throw runtime_error(strErrMsg);
    }

    std::vector<uint8_t> outdata;
    if(! bech32::convertbits<5, 8, false>(outdata, retpair.second) )
    {
        strErrMsg = std::string("bech32::convertbits failed  ");
        throw runtime_error(strErrMsg);
    }

    std::string  cstrHexRet = Bin2HexStr( outdata.data(), outdata.size());
	strHexStrAddrOut = cstrHexRet;

    return true;
}


struct HtdfRawTx
{
	uint64_t	uAccountNumber;
	char		szChainId[128];
	uint64_t	uFeeAmount;
	char		szFeeDenom[128];
	uint64_t	uGas;
	char		szMemo[1024];
	uint64_t	uMsgAmount;
	char		szMsgDenom[128];
	char		szMsgFrom[128];
	char		szMsgTo[128];
	uint64_t	uSequence;

	char        szData[1024]; 

	HtdfRawTx()
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

	HtdfRawTx(const HtdfRawTx & other)
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

	HtdfRawTx& operator = (const HtdfRawTx &other)
	{
	    if(this == &other) return *this;

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

    //version 2019
	bool ToString( std::string &strOut)
	{
		if (false == this->ParamsCheck(strOut))
		{
			return false;
		}
		strOut.clear();


		std::string  strJson;

		strJson += "{"; //root

		char buf[1024] = { 0 };

		//account_number
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"account_number\":\"%llu\"", this->uAccountNumber);
		strJson += buf;
		strJson += ",";

		//chain_id
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"chain_id\":\"%s\"", this->szChainId);
		strJson += buf;
		strJson += ",";

		//fee
		strJson += "\"fee\":{"; 
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"amount\":[{\"amount\":\"%llu\",\"denom\":\"%s\"}],\"gas\":\"%llu\"", 
						this->uFeeAmount, this->szFeeDenom, this->uGas);
		strJson += buf;
		strJson += "}"; 
		strJson += ",";


		//memo
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"memo\":\"%s\"", this->szMemo);
		strJson += buf;
		strJson += ",";

		//msgs  
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"msgs\":[{\"Amount\":[{\"amount\":\"%llu\",\"denom\":\"%s\"}],\"From\":\"%s\",\"To\":\"%s\"}]", 
					this->uMsgAmount, this->szMsgDenom, this->szMsgFrom, this->szMsgTo);
		strJson += buf;
		strJson += ",";

		//sequence
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"sequence\":\"%llu\"", this->uSequence);
		strJson += buf;


		strJson += "}"; //root 

		strOut = strJson;
		return true;
	}



    //version 2020 
	bool ToString_2020( std::string &strOut)
	{
		if (false == this->ParamsCheck(strOut))
		{
			return false;
		}
		strOut.clear();


		std::string  strJson;

		strJson += "{"; //root

		char buf[1024] = { 0 };

		//account_number
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"account_number\":\"%llu\"", this->uAccountNumber);
		strJson += buf;
		strJson += ",";

		//chain_id
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"chain_id\":\"%s\"", this->szChainId);
		strJson += buf;
		strJson += ",";

        //fee
		strJson += "\"fee\":{";
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"gas_price\":\"%llu\",\"gas_wanted\":\"%llu\"", this->uFeeAmount, this->uGas);
		strJson += buf;
		strJson += "}";
		strJson += ",";


		//memo
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"memo\":\"%s\"", this->szMemo);
		strJson += buf;
		strJson += ",";

        //msgs  
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"msgs\":[{\"Amount\":[{\"amount\":\"%llu\",\"denom\":\"%s\"}],\"Data\":\"%s\",\"From\":\"%s\",\"GasPrice\":%llu,\"GasWanted\":%llu,\"To\":\"%s\"}]",
			this->uMsgAmount, this->szMsgDenom, this->szData ,this->szMsgFrom, this->uFeeAmount, this->uGas,this->szMsgTo);
		strJson += buf;
		strJson += ",";

		//sequence
		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "\"sequence\":\"%llu\"", this->uSequence);
		strJson += buf;


		strJson += "}"; //root

		strOut = strJson;
		return true;
	}



	bool  ParamsCheck( std::string &strErrMsg  )
	{
		if (INTMAX_MAX == this->uAccountNumber || 0 == this->uAccountNumber)
		{
			strErrMsg = boost::str(boost::format("invalid `account_number`: %llu , must between %llu and %llu.") % this->uAccountNumber % 0 % INTMAX_MAX);
			return false;
		}

		if(!(0 == strcmp(STR_MAINCHAIN, this->szChainId) || 0 == strcmp(STR_TESTCHAIN, this->szChainId) ))
		{
			strErrMsg = boost::str( boost::format("invalid `chain_id`: %s, must be '%s'or '%s' . ") % this->szChainId % STR_MAINCHAIN % STR_TESTCHAIN );
			return false;
		}

		if (UINT_MAX_FEE_AMOUNT < this->uFeeAmount || this->uFeeAmount < UINT_MIN_FEE_AMOUNT)
		{
			strErrMsg =  boost::str(boost::format("invalid `fee amount`, must between %d and %d.") % UINT_MIN_FEE_AMOUNT % UINT_MAX_FEE_AMOUNT  );
			return false;
		}

		if (UINT_MAX_GAS_AMOUNT < this->uGas || this->uGas < UINT_MIN_GAS_AMOUNT)
		{
			strErrMsg = boost::str(boost::format("invalid `fee gas` : %llu, must between %llu and %llu.") % this->uGas  % UINT_MIN_GAS_AMOUNT % UINT_MAX_GAS_AMOUNT );
			return false;
		}

      
        if (!(0 == strcmp(STR_SATOSHI, this->szFeeDenom)))
        {
            strErrMsg = boost::str(boost::format("invalid `fee denom` : %s, must be `%s`.") % this->szFeeDenom % STR_SATOSHI );
            return false;
        }

        if (!(0 == strcmp(STR_SATOSHI, this->szMsgDenom) ))
        {
            strErrMsg = boost::str(boost::format("invalid `msgs amount denom` : %s, must be `%s`.") % this->szMsgDenom % STR_SATOSHI );
            return false;
        }

        int nAddrLen = UINT_ADDR_LEN
		if ( nAddrLen != strlen(this->szMsgFrom))
		{
			strErrMsg = boost::str(boost::format("invalid address `msg From`:%s, address length must be %d.") % this->szMsgFrom % nAddrLen );
			return false;
		}

		if ( nAddrLen != strlen(this->szMsgTo))
		{
			strErrMsg = boost::str(boost::format("invalid address `msg To`:%s, address length must be %d.") % this->szMsgTo % nAddrLen );
			return false;
		}

        if( ! boost::starts_with(this->szMsgFrom, std::string(STR_HTDF) + "1"))
        {
			strErrMsg = boost::str(boost::format( "invalid address `From`:%s.") % this->szMsgFrom);
			return false;
		}

        if( ! boost::starts_with(this->szMsgTo, std::string(STR_HTDF) + "1"))
        {
			strErrMsg = boost::str(boost::format( "invalid address `To`:%s.") % this->szMsgTo);
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




struct HtdfBroadcastTx
{
	std::string					strType;		//"auth/StdTx"

	HtdfRawTx						HtdfRawTx;
	std::string					strMsgType;		//"htdfservice/send"
	std::string					strPubKeyType;	//type
	std::string					strPubkeyValue;	//value
	std::string					strSignature;	//signature

	HtdfBroadcastTx()
	{
		strType				= STR_BROADCAST_TYPE; //"auth/StdTx";
		HtdfRawTx				= HtdfRawTx();
		strMsgType			= STR_BROADCAST_MSG_TYPE;// "htdfservice/send";
		strPubKeyType		= STR_BROADCAST_PUB_KEY_TYPE ;//"tendermint/PubKeySecp256k1";
		strPubkeyValue		= "";
		strSignature		= "";
	}

	HtdfBroadcastTx(const HtdfBroadcastTx &other)
	{
		strType				= other.strType;
		HtdfRawTx				= other.HtdfRawTx;
		strMsgType			= other.strMsgType;
		strPubKeyType		= other.strPubKeyType;
		strPubkeyValue		= other.strPubkeyValue;
		strSignature		= other.strSignature;
	}

	HtdfBroadcastTx& operator = (const HtdfBroadcastTx &other)
	{
		strType = other.strType;
		HtdfRawTx = other.HtdfRawTx;
		strMsgType = other.strMsgType;
		strPubKeyType = other.strPubKeyType;
		strPubkeyValue = other.strPubkeyValue;
		strSignature = other.strSignature;

		return *this;
	}

	bool ToString( std::string &strRet )
	{
		std::string strErrMsg;
		if (false == this->ParamsCheck(strErrMsg))
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
		strJson += boost::str( boost::format("\
			\"msg\":[{\
				\"type\":\"%s\",\
				\"value\":{\
					\"From\":\"%s\",\
					\"To\":\"%s\",\
					\"Amount\":[{\
						\"denom\":\"%s\",\
						\"amount\":\"%llu\"\
					}]\
				  }\
			}],") % this->strMsgType % this->HtdfRawTx.szMsgFrom % this->HtdfRawTx.szMsgTo \
				% this->HtdfRawTx.szMsgDenom % this->HtdfRawTx.uMsgAmount );

		//fee
		strJson += boost::str( boost::format( "\
				\"fee\":{\
					\"amount\":[{\
						\"denom\":\"%s\",\
						\"amount\":\"%llu\"\
					}],\
					\"gas\":\"%llu\"\
				}," ) % this->HtdfRawTx.szFeeDenom % this->HtdfRawTx.uFeeAmount % this->HtdfRawTx.uGas);

		//signatures
		strJson += boost::str(boost::format("\
			\"signatures\":[{\
				\"pub_key\":{\
					\"type\":\"%s\",\
					\"value\":\"%s\"\
				},\
				\"signature\":\"%s\"\
			}],") % this->strPubKeyType % this->strPubkeyValue % this->strSignature );


		strJson = boost::algorithm::erase_all_copy(strJson, " ");  
		strJson = boost::algorithm::erase_all_copy(strJson, "\t" ); 
		strJson = boost::algorithm::erase_all_copy(strJson, "\n"); 
		strJson = boost::algorithm::erase_all_copy(strJson, "\r"); 

		//memo  
		strJson += boost::str(boost::format("\"memo\":\"%s\"") % this->HtdfRawTx.szMemo);
		strJson += "}"; //value
		strJson += "}"; //root

		strRet = strJson;
		return true;
	}

	bool ToString_2020( std::string &strRet )
	{
		std::string strErrMsg;
		if (false == this->ParamsCheck(strErrMsg))
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
						\"amount\":\"%llu\"\
					}],\
					\"Data\":\"%s\",\
					\"GasPrice\":\"%llu\",\
					\"GasWanted\":\"%llu\"\
				  }\
			}],") % this->strMsgType % this->HtdfRawTx.szMsgFrom % this->HtdfRawTx.szMsgTo \
			% this->HtdfRawTx.szMsgDenom % this->HtdfRawTx.uMsgAmount % this->HtdfRawTx.szData % this->HtdfRawTx.uFeeAmount % this->HtdfRawTx.uGas);


        //fee
		strJson += boost::str(boost::format("\
				\"fee\":{\
					\"gas_price\":\"%llu\",\
					\"gas_wanted\":\"%llu\"\
				},") %  this->HtdfRawTx.uFeeAmount % this->HtdfRawTx.uGas);


		//signatures
		strJson += boost::str(boost::format("\
			\"signatures\":[{\
				\"pub_key\":{\
					\"type\":\"%s\",\
					\"value\":\"%s\"\
				},\
				\"signature\":\"%s\"\
			}],") % this->strPubKeyType % this->strPubkeyValue % this->strSignature );


		strJson = boost::algorithm::erase_all_copy(strJson, " ");  
		strJson = boost::algorithm::erase_all_copy(strJson, "\t" );
		strJson = boost::algorithm::erase_all_copy(strJson, "\n"); 
		strJson = boost::algorithm::erase_all_copy(strJson, "\r"); 

		strJson += boost::str(boost::format("\"memo\":\"%s\"") % this->HtdfRawTx.szMemo);

		strJson += "}"; //value
		strJson += "}"; //root

		strRet = strJson;
		return true;
	}

	bool ParamsCheck(std::string &strErrMsg)
	{

		if (false == this->HtdfRawTx.ParamsCheck(strErrMsg))
		{
			strErrMsg += "HtdfBroadcastTx::ParamsCheck:";
			return false;
		}

		if ( STR_BROADCAST_MSG_TYPE != this->strMsgType && STR_BROADCAST_MSG_TYPE_HET != this->strMsgType)
		{
			strErrMsg = boost::str(boost::format("invalid `msg type` : '%s', must be '%s' or '%s'.")
				                      % this->strMsgType % STR_BROADCAST_MSG_TYPE % STR_BROADCAST_MSG_TYPE_HET);
			return false;
		}

		if ( STR_BROADCAST_PUB_KEY_TYPE != this->strPubKeyType )
		{
			strErrMsg = boost::str(boost::format("invalid `pub_key type` : '%s', must be '%s'.") 
				                    % this->strPubKeyType% STR_BROADCAST_PUB_KEY_TYPE);
			return false;
		}

		if (strPubkeyValue.empty())
		{
			
			strErrMsg = boost::str(boost::format("invalid `pub_key value` is empty, must be base64(pubkey).") 
									% this->strPubKeyType % STR_BROADCAST_PUB_KEY_TYPE);
			return false;
		}

		string strTmpDecode;
        strTmpDecode = htdf::Base64Decode(strPubkeyValue.data(), strPubkeyValue.size() , false);
		if (UINT_PUB_KEY_LEN != strTmpDecode.size())
		{
            strErrMsg = boost::str(boost::format("invalid `pub_key value` length %d is not %d. After base64 decode, pubkey's length must be %d.")
                                    %strTmpDecode.size() % UINT_PUB_KEY_LEN % UINT_PUB_KEY_LEN);
			return false;
		}

		strTmpDecode.clear();
        strTmpDecode =  htdf::Base64Decode(this->strSignature.data(), this->strSignature.size(), false);
		if (UINT_SIG_RS_LEN != strTmpDecode.size())
		{
			strErrMsg = boost::str(boost::format("invalid `signature` length is not %d. After base64 decode, signature's length must be %d.")
				% UINT_SIG_RS_LEN % UINT_SIG_RS_LEN);
			return false;
		}

		return true;
	}

	bool ToHexStr(std::string &strOut)
	{
		std::string strErrMsg;
		if (false == this->ParamsCheck(strErrMsg))
		{
			strOut = strErrMsg;
			return false;
		}

		std::string strRet;
		std::string strHex;
		if (false == this->ToString(strErrMsg)) 
		{
			strOut = strErrMsg;
			return false;
		}

		this->ToString(strHex);

		strRet = "";
		for (size_t i  = 0; i < strHex.size(); i++)
		{
			strRet += boost::str(boost::format("%02x") % ((int)strHex[i]));
		}
		strOut = strRet;

		return true;
	}

	bool ToHexStr_2020(std::string &strOut)
	{
		std::string strErrMsg;
		if (false == this->ParamsCheck(strErrMsg))
		{
			strOut = strErrMsg;
			return false;
		}

		std::string strRet;
		std::string strHex;
		if (false == this->ToString_2020(strErrMsg)) 
		{
			strOut = strErrMsg;
			return false;
		}

		this->ToString_2020(strHex);

		strRet = "";
		for (size_t i  = 0; i < strHex.size(); i++)
		{
			strRet += boost::str(boost::format("%02x") % ((int)strHex[i]));
		}
		strOut = strRet;

		return true;
	}


}; // struct HtdfBroadcastTx

} //namespace htdf
#endif // _HTDF_H_
