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

#include "constant.h"
#include "utils.h"

#include <secp256k1.h>
using namespace std;

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

    void MakeNewKey(unsigned char *key32);

    int PrivateKeyToCompressPubKey(const string &strPrivKey, string &strPubKey);

    string PubkToAddress(const string &strPubk);

    enum ErrorCode
    {
        NO_ERROR = 0,
        ARGS_ERROR = 1001,
        ECCSIGN_STEP1_ERROR = 1005,
        ECCSIGN_STEP2_ERROR = 1006
    };

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

        CRawTx();
        CRawTx(const CRawTx &other);
        CRawTx &operator=(const CRawTx &other);
        bool toString(string &strOut);
        bool checkParams(string &strErrMsg);
    };

    struct CBroadcastTx
    {
        CRawTx rtx;
        string strType;        //"auth/StdTx"
        string strMsgType;     //"htdfservice/send"
        string strPubKeyType;  //type
        string strPubkeyValue; //value
        string strSignature;   //signature

        CBroadcastTx();
        CBroadcastTx(const CBroadcastTx &other);
        CBroadcastTx &operator=(const CBroadcastTx &other);
        bool toString(string &strRet);
        bool checkParams(string &strErrMsg);
        bool toHexStr(string &strOut);
    };
} // namespace htdf
#endif // __CPP_DEMOS_HTDF_H_
