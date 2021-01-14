﻿/************************************************************************
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

#include <mutex>

#include "http/httplib.h" // for http
#include <memory>

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

    // struct Client;
    // typedef Client;
    // using Client=httplib::Client;
    struct CActInfo;
    struct CBroadcastRsp;
    class CRpc 
    {
    public:
        enum ApiName{
            ACCOUNT_INFO,
            BROADCAST,
            TXS,
        };
        

        CRpc() = delete;
        CRpc(CRpc&) = delete;
        CRpc(CRpc&&) = delete;
        explicit CRpc(string strNodeHost, string chainid, int port = 1317);
        CActInfo GetAccountInfo(string strAddress);
        string GetTransaction(string strTxHash);
        CBroadcastRsp Broadcast(string strSignedTx);
    
    private:
        string GetURL(ApiName apiName, string arg1="", string arg2="", string arg3="");

    private:
        int _m_port;
        string _m_host;
        mutex _m_mtx;
        std::shared_ptr<httplib::Client> _m_pHttp;
    };

    struct CActInfo
    {
        string address;
        uint64_t sequence;
        uint32_t account_number;
        double balance;
        bool active = false;
    };

    struct CBroadcastRsp
    {
        string  tx_hash;
        // uint64_t  height;
        string  raw_log;
    };

    class CTxBuilder
    {
    public:
        CTxBuilder(
            string chainid,
            string from,
            string to,
            uint64_t amountSatoshi,
            uint64_t sequence,
            uint32_t accountNumber,
            string memo = "",
            string data = "",
            uint32_t gasWanted = 30000,
            uint32_t gasPrice = 100
            );

        string Build();
        string Sign(string privateKey);

    private:
        // string 
        string _m_unsignedTx;
        CRawTx _m_rtx;
        htdf::CBroadcastTx _m_csBTx;

    private:
        string _m_chainid;
        string _m_from;
        string _m_to;
        uint64_t _m_sequence;
        uint32_t _m_accountNumber;
        uint32_t _m_gasPrice;
        uint32_t _m_gasWanted;
        string _m_data;
        string _m_memo;
        uint64_t _m_amountSatoshi;
    };

} // namespace htdf
#endif // __CPP_DEMOS_HTDF_H_
