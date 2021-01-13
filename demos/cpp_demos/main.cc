/*
author: yqq
date: 2021-01-12
descriptions: implement demo for c++
*/

#include <stdio.h>
#include "test.h"

int main(int argc, char const *argv[])
{
    TestBuildAndSingTx();
    TestHtdfRawTx();
    TestBroadcastTx();
    TestPrivKeyToPubKeyCompress();
    TestGetOsRand();
    TestCpp11Random();
    TestMakeNewKey();
    TestPubkToAddress();
    TestBitcoinSHA256();
    TestBitcoinRipemd160();
    // TestBase64();
    TestBech32();
    TestHex2Bin();
    TestBitcoinHex2Bin();
    TestBitcoinBase64();
}
