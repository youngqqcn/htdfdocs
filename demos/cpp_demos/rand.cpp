#include "rand.h"

#ifdef WIN32
#include <compat.h> // for Windows API
#include <wincrypt.h>
#endif
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

/** Fallback: get 32 bytes of system entropy from /dev/urandom. The most
 * compatible way to get cryptographic randomness on UNIX-ish platforms.
 */
static void GetDevURandom(unsigned char *ent32)
{
    int f = open("/dev/urandom", O_RDONLY);
    if (f == -1) {
        return;
    }
    int have = 0;
    do {
        ssize_t n = read(f, ent32 + have, NUM_OS_RANDOM_BYTES - have);
        if (n <= 0 || n + have > NUM_OS_RANDOM_BYTES) {
            close(f);
            return;
        }
        have += n;
    } while (have < NUM_OS_RANDOM_BYTES);
    close(f);
}

/** Get 32 bytes of system entropy. */
int GetOSRand(unsigned char *ent32)
{
#if defined(WIN32)
    HCRYPTPROV hProvider;
    int ret = CryptAcquireContextW(&hProvider, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
    if (!ret) {
        return 1;
    }
    ret = CryptGenRandom(hProvider, NUM_OS_RANDOM_BYTES, ent32);
    if (!ret) {
        return 2;
    }
    CryptReleaseContext(hProvider, 0);
    return 0;
#else
    /* Fall back to /dev/urandom if there is no specific method implemented to
     * get system entropy for this OS.
     */
    GetDevURandom(ent32);
    return 0;
#endif
}