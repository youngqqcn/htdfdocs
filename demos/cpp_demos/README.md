# HTDF signature wrapper (C++)

### dependecy

- secp256k1 : https://github.com/bitcoin-core/secp256k1
- openssl : https://github.com/openssl/openssl


### usage


```cpp
struct Test
{
    std::string strPrivKey;
    htdf::_htdfRawTx _htdfRawTx ;
    htdf::HtdfBroadcastTx _htdfBcastTx ;
    
    //....
};

bool HtdfTransactionSignatureDemo(Test *pParam, /*..TODO..*/ )
{
    //.......other code .....

    std::string strErrMsg ;
    std::string  cstrRawTxJsonStr ;

    if(false == pParam->_htdfRawTx.ToString_2020(cstrRawTxJsonStr))
    {
        strErrMsg = "internal error: _htdfRawTx.ToString_2020(strRawTxJsonStr)  error";
        throw runtime_error( strErrMsg );
    } 

    unsigned char uszSha256Data[256/8] = {0}; memset(uszSha256Data, 0, sizeof(uszSha256Data));
    SHA256( (unsigned char *)cstrRawTxJsonStr.data(), cstrRawTxJsonStr.size(),  uszSha256Data);


    std::string  cstrPrivKey = pParam->strPrivKey.toStdString();
    std::string  cstrPrivKeyBin = htdf::HexToBin(cstrPrivKey);  //private key

    unsigned char uszSigRS[64] = {0}; 
    memset(uszSigRS, 0, sizeof(uszSigRS));
    unsigned int  uSigOutLen = 0;
    char szErrMsg [1024] = {0}; memset(szErrMsg, 0, sizeof(szErrMsg));

    iRet = htdf::ECDSA_Sign(
                uszSha256Data,                             
                sizeof(uszSha256Data),                     
                (unsigned char *)(cstrPrivKeyBin.data()),  
                cstrPrivKeyBin.size(),                     
                uszSigRS,                                   
                sizeof(uszSigRS),                           
                &uSigOutLen,                                
                szErrMsg );                                 

    if(htdf::NO_ERROR != iRet)
    {
        strErrMsg = std::string("htdf::Sign() error: ") + std::string(szErrMsg);
        throw runtime_error(strErrMsg);
    }

    pParam->_htdfBcastTx._htdfRawTx       = pParam->_htdfRawTx;
    pParam->_htdfBcastTx.strType          = STR_BROADCAST_TYPE;
    pParam->_htdfBcastTx.strMsgType       = STR_BROADCAST_MSG_TYPE;
    pParam->_htdfBcastTx.strPubKeyType    = STR_BROADCAST_PUB_KEY_TYPE;
    pParam->_htdfBcastTx.strPubkeyValue   = pParam->GetPubKey(true);         
    pParam->_htdfBcastTx.strSignature     = htdf::Base64Encode((const char *)uszSigRS, sizeof(uszSigRS), false);  

    std::string cstrErrMsgTmp;
    if(false == pParam->_htdfBcastTx.ParamsCheck(cstrErrMsgTmp) )
    {
        throw runtime_error(cstrErrMsgTmp);
    }

    std::string cstrHexData;
    if(false == pParam->_htdfBcastTx.ToHexStr_2020(cstrHexData))
    {
        strErrMsg = "ToHexStr_2020 error";
        throw runtime_error(strErrMsg);
    }

    pParam->strHexData = cstrHexData;

    // ..... other code

    return true;
}

```
