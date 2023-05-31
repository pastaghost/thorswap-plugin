#include <stdio.h>
#include <stdint.h>
#include "utils.h"
#include "thorswap_plugin.h"
#include "memo.h"

// void *_sbrk(int incr) {
//     return (void *) -1;
// }

// size_t hex_to_ascii(const char *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len) {
//     PRINTF("IN HEX TO ASCII: %s\n", ascii_buf);
//     size_t i = 0, r = 0;
//     for (i = 0; i < hex_buf_len; i++) {
//         r += sprintf((char *) ascii_buf + 2 * i, "%02x", hex_buf[i]);
//     }
//     PRINTF("CONVERTED FROM HEX TO STRING: %s\n", ascii_buf);
//     return r;
// }

void debug_print_context(context_t *context, cx_sha3_t *sha_context) {
    char addr_len_buffer[66] = {'\0'};
    char addr_len_2_buffer[66] = {'\0'};
    char int256_len_buffer[INT256_LENGTH] = {'\0'};

    PRINTF("---- CURRENT CONTEXT STATE ----\n");

    getEthAddressStringFromBinary(
        context->vault,
        addr_len_buffer,  // +2 here because we've already prefixed with '0x'.
        sha_context,
        1);
    strncpy(addr_len_2_buffer, addr_len_buffer, ADDRESS_LENGTH);
    PRINTF("Vault:\t\t %s\n", addr_len_2_buffer);
    getEthAddressStringFromBinary(
        context->asset,
        addr_len_buffer,  // +2 here because we've already prefixed with '0x'.
        sha_context,
        1);
    strncpy(addr_len_2_buffer, addr_len_buffer, ADDRESS_LENGTH);
    PRINTF("Asset:\t\t %s\n", addr_len_2_buffer);
    uint256_to_decimal(context->amount, INT256_LENGTH, int256_len_buffer, INT256_LENGTH);
    PRINTF("Amount:\t\t %s\n", int256_len_buffer);
    PRINTF("Memo Offset:\t\t %d\n", context->memo_offset);
    PRINTF("Memo Length:\t\t %d\n", context->memo_length);
    PRINTF("Memo Bytes Remaining:\t\t %d\n", context->memo_bytes_remaining);
    PRINTF("Memo Pointer:\t\t %d\n",
           &parsed_memo);  //  Replace with utility function to parse memo
    uint256_to_decimal(context->expiration, INT256_LENGTH, int256_len_buffer, INT256_LENGTH);
    PRINTF("Expiration:\t\t %s\n", int256_len_buffer);
    PRINTF("Token Found:\t\t %s\n", context->token_found ? "true" : "false");
    PRINTF("Decimals:\t\t %d\n", context->decimals);
    strncpy(int256_len_buffer, (char *) context->ticker, MAX_TICKER_LEN);
    PRINTF("Ticker:\t\t %s\n", int256_len_buffer);
    PRINTF("Next Param:\t\t %d\n", context->next_param);
    // PRINTF("Offset:\t\t %d\n", context->offset);
    // PRINTF("Go To Offset:\t\t %s\n", context->go_to_offset ? "true" : "false");
    PRINTF("Selector Index:\t\t %d\n", context->selectorIndex);
}

void debug_memo(memo_t *memo) {
    PRINTF("---- CURRENT MEMO STATE ----\n");

    PRINTF("Asset:\t\t %s\n", memo->asset);
    PRINTF("Dest Addr:\t\t %s\n", memo->dest_addr);
    PRINTF("Limit:\t\t %d\n", memo->limit);
    PRINTF("Affiliate:\t\t %s\n", memo->affiliate);
    PRINTF("Fee:\t\t %d\n", memo->fee);
    PRINTF("Dex Addr:\t\t %d\n", memo->dex_aggregator_addr);
    PRINTF("Final Token Addr:\t\t %s\n", memo->final_token_addr);
    PRINTF("Min Amt Out:\t\t %d\n", memo->min_amount_out);
}