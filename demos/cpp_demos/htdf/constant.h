#ifndef __CPP_DEMOS_CONSTANT_H__
#define __CPP_DEMOS_CONSTANT_H__

namespace htdf 
{

#define UINT_MAX_FEE_AMOUNT (2000000000)
#define UINT_MIN_FEE_AMOUNT (10)
#define UINT_MAX_GAS_AMOUNT (200000000)
#define UINT_MIN_GAS_AMOUNT (30000)
#define UINT_ADDR_LEN (43)
#define UINT_PRIV_KEY_LEN (32)
#define UINT_PUB_KEY_LEN (33)
#define UINT_SIG_RS_LEN (64)
#define UINT_TXID_CHAR_LEN (64)

#define STR_MAINCHAIN "mainchain"
#define STR_TESTCHAIN "testchain"

#define STR_HTDF "htdf"
#define STR_SATOSHI "satoshi"

#define STR_BROADCAST_TYPE "auth/StdTx"
#define STR_BROADCAST_MSG_TYPE "htdfservice/send"
#define STR_BROADCAST_MSG_TYPE_HET "hetservice/send"
#define STR_BROADCAST_PUB_KEY_TYPE "tendermint/PubKeySecp256k1"

}// htdf

#endif //__CPP_DEMOS_CONSTANT_H__