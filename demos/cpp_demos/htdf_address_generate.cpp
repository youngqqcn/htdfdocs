#include <iostream>
#include <string>

#include "htdf/htdf.h"
#include "htdf/crypto/strencodings.h"

#include <assert.h>
using namespace std;

void GenerateAddress()
{
    string privKey; 
    privKey.resize(32);
    htdf::MakeNewKey( (unsigned char *)privKey.data() );
    string  strPrivKey = Bin2HexStr((unsigned char *)privKey.data(), privKey.size());
    cout << "private key: " << strPrivKey << endl;

    string pubKey;
    int ret = htdf::PrivateKeyToCompressPubKey(strPrivKey, pubKey);
    assert(0 == ret);
    cout << "public key: " << pubKey << endl;
    string strAddress = htdf::PubkToAddress(pubKey);
    cout << "address key: " << strAddress << endl;
}


int main(int argc, char const *argv[])
{
    GenerateAddress();
    return 0;
}
