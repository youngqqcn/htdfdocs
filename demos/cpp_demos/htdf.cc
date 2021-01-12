/************************************************************************
author: yqq
date: 2019-05-08  14:25
descriptions: htdf transaction signature 
************************************************************************/

#include "htdf.h"
#include <secp256k1.h>
#include <secp256k1_recovery.h>



int htdf::sign(
    unsigned char *pszIn,
    unsigned int uInLen,
    unsigned char *pszPrivKey,
    unsigned int uPrivKeyLen,
    unsigned char *pszOut,
    unsigned int uOutBufLen,
    unsigned int *puOutDataLen,
    char *pszErrMsg)
{
    if (NULL == pszErrMsg)
    {
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszIn)
    {
        strcpy(pszErrMsg, "pszIn is null.");
        return htdf::ARGS_ERROR;
    }

    if (0 == uInLen)
    {
        strcpy(pszErrMsg, "uInLen is 0.");
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszPrivKey)
    {
        strcpy(pszErrMsg, "pszPrivKey is null.");
        return htdf::ARGS_ERROR;
    }

    if (UINT_PRIV_KEY_LEN != uPrivKeyLen)
    {
        sprintf(pszErrMsg, "priv-key len is not %d bytes.", UINT_PRIV_KEY_LEN);
        return htdf::ARGS_ERROR;
    }

    if (NULL == pszOut)
    {
        strcpy(pszErrMsg, "pszOut is null.");
        return htdf::ARGS_ERROR;
    }

    if (uOutBufLen < UINT_SIG_RS_LEN)
    {
        sprintf(pszErrMsg, "uOutBufLen less than %d. Must more than %d.", UINT_SIG_RS_LEN, UINT_SIG_RS_LEN);
        return htdf::ARGS_ERROR;
    }

    if (NULL == puOutDataLen)
    {
        strcpy(pszErrMsg, "puOutDataLen is null");
        return htdf::ARGS_ERROR;
    }

    auto *ctx = getCtx();

    secp256k1_ecdsa_recoverable_signature rawSig;
    memset(&rawSig.data, 0, 65);
    if (!secp256k1_ecdsa_sign_recoverable(ctx, &rawSig, pszIn, pszPrivKey, nullptr, nullptr))
    {
        strcpy(pszErrMsg, "secp256k1_ecdsa_sign_recoverable  failed.");
        return htdf::ECCSIGN_STEP1_ERROR;
    }

    int iRecid = 0;
    unsigned char uszSigRSData[UINT_SIG_RS_LEN] = {0};
    memset(uszSigRSData, 0, sizeof(uszSigRSData));
    secp256k1_ecdsa_recoverable_signature_serialize_compact(ctx, uszSigRSData, &iRecid, &rawSig);

    memcpy(pszOut, uszSigRSData, UINT_SIG_RS_LEN);
    *puOutDataLen = UINT_SIG_RS_LEN;

    return htdf::NO_ERROR;
}



//输入: 十六进制字符串形式的私钥
//输出: 十六进制字符串形式的公钥
int htdf::PrivateKeyToCompressPubKey(const std::string &_strPrivKey, std::string &strPubKey)
{
    std::string strPrivKey = HexToBin(_strPrivKey);

    secp256k1_pubkey pubkey;
    memset(pubkey.data, 0, sizeof(pubkey.data));

    auto *ctx = getCtx();

    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, (unsigned char *)strPrivKey.data()))
    {
        return 1;
    }

    unsigned char output[1024] = {0};
    memset(output, 0, sizeof(output));
    size_t outputlen = 33;
    secp256k1_ec_pubkey_serialize(ctx, output, &outputlen, &pubkey, SECP256K1_EC_COMPRESSED);

    if (33 != outputlen)
    {
        return 1;
    }

    strPubKey = Bin2HexStr(output, outputlen);
    return 0;
}
