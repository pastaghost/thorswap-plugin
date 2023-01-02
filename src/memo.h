#ifndef _MEMO_H_
#define _MEMO_H_

#include <stdint.h>
#include "constants.h"

typedef struct memo_t {
    uint8_t asset[61];
    uint8_t dest_addr[41];
    uint8_t limit[10];      // Trade limit in 1e8 precision
    uint8_t affiliate[44];  // THORChain address length = 43, max THORName length = 30
    uint8_t fee[5];         // Fee, from 0-1000 basis points
} memo_t;

#endif