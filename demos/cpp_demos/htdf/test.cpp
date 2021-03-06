/**
 * author: yqq
 * date: 2021-01-12
 * descriptions: test cases of htdf
 */

#include <ctime>
#include <random> // c++11 random
#include "crypto/sha256.h"
#include "base64/base64.h"
#include "crypto/hash.h"
#include "bech32/bech32.h"
#include "crypto/strencodings.h"
#include "utils.h"
#include "htdf.h"

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"   /* Red */
#define GREEN "\033[32m" /* Green */

#define LOG_PASSED cout << GREEN << __FUNCTION__ << "\t: PASSED" << RESET << endl
#define LOG_FAILED cout << RED << __FUNCTION__ << ": FAILED,"

void TestBitcoinSHA256()
{
    CSHA256 sha256;
    string strTest = "";
    string strHash;
    strHash.resize(32);

    // 空字符串的hash
    // unsigned char uszRight[32] = {227, 176, 196, 66, 152, 252, 28, 20, 154, 251, 244, 200, 153, 111, 185, 36, 39, 174, 65, 228, 100, 155, 147, 76, 164, 149, 153, 27, 120, 82, 184, 85};
    // string strExpectedHash =  Bin2HexStr(uszRight, sizeof(uszRight));
    string strExpectedHash = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855";

    sha256.Write((const unsigned char *)strTest.data(), strTest.size())
        .Finalize((unsigned char *)strHash.data());
    string strHexHash = htdf::Bin2HexStr((const unsigned char *)strHash.data(), strHash.size());

    if (strHexHash == strExpectedHash)
    {
        LOG_PASSED;
    }
    else
    {
        // cout << __FUNCTION__ << ": FAILED,  " << strHexHash << " != " << strExpectedHash << endl;
        LOG_FAILED << strHexHash << " != " << strExpectedHash << endl;
    }
}

void TestBitcoinRipemd160()
{
    CRIPEMD160 ripemd160;
    string strTest = "";
    string strHash;
    strHash.resize(CRIPEMD160::OUTPUT_SIZE);
    ripemd160.Write((const unsigned char *)strTest.data(), strTest.size())
        .Finalize((unsigned char *)strHash.data());
    string strHexHash = htdf::Bin2HexStr((const unsigned char *)strHash.data(), strHash.size());
    string strExpectedHash = "9c1185a5c5e9fc54612808977ee8f548b2258d31";

    if (strHexHash == strExpectedHash)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << strHexHash << " != " << strExpectedHash << endl;
    }
}

void TestHtdfRawTx()
{
    htdf::CRawTx rtx;
    rtx.uAccountNumber = 12;
    strcpy(rtx.szChainId, STR_MAINCHAIN);
    strcpy(rtx.szFeeDenom, STR_SATOSHI);
    strcpy(rtx.szMemo, "");
    strcpy(rtx.szMsgDenom, STR_SATOSHI);
    strcpy(rtx.szMsgFrom, "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2");
    strcpy(rtx.szMsgTo, "htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2");
    rtx.uMsgAmount = 66;
    rtx.uGas = 200000;
    rtx.uFeeAmount = 100;
    rtx.uSequence = 5;

    std::string strJson;
    if (false == rtx.toString(strJson))
    {
        LOG_FAILED << strJson << std::endl;
        return;
    }

    std::string strJsonRight = "{\"account_number\":\"12\",\"chain_id\":\"mainchain\",\"fee\":{\"gas_price\":\"100\",\"gas_wanted\":\"200000\"},\"memo\":\"\",\"msgs\":[{\"Amount\":[{\"amount\":\"66\",\"denom\":\"satoshi\"}],\"Data\":\"\",\"From\":\"htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2\",\"GasPrice\":100,\"GasWanted\":200000,\"To\":\"htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2\"}],\"sequence\":\"5\"}";
    if (strJsonRight == strJson)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << std::endl;
    }
}

void TestBroadcastTx()
{
    htdf::CRawTx rtx;
    rtx.uAccountNumber = 12;
    strcpy(rtx.szChainId, STR_MAINCHAIN);
    strcpy(rtx.szFeeDenom, STR_SATOSHI);
    strcpy(rtx.szMemo, "");
    strcpy(rtx.szMsgDenom, STR_SATOSHI);
    strcpy(rtx.szMsgFrom, "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2");
    strcpy(rtx.szMsgTo, "htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2");
    rtx.uMsgAmount = 66;
    rtx.uGas = 200000;
    rtx.uFeeAmount = 20;
    rtx.uSequence = 5;

    htdf::CBroadcastTx csBTx;
    csBTx.rtx = rtx;
    csBTx.strType = STR_BROADCAST_TYPE;
    csBTx.strMsgType = STR_BROADCAST_MSG_TYPE;
    csBTx.strPubKeyType = STR_BROADCAST_PUB_KEY_TYPE;
    csBTx.strPubkeyValue = "A9wn8etkGR2CeeEzkHqGq17gxFtWajDa/aqmPCUVt9cN";
    csBTx.strSignature = "HnBXVHouML/jR45/Ln5GOu6JQdPekB8J/x7H5h6VX357g6iqkuFawFwKItTugVg2Y2QUMD/DYDjJHm1znao9Kw==";

    std::string strTmp;
    if (false == csBTx.toString(strTmp))
    {
        LOG_FAILED << strTmp << std::endl;
        return;
    }

    // std::cout << "json str:" << strTmp << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;

    std::string strHex;
    if (false == csBTx.toHexStr(strHex))
    {
        LOG_FAILED << strHex << std::endl;
        return;
    }

    // std::cout << "broadcasttx hex str: " << strHex << std::endl;
    std::string strHexRight = "7b2274797065223a22617574682f5374645478222c2276616c7565223a7b226d7367223a5b7b2274797065223a2268746466736572766963652f73656e64222c2276616c7565223a7b2246726f6d223a2268746466316a7268366b787263723066643867666764776e613879797239746b74393967676d7a396a6132222c22546f223a2268746466316d3570687376677277707864736d61683563716b76643666667a39787a72633365306a6b7232222c22416d6f756e74223a5b7b2264656e6f6d223a227361746f736869222c22616d6f756e74223a223636227d5d2c2244617461223a22222c224761735072696365223a223230222c2247617357616e746564223a22323030303030227d7d5d2c22666565223a7b226761735f7072696365223a223230222c226761735f77616e746564223a22323030303030227d2c227369676e617475726573223a5b7b227075625f6b6579223a7b2274797065223a2274656e6465726d696e742f5075624b6579536563703235366b31222c2276616c7565223a224139776e3865746b475232436565457a6b4871477131376778467457616a44612f61716d504355567439634e227d2c227369676e6174757265223a22486e425856486f754d4c2f6a5234352f4c6e35474f75364a516450656b42384a2f7837483568365658333537673669716b7546617746774b4974547567566732593251554d442f4459446a4a486d317a6e616f394b773d3d227d5d2c226d656d6f223a22227d7d";
    if (strHexRight == strHex)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << strHex << " != " << strHexRight << std::endl;
    }
}

void TestBuildAndSingTx()
{
    htdf::CRawTx rtx;
    rtx.uAccountNumber = 11;
    strcpy(rtx.szChainId, STR_TESTCHAIN);
    strcpy(rtx.szFeeDenom, "satoshi");
    strcpy(rtx.szMemo, "yqq");
    strcpy(rtx.szMsgDenom, "satoshi");
    strcpy(rtx.szMsgFrom, "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2");
    strcpy(rtx.szMsgTo, "htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2");
    rtx.uMsgAmount = 12345678;
    rtx.uGas = 200000;
    rtx.uFeeAmount = 100;
    rtx.uSequence = 8;

    htdf::CBroadcastTx csBTx;
    csBTx.rtx = rtx;
    csBTx.strType = STR_BROADCAST_TYPE;
    csBTx.strMsgType = STR_BROADCAST_MSG_TYPE;
    csBTx.strPubKeyType = STR_BROADCAST_PUB_KEY_TYPE;
    csBTx.strPubkeyValue = "AynNwicLJpg9SSyojXQ04vGcd2DYe4663ynQFrk5Mp6d";

    std::string strOut;
    if (false == rtx.toString(strOut))
    {
        LOG_FAILED << strOut << std::endl;
        return;
    }

    // std::cout << "json:  " << std::endl << strOut << std::endl;
    std::string strJsonRight = R"({"account_number":"11","chain_id":"testchain","fee":{"gas_price":"100","gas_wanted":"200000"},"memo":"yqq","msgs":[{"Amount":[{"amount":"12345678","denom":"satoshi"}],"Data":"","From":"htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2","GasPrice":100,"GasWanted":200000,"To":"htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2"}],"sequence":"8"})";
    if (strJsonRight != strOut)
    {
        LOG_FAILED << strOut << " != " << strJsonRight << std::endl;
        return;
    }

    unsigned char uszShaData[CSHA256::OUTPUT_SIZE] = {0};
    memset(uszShaData, 0, sizeof(uszShaData));
    // SHA256((unsigned char *)strOut.data(), strOut.size(), uszShaData);
    CSHA256 sh256;
    sh256.Write((unsigned char *)strOut.data(), strOut.size());
    sh256.Finalize(uszShaData);

    std::string strSha256 = htdf::Bin2HexStr(uszShaData, sizeof(uszShaData));
    // std::cout << "sha256 output: " << std::endl
    //           << strSha256 << std::endl
    //           << std::endl
    //           << std::endl;

    string strEpectedSha256 = "d9971bb7d8c24fd45ac86efe0f560511cac5cab8bc779de8a9841918d458dc12";
    if (strSha256 != strEpectedSha256)
    {
        LOG_FAILED << strSha256 << "!= " << strEpectedSha256 << std::endl;
        return;
    }

    std::string strPrivKey = htdf::HexToBin(std::string("485de9a2ee4ed834272389617da915da9176bd5281ec5d261256e15be0c375f2"));
    unsigned char uszSigOut[64] = {0};
    memset(uszSigOut, 0, sizeof(uszSigOut));
    unsigned int uSigOutLen = 0;
    char szMsgBuf[1024] = {0};
    memset(szMsgBuf, 0, sizeof(szMsgBuf));
    int iRet = htdf::CPrivateKey::sign(uszShaData, sizeof(uszShaData), (unsigned char *)strPrivKey.data(), strPrivKey.size(), uszSigOut, sizeof(uszSigOut), &uSigOutLen, szMsgBuf);
    if (0 != iRet)
    {
        LOG_FAILED << iRet << std::endl;
        return;
    }
    std::string strTmpSig = htdf::Bin2HexStr(uszSigOut, uSigOutLen);

    string strExpectedSig = "9f670ff17ef97d3d9dbf35b7bbbcd9b6c21a475c134eb4f8ae2abb10ddcb60a606b19edcd59ca8f388730c450b9fc88d57b2a0566bc71630cca3093b4aff3bb3";
    if (strTmpSig == strExpectedSig)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << std::endl;
    }
}

void TestPrivKeyToPubKeyCompress()
{
    string strPrivKey = "f4c3b67fb023405e615994f12bcfad63255d90b47c179d7bb00f612000dd5a2b";
    string strPubKey;
    string strRightPubKey = "02af7557b38a3f2313e49ca2109859736786832052bc8bca2cf5ae81d3f5e21e83";

    htdf::CPrivateKey privkey(strPrivKey);
    if(strRightPubKey == privkey.getPubkey().hexString() )
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << endl;
    }
}

void TestCpp11Random()
{
    // random
    unsigned char entroy32[32];
    htdf::GetRandom32Bytes(entroy32);
    string strRand = htdf::Bin2HexStr(entroy32, sizeof(entroy32));
    // cout << "rand = " << strRand << endl;
    LOG_PASSED;
}

void TestMakeNewKey()
{
    auto privkey = htdf::CPrivateKey::newRandomPrivKey();
    if(privkey.isValid())
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << "newRandomPrivKey error" << endl;
    }
    
}

void TestPubkToAddress()
{
    // private key: 5c9afe978e62a4f9911d0d8314f401c679f6abd2392f16e31256d62604975e15
    string strPubk = "02a5314d3192bbe6dd9dd86465ea1e623cb4af01be99616e5c4838be69aeab8b54";

    htdf::CPublickey pubKey(strPubk);
    string strAddr = pubKey.getBech32Address();
    string strAddress = "htdf106pthht5503w3u96se9utzxl0aq302ya32m6ky";
    if (strAddr == strAddress)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << strAddr << " != " << strAddress << endl;
    }
}

void TestHex2Bin()
{
    string hex = "02a5314d3192bbe6dd9dd86465ea1e623cb4af01be99616e5c4838be69aeab8b54";
    string bin = htdf::HexToBin(hex);
    unsigned char szExpected[] = {
        2, 165, 49, 77, 49, 146, 187, 230, 221, 157, 216, 100,
        101, 234, 30, 98, 60, 180, 175, 1,
        190, 153, 97, 110, 92, 72, 56, 190, 105, 174, 171, 139, 84};

    if (0 == memcmp(szExpected, bin.data(), 32))
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << endl;
    }
}

void TestBitcoinHex2Bin()
{
    string hex = "02a5314d3192bbe6dd9dd86465ea1e623cb4af01be99616e5c4838be69aeab8b54";
    // string bin = HexToBin(hex);
    auto bin = ParseHex(hex);
    unsigned char szExpected[] = {
        2, 165, 49, 77, 49, 146, 187, 230, 221, 157, 216, 100,
        101, 234, 30, 98, 60, 180, 175, 1,
        190, 153, 97, 110, 92, 72, 56, 190, 105, 174, 171, 139, 84};

    if (0 == memcmp(szExpected, bin.data(), 32))
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << endl;
    }
}

void TestBitcoinBase64()
{
    char psz[] = "helloworld";
    string b64 = EncodeBase64(psz);
    string strExpected = "aGVsbG93b3JsZA==";
    if (b64 == strExpected)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << b64 << " != " << strExpected << endl;
    }
}

void TestBech32()
{
    bech32::data d;
    d.resize(20);
    string strExpected = "htdf1qqqqqqqqqqqqqqqqqqqqldl6yp";
    string strBech32 = bech32::encode("htdf", d);
    if (strBech32 == strExpected)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << strBech32 << " != " << strExpected << endl;
    }
}

void TestRpcGetAccountInfo()
{
    htdf::CRpc rpc("192.168.0.171", "testchain");
    string address = "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml";
    htdf::CActInfo act = rpc.GetAccountInfo(address);
    if (act.active)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << address << " not found" << endl;
    }
}

void TestRpcGetTransaction()
{
    htdf::CRpc rpc("192.168.0.171", "testchain");
    string txhash = "B9A2AE2408556CA2F8BAAFB4E058F53BA62838037CBE5C31BFA984356F98E174";
    auto tx = rpc.GetTransaction(txhash);
    // cout << tx << endl;
    if (tx.success)
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << txhash << " not found" << endl;
    }
}

void TestRpcBroadcast()
{
    // htdf::CRpc rpc("192.168.0.171", "testchain");
    // string txdata = "7b2274797065223a22617574682f5374645478222c2276616c7565223a7b226d7367223a5b7b2274797065223a2268746466736572766963652f73656e64222c2276616c7565223a7b2246726f6d223a226874646631787770737136797178307a79366772796779377333393565323634367767677566716e646d6c222c22546f223a2268746466316a7268366b787263723066643867666764776e613879797239746b74393967676d7a396a6132222c22416d6f756e74223a5b7b2264656e6f6d223a227361746f736869222c22616d6f756e74223a223130303030303030227d5d2c2244617461223a22222c224761735072696365223a22313030222c2247617357616e746564223a223330303030227d7d5d2c22666565223a7b226761735f7072696365223a22313030222c226761735f77616e746564223a223330303030227d2c227369676e617475726573223a5b7b227075625f6b6579223a7b2274797065223a2274656e6465726d696e742f5075624b6579536563703235366b31222c2276616c7565223a22412f7448756e637755475a484c784b64657671617364532b444363573866627430537638735270657a715779227d2c227369676e6174757265223a227471535762595a433473654d544d6f6974706c7262556955565735584469313351524e657668474548505638786c64594f5152595a6b57614556567a7847354e465a7a5a71487767643758316436614b4877626939773d3d227d5d2c226d656d6f223a22227d7d";
    // htdf::CBroadcastRsp rsp = rpc.Broadcast(txdata);
    // cout << rsp.tx_hash << endl;
    // if (!rsp.tx_hash.empty())
    // {
    //     LOG_PASSED;
    // }
    // else
    // {
    //     LOG_FAILED << " broadcast failed." << endl;
    // }
}

void TestTxBuilder()
{
    htdf::CTxBuilder txBuilder(
        "testchain",
        "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml",
        "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2",
        10000000,
        10021, //seq
        6      //acctnumber
    );

    txBuilder.Build();
    string signedTx = txBuilder.Sign("279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8");
    // cout << signedTx << endl;
    LOG_PASSED;
}

void TestGetBlock()
{
    htdf::CRpc rpc("192.168.0.171", "testchain");
    htdf::CBlock blk = rpc.GetBlock(90202);
    // cout << blk.hash << endl;
    // cout << blk.txs.size() << endl;

    // for( auto &tx : blk.txs)
    // {
    //     // cout << tx.ToString() << endl;
    //     cout << tx << endl;
    // }
    if (!blk.empty())
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << " GET block failed." << endl;
    }
}

void TestGetLatestBlock()
{
    htdf::CRpc rpc("192.168.0.171", "testchain");
    htdf::CBlock blk = rpc.GetLatestBlock();
    // cout << blk.hash << endl;
    // cout << blk.txs.size() << endl;
    // cout << blk.blocktime << endl;
    if (!blk.empty())
    {
        LOG_PASSED;
    }
    else
    {
        LOG_FAILED << " GET latest block failed." << endl;
    }
}

int main(int argc, char const *argv[])
{
    TestBuildAndSingTx();
    TestHtdfRawTx();
    TestBroadcastTx();
    TestPrivKeyToPubKeyCompress();
    TestCpp11Random();
    TestMakeNewKey();
    TestPubkToAddress();
    TestBitcoinSHA256();
    TestBitcoinRipemd160();
    TestBech32();
    TestHex2Bin();
    TestBitcoinHex2Bin();
    TestBitcoinBase64();
    TestRpcGetAccountInfo();
    TestRpcGetTransaction();
    TestRpcBroadcast();
    TestTxBuilder();
    TestGetBlock();
    TestGetLatestBlock();
}
