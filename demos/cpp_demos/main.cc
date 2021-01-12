/*
author: yqq
date: 2021-01-12
descriptions: implement demo for c++
*/

#include <stdio.h>
#include "test.h"

int main(int argc, char const *argv[])
{
    TestSha256();
    TestBuildAndSingTx();
    TestHtdfRawTx();
    TestBroadcastTx();
    TestPrivKeyToPubKeyCompress();
}
