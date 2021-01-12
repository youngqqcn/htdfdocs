#include <openssl/sha.h> 
#include <secp256k1.h>
#include <secp256k1_recovery.h>

#include "utils.h"
#include "htdf.h"
#include "test.h"

using namespace std;
// using namespace boost::property_tree;

//简单json字符获取值
// void TestStrToJsonObj()
// {
//     std::string strTmpJson = "{\"name\":\"yqq\", \"age\":99, \"isTest\":true}";
//     std::stringstream ss(strTmpJson);
//     boost::property_tree::ptree root;
//     try
//     {
//         boost::property_tree::read_json(ss, root);
//     }
//     catch (const std::exception &e)
//     {
//         std::cout << "read_json  error:" << e.what() << std::endl;
//     }

//     for (auto it = root.begin(); it != root.end(); it++)
//     {
//         std::string ssTmp;
//         std::cout << it->first << ":" << root.get<std::string>(it->first) << std::endl;
//     }
// }

void TestSha256()
{
    //1.将json转为bytes形式
    std::string strTest = "";
    unsigned char *pJsonBuf = new unsigned char[strTest.size()];
    size_t uBufSize = strTest.size();
    memset(pJsonBuf, 0, uBufSize);
    memcpy(pJsonBuf, strTest.c_str(), uBufSize);

    //2.进行sha256哈希
    const size_t uSha256Size = 32;
    unsigned char *pSha256Out = new unsigned char[uSha256Size]; //32 byte 即可
    memset(pSha256Out, 0, uSha256Size);
    SHA256(pJsonBuf, uBufSize, pSha256Out);

    //使用的sha256 对空字符串进行sha256 输出是
    //[227 176 196 66 152 252 28 20 154 251 244 200 153 111 185 36 39 174 65 228 100 155 147 76 164 149 153 27 120 82 184 85]
    unsigned char uszRight[32] = {227, 176, 196, 66, 152, 252, 28, 20, 154, 251, 244, 200, 153, 111, 185, 36, 39, 174, 65, 228, 100, 155, 147, 76, 164, 149, 153, 27, 120, 82, 184, 85};
    if (0 == memcmp(uszRight, pSha256Out, 32))
    {
        cout << "TestSha256: PASSED\n"
             << endl;
    }
    else
    {
        cout << "TestSha256: FAILED\\n"
             << endl;
    }

    delete[] pSha256Out;
    pSha256Out = NULL;
    delete[] pJsonBuf;
    pJsonBuf = NULL;
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
        std::cout << "错误信息:" << strJson << std::endl;
        return;
    }

    std::string strJsonRight = "{\"account_number\":\"12\",\"chain_id\":\"mainchain\",\"fee\":{\"gas_price\":\"100\",\"gas_wanted\":\"200000\"},\"memo\":\"\",\"msgs\":[{\"Amount\":[{\"amount\":\"66\",\"denom\":\"satoshi\"}],\"Data\":\"\",\"From\":\"htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2\",\"GasPrice\":100,\"GasWanted\":200000,\"To\":\"htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2\"}],\"sequence\":\"5\"}";

    std::cout << "rawtx string: " << strJson << std::endl;
    std::cout << std::endl;
    if (strJsonRight == strJson)
    {
        std::cout << "json 对比成功,  测试 rtx 成功!" << std::endl;
    }
    else
    {
        std::cout << "json 对比失败,  测试 rtx 失败!" << std::endl;
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
        std::cout << "错误信息:" << strTmp << std::endl;
        return;
    }

    std::cout << "json str:" << strTmp << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    std::string strHex;
    if (false == csBTx.toHexStr(strHex))
    {
        std::cout << "错误信息:" << strHex << std::endl;
        return;
    }

    std::cout << "broadcasttx hex str: " << strHex << std::endl;

    std::string strHexRight = "7b2274797065223a22617574682f5374645478222c2276616c7565223a7b226d7367223a5b7b2274797065223a2268746466736572766963652f73656e64222c2276616c7565223a7b2246726f6d223a2268746466316a7268366b787263723066643867666764776e613879797239746b74393967676d7a396a6132222c22546f223a2268746466316d3570687376677277707864736d61683563716b76643666667a39787a72633365306a6b7232222c22416d6f756e74223a5b7b2264656e6f6d223a227361746f736869222c22616d6f756e74223a223636227d5d2c2244617461223a22222c224761735072696365223a223230222c2247617357616e746564223a22323030303030227d7d5d2c22666565223a7b226761735f7072696365223a223230222c226761735f77616e746564223a22323030303030227d2c227369676e617475726573223a5b7b227075625f6b6579223a7b2274797065223a2274656e6465726d696e742f5075624b6579536563703235366b31222c2276616c7565223a224139776e3865746b475232436565457a6b4871477131376778467457616a44612f61716d504355567439634e227d2c227369676e6174757265223a22486e425856486f754d4c2f6a5234352f4c6e35474f75364a516450656b42384a2f7837483568365658333537673669716b7546617746774b4974547567566732593251554d442f4459446a4a486d317a6e616f394b773d3d227d5d2c226d656d6f223a22227d7d";
    if (strHexRight == strHex)
    {
        std::cout << "对比成功!  CsBroadcastTx 测试成功  " << std::endl;
    }
    else
    {
        std::cout << "对比失败!  CsBroadcastTx 测试失败" << std::endl;
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
        std::cout << "错误:" << strOut << std::endl;
        return;
    }
    std::cout << "json:  " << std::endl << strOut << std::endl;
    std::string strJsonRight = R"({"account_number":"11","chain_id":"testchain","fee":{"gas_price":"100","gas_wanted":"200000"},"memo":"yqq","msgs":[{"Amount":[{"amount":"12345678","denom":"satoshi"}],"Data":"","From":"htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2","GasPrice":100,"GasWanted":200000,"To":"htdf1m5phsvgrwpxdsmah5cqkvd6ffz9xzrc3e0jkr2"}],"sequence":"8"})";
    if (strJsonRight == strOut)
    {
        std::cout << "json 对比成功 !" << std::endl;
    }
    else
    {
        std::cout << "json 对比失败 !" << std::endl;
    }

    unsigned char uszShaData[256 / 8] = {0};
    memset(uszShaData, 0, sizeof(uszShaData));
    SHA256((unsigned char *)strOut.data(), strOut.size(), uszShaData);

    std::string strSha256 = Bin2HexStr(uszShaData, sizeof(uszShaData));
    std::cout << "sha256 output: " << std::endl
              << strSha256 << std::endl
              << std::endl
              << std::endl;

    if (strSha256 == "d9971bb7d8c24fd45ac86efe0f560511cac5cab8bc779de8a9841918d458dc12")
    {
        std::cout << "SHA256 结果对比成功!" << std::endl
                  << std::endl;
    }
    else
    {
        std::cout << "SHA256 结果对比失败!!" << std::endl
                  << std::endl;
    }

    std::string strPrivKey = HexToBin(std::string("485de9a2ee4ed834272389617da915da9176bd5281ec5d261256e15be0c375f2"));

    unsigned char uszSigOut[64] = {0};
    memset(uszSigOut, 0, sizeof(uszSigOut));
    unsigned int uSigOutLen = 0;
    char szMsgBuf[1024] = {0};
    memset(szMsgBuf, 0, sizeof(szMsgBuf));
    int iRet = htdf::sign(uszShaData, sizeof(uszShaData), (unsigned char *)strPrivKey.data(), strPrivKey.size(), uszSigOut, sizeof(uszSigOut), &uSigOutLen, szMsgBuf);
    if (htdf::NO_ERROR != iRet)
    {
        std::cout << "签名错误:" << szMsgBuf << std::endl
                  << std::endl;
        return;
    }
    std::string strTmpSig = Bin2HexStr(uszSigOut, uSigOutLen);

    std::cout << "签名结果:" << std::endl
              << strTmpSig << std::endl
              << std::endl;

    if (strTmpSig == "9f670ff17ef97d3d9dbf35b7bbbcd9b6c21a475c134eb4f8ae2abb10ddcb60a606b19edcd59ca8f388730c450b9fc88d57b2a0566bc71630cca3093b4aff3bb3")
    {
        std::cout << "对比签名结果成功!" << std::endl;

        std::cout << "测试签名 成功!!" << std::endl;
    }
    else
    {
        std::cout << "对比签名结果失败!" << std::endl
                  << std::endl;
    }
}


void TestPrivKeyToPubKeyCompress()
{
    string strPrivKey = "f4c3b67fb023405e615994f12bcfad63255d90b47c179d7bb00f612000dd5a2b";
    string strPubKey;
    string strRightPubKey = "02af7557b38a3f2313e49ca2109859736786832052bc8bca2cf5ae81d3f5e21e83";
    if( 0 == htdf::PrivateKeyToCompressPubKey(strPrivKey, strPubKey) )
    {
        cout << "TestPrivKeyToPubKeyCompress:PASSED" << endl;
    }
    else
    {
        cout << "TestPrivKeyToPubKeyCompress:FAILED" << endl;
    }
}