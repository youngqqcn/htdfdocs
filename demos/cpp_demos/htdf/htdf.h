/************************************************************************
author: yqq
date: 2020-01-12
decriptions:  htdf 
************************************************************************/

#ifndef __CPP_DEMOS_HTDF_H_
#define __CPP_DEMOS_HTDF_H_

#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <memory>

#include <secp256k1.h>    // secp256k1
#include "http/httplib.h" // for http
#include "constant.h"
#include "utils.h"

using namespace std;

#define NAMESPACE_BEGIN(name) \
    namespace name            \
    {
#define NAMESPACE_END }



NAMESPACE_BEGIN(htdf)
class CPublickey
{
public:
    static const int SIZE = 33;
    CPublickey() = delete;
    explicit CPublickey(const string &hexPubkey);
    explicit CPublickey(unsigned char *pbuf, int len = CPublickey::SIZE);
    string hexString() const;
    string get() const;
    // bool verifySig(string sig);
    string getBech32Address() const;
    string getHexAddress() const;
    static bool checkPubKey(const string &pubkey);
    static string convertBech32ToHex(const string &bech32Addr);
    static string convertHexToBech32(const string &hexAddr, const string &hrp = "htdf");

private:
    string _m_pubkey;
    bool _m_fValid;
};

class CPrivateKey
{
public:
    static const int SIZE = 32;

    CPrivateKey() = delete;
    explicit CPrivateKey(string hexPrivkey);
    explicit CPrivateKey(unsigned char *pbuf, int len = CPrivateKey::SIZE);

    CPublickey getPubkey() const;
    string hexString() const;
    string get() const;

    static CPrivateKey newRandomPrivKey();

    static int sign(
        unsigned char *pszIn,
        unsigned int uInLen,
        unsigned char *pszPrivKey,
        unsigned int uPrivKeyLen,
        unsigned char *pszOut,
        unsigned int uOutBufLen,
        unsigned int *puOutDataLen,
        char *pszErrMsg);

    static bool checkPrivkey(const unsigned char *vch);
    static bool checkPrivkey(const string &privkey);

    bool isValid() const;

private:
    string _m_privkey;
    bool _m_fValid;
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

struct CTx;
struct CActInfo;
struct CBroadcastRsp;
struct CLatestBlock;
struct CBlock;
class CRpc
{
public:
    enum ApiName
    {
        ACCOUNT_INFO, // GET, get an account info
        BROADCAST,    // POST, broadcast a signed tx
        TXS,          // GET, get transaction details by txhash
        LATEST_BLOCK, // GET, get the latest block info
        BLOCK_DETAILS // GET, get specially block details
    };

    CRpc() = delete;
    CRpc(CRpc &) = delete;
    CRpc(CRpc &&) = delete;
    explicit CRpc(string strNodeHost, string chainid, int port = 1317);
    CActInfo GetAccountInfo(string strAddress);
    CTx GetTransaction(string strTxHash);
    CBroadcastRsp Broadcast(string strSignedTx);
    CBlock GetBlock(uint32_t height);
    CBlock GetLatestBlock();

private:
    string GetURL(ApiName apiName, string arg1 = "", string arg2 = "", string arg3 = "");

private:
    int _m_port;
    string _m_host;
    mutex _m_mtx;
    std::shared_ptr<httplib::Client> _m_pHttp;
};

struct CActInfo
{
    string address;
    uint64_t sequence = 0;
    uint32_t account_number = 0;
    double balance = 0;
    bool active = false;
};

struct CBroadcastRsp
{
    string tx_hash;
    string raw_log;
};

struct Amount
{
    explicit Amount();
    explicit Amount(double htdf);
    explicit Amount(uint64_t satoshi);
    static Amount fromStringHtdf(string htdf);
    static Amount fromStringSatoshi(string satoshi);
    double GetHtdf() const;
    uint64_t GetSatoshi() const;

private:
    double _m_amount;
    uint64_t _m_amountSatoshi;
};

struct CTx
{
    string msgType; // msg type of transaction
    string log;     // log, if transaction failed, log is errmsg
    string hash;
    string from;
    string to;
    Amount amount;
    string memo;
    string data;
    uint32_t gasWanted = 0;
    uint32_t gasPrice = 0;
    uint32_t gasUsed = 0;
    uint32_t height = 0;
    string timstamp; // string format: "2021-01-14T03:25:47Z"

    bool success = false;
    string ToString() const;
    bool empty() const { return hash.empty(); }
};

struct CBlock
{
    uint32_t height = 0;
    int num_txs = 0;
    string hash;
    // int64_t blocktime;
    string blocktime; // UTC+0
    vector<CTx> txs;

    bool empty() const { return hash.empty(); };
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
        uint32_t gasPrice = 100);

    string Build();
    string Sign(const string &privateKey);

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

NAMESPACE_END // htdf


ostream &operator<<(ostream &os, const htdf::CTx &tx);

#endif // __CPP_DEMOS_HTDF_H_
