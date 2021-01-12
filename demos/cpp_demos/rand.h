#ifndef __BITCOIN_RANDOM_H__
#define __BITCOIN_RANDOM_H__

#include <chrono> // For std::chrono::microseconds
#include <cstdint>
#include <limits>

static const int NUM_OS_RANDOM_BYTES = 32;
int GetOSRand(unsigned char* ent32);

#endif // __BITCOIN_RANDOM_H__
