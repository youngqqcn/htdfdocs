#ifndef __CPP_DEMOS_UTILS_H__
#define __CPP_DEMOS_UTILS_H__
#include <iostream>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <time.h>
#include <type_traits>

#include <secp256k1.h>
#include <secp256k1_recovery.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <memory>
#include <random> // for std::random_device
#include "bech32/bech32.h"

using namespace std;

inline secp256k1_context const *GetSecp256k1Ctx()
{
    static std::unique_ptr<secp256k1_context, decltype(&secp256k1_context_destroy)> s_ctx{
        secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY),
        &secp256k1_context_destroy};
    return s_ctx.get();
}

inline std::string Bin2HexStr(const unsigned char *pBin, unsigned int len)
{
    std::string strHex;
    for (unsigned int i = 0; i < len; i++)
    {
        char buf[10] = {0};
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%02x", pBin[i]);
        strHex += buf;
    }
    return strHex;
}

inline std::vector<unsigned char> Bin2ByteArray(const unsigned char *pBin, unsigned int len)
{
    std::vector<unsigned char> vctBytes;

    for (unsigned int i = 0; i < len; i++)
    {
        vctBytes.push_back(pBin[i]);
    }
    return vctBytes;
}

inline std::vector<unsigned char> Bin2ByteArray(const std::string &strTmp, unsigned int len)
{
    std::vector<unsigned char> vctBytes;

    for (unsigned int i = 0; i < strTmp.size(); i++)
    {
        vctBytes.push_back(strTmp[i]);
    }
    return vctBytes;
}

inline void PrintBytesArray(std::vector<unsigned char> &vctBytes)
{
    for (size_t i = 0; i < vctBytes.size(); i++)
    {
        //std::cout << vctBytes[i] << ",";
        printf("%d,", vctBytes[i]);
    }
    std::cout << endl;
}

inline string HexToBin(const string &strHexIn)
{
    if (strHexIn.size() % 2 != 0)
        return "";

    string strHex = (0 == strHexIn.find_first_of("0x")) ? (strHexIn.substr(2)) : (strHexIn);

    string strBin;
    strBin.resize(strHex.size() / 2);
    for (size_t i = 0; i < strBin.size(); i++)
    {
        uint8_t cTemp = 0;
        for (size_t j = 0; j < 2; j++)
        {
            char cCur = strHex[2 * i + j];
            if (cCur >= '0' && cCur <= '9')
            {
                cTemp = (cTemp << 4) + (cCur - '0');
            }
            else if (cCur >= 'a' && cCur <= 'f')
            {
                cTemp = (cTemp << 4) + (cCur - 'a' + 10);
            }
            else if (cCur >= 'A' && cCur <= 'F')
            {
                cTemp = (cTemp << 4) + (cCur - 'A' + 10);
            }
            else
            {
                return "";
            }
        }
        strBin[i] = cTemp;
    }

    return strBin;
}

inline std::string Base64Encode(const char *input, int length, bool with_new_line)
{
    BIO *bmem = NULL;
    BIO *b64 = NULL;
    BUF_MEM *bptr = NULL;

    b64 = BIO_new(BIO_f_base64());
    if (!with_new_line)
    {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    char *buff = (char *)malloc(bptr->length + 1);
    memcpy(buff, bptr->data, bptr->length);
    buff[bptr->length] = 0;
    BIO_free_all(b64);

    std::string strRet(buff);
    free(buff);
    buff = NULL;
    return strRet;
}

inline std::string Base64Decode(const char *input, int length, bool with_new_line)
{
    BIO *b64 = NULL;
    BIO *bmem = NULL;
    char *buffer = (char *)malloc(length);
    memset(buffer, 0, length);

    b64 = BIO_new(BIO_f_base64());
    if (!with_new_line)
    {
        BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    }
    bmem = BIO_new_mem_buf(input, length);
    bmem = BIO_push(b64, bmem);
    BIO_read(bmem, buffer, length);

    BIO_free_all(bmem);

    std::string strRet;
    strRet.append(buffer, length);

    free(buffer);
    buffer = NULL;
    return strRet;
}

inline bool Bech32AddrToHexStrAddr(const std::string &strBech32AddrIn, std::string &strHexStrAddrOut) noexcept(false)
{
    std::string strErrMsg = "";

    if (strBech32AddrIn.length() < 40)
    {
        strErrMsg = std::string(" bech32addr's length less 40 or bech32addr is not htdf address.  ");
        throw runtime_error(strErrMsg);
    }

    std::pair<std::string, std::vector<uint8_t>> retpair = bech32::decode(strBech32AddrIn);
    if (std::string("htdf") != retpair.first || retpair.second.size() < 30)
    {
        strErrMsg = std::string(" hrp is not `htdf` or decode data's size less than 30 ");
        throw runtime_error(strErrMsg);
    }

    std::vector<uint8_t> outdata;
    if (!bech32::convertbits<5, 8, false>(outdata, retpair.second))
    {
        strErrMsg = std::string("bech32::convertbits failed  ");
        throw runtime_error(strErrMsg);
    }

    std::string cstrHexRet = Bin2HexStr(outdata.data(), outdata.size());
    strHexStrAddrOut = cstrHexRet;

    return true;
}


static void GetRandom32Bytes(unsigned char *rand32)
{
    typedef std::random_device::result_type rtype ;
    std::random_device rd;
    static const int bsize = 32;
    static_assert( (bsize % sizeof(rtype)) == 0);
    for(int i = 0; i < bsize/sizeof(rtype); i++)
    {
        *((rtype *)(rand32 + i*sizeof(rtype))) = rd();
    }
}

#endif // __CPP_DEMOS_UTILS_H__