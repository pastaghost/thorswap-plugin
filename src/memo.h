#ifndef _MEMO_H_
#define _MEMO_H_

#include <stdint.h>

#define HAS_ASSET(x)               ((x)->fields & 1UL)
#define HAS_DEST_ADDR(x)           ((x)->fields & 1UL << 1)
#define HAS_LIMIT(x)               ((x)->fields & 1UL << 2)
#define HAS_AFFILIATE(x)           ((x)->fields & 1UL << 3)
#define HAS_FEE(x)                 ((x)->fields & 1UL << 4)
#define HAS_DEX_AGGREGATOR_ADDR(x) ((x)->fields & 1UL << 5)
#define HAS_FINAL_TOKEN_ADDR(x)    ((x)->fields & 1UL << 6)
#define HAS_MIN_AMOUNT_OUT(x)      ((x)->fields & 1UL << 7)

/** __builtin_popcount() is a compiler intrinsic available in both GCC and Clang.
 * Note that this may not work on other compilers. */
#define NUM_MEMO_FIELDS(x) (__builtin_popcount((x)->fields))

enum errors {
    ERROR_NONE = 0,
    ERROR_INVALID_PARAMETERS,
    ERROR_INVALID_MEMO_PREFIX,
    ERROR_MISSING_ASSET,
    ERROR_MISSING_DEST_ADDR,
    ERROR_MISSING_AFFILIATE,
    ERROR_MISSING_DEX_AGGREGATOR_ADDR,
    ERROR_MISSING_FINAL_TOKEN_ADDR,
    ERROR_MISSING_MIN_AMOUNT_OUT,
    ERROR_PARSING_INCOMPLETE
};

typedef struct memo_t {
    char asset[61];
    char dest_addr[41];
    char limit[10];      // Trade limit in 1e8 precision
    char affiliate[44];  // THORChain address length = 43, max THORName length = 30
    char fee[5];         // Fee, from 0-1000 basis points
    char dex_aggregator_addr[41];
    char final_token_addr[41];
    char min_amount_out[30];  // TODO: What is the maximum number of characters here?
    uint8_t fields;
} memo_t;

extern memo_t parsed_memo;  // Parsed memo data
extern char memo_str[257];

int parse_memo(memo_t *memo, char *str);
int validate_memo(memo_t *memo);

#endif