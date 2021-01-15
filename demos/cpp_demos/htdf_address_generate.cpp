#include <iostream>
#include <string>
#include <assert.h>
#include "htdf/htdf.h"
#include "htdf/crypto/strencodings.h"

using namespace std;

void Test()
{
    htdf::CPrivateKey privkey("279bdcd8dccec91f9e079894da33d6888c0f9ef466c0b200921a1bf1ea7d86e8");
    assert(privkey.isValid());
    htdf::CPublickey pubkey = privkey.getPubkey();
    assert("htdf1xwpsq6yqx0zy6grygy7s395e2646wggufqndml" == pubkey.getBech32Address());
}

void GenerateAddress()
{
    htdf::CPrivateKey privkey = htdf::CPrivateKey::newRandomPrivKey();
    assert(privkey.isValid());
    cout << "private key: " << privkey.hexString() << endl;
    htdf::CPublickey pubkey = privkey.getPubkey();
    cout << "public key: " << pubkey.hexString() << endl;
    cout << "address: " << pubkey.getBech32Address() << endl;
}

int main(int argc, char const *argv[])
{
    Test();
    GenerateAddress();
    return 0;
}
