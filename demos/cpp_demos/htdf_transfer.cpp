/*
author: yqq 
date: 2021-01-15 
descriptions: htdf normal transaction demo written in cpp
*/

#include <iostream>
#include <thread>
#include <chrono>
#include "htdf/htdf.h"
using namespace std;

void TransferDemo()
{
    string chainid = "testchain";
    string from = "htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml";
    string to = "htdf1jrh6kxrcr0fd8gfgdwna8yyr9tkt99ggmz9ja2";
    string privKey = "279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8";
    // uint64_t amountSatoshi = 1234567; // 
    htdf::Amount amount = htdf::Amount::fromStringHtdf("0.99999999");

    htdf::CRpc rpc("192.168.0.171", chainid, 1317);
    auto fromAct = rpc.GetAccountInfo(from);
    htdf::CTxBuilder txBuilder(
        chainid,
        from,
        to,
        amount.GetSatoshi(),
        fromAct.sequence,
        fromAct.account_number
    );
    txBuilder.Build();
    string signedTx = txBuilder.Sign(privKey);
    auto rsp = rpc.Broadcast(signedTx);
    if(!rsp.raw_log.empty())
    {
        cerr << rsp.raw_log << endl;
        return;
    }
    cout << "tx_hash:" << rsp.tx_hash << endl;

    // waiting for tx 
    int waitSecs = 10;
    cout << "waiting  " << waitSecs << " seconds " << flush;
    for(int i = 0; i < waitSecs; i++)
    {
        this_thread::sleep_for( chrono::seconds(1));
        cout << "." << flush;
    }
    cout << endl;

    // get transaction
    auto tx = rpc.GetTransaction( rsp.tx_hash);
    cout << "tx data is :" << endl << tx << endl;
}

int main(int argc, char const *argv[])
{
    TransferDemo();
    return 0;
}
