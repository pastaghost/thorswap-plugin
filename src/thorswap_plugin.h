#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include "memo.h"
#include <string.h>

#define NUM_SELECTORS 1

#define PLUGIN_NAME "THORSwap"

typedef enum { DEPOSIT_WITH_EXPIRY = 0 } selector_t;

typedef enum {
    VAULT = 0,    // Vault address
    ASSET,        // Assset contract address
    AMOUNT,       // Swap amount, in base decimal precision for token with address above
    MEMO_OFFSET,  // Memory offset to start of memo data (# bytes)
    MEMO_LENGTH,  // Number of characters in memo
    MEMO,         // THORChain memo string
    EXPIRATION,   // Expiration timestamp
    NONE
} parameter;

extern const uint32_t THORCHAIN_ROUTER_V4_SELECTORS[NUM_SELECTORS];

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct context_t {
    // 133/160 bytes used. TODO: Verify this.
    // memo_t *memo;  // Parsed memo data
    // char *memo_str;
    uint16_t memo_offset;
    uint16_t memo_length;
    uint16_t memo_bytes_remaining;  // Remaining number of bytes to parse
    uint8_t vault[ADDRESS_LENGTH];
    uint8_t asset[ADDRESS_LENGTH];
    uint8_t amount[INT256_LENGTH];
    uint8_t expiration[INT256_LENGTH];
    uint8_t token_found;
    uint8_t decimals;
    uint8_t ticker[MAX_TICKER_LEN];

    // For parsing data.
    uint8_t next_param;  // Set to be the next param we expect to parse.
    // uint16_t offset;     // Offset at which the array or struct starts .
    // bool go_to_offset;   // If set, will force the parsing to iterate through parameters until
    //                      // `offset` is reached.

    // For both parsing and display.
    selector_t selectorIndex;
} context_t;

// Piece of code that will check that the above structure is not bigger than 5 * 32. Do not remove
// this check.
_Static_assert(sizeof(context_t) <= 5 * 32, "Structure of parameters too big.");

void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_init_contract(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);