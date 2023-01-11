#include <stdio.h>
#include <string.h>
#include "memo.h"

memo_t parsed_memo = {0};

int parse_memo(memo_t *memo, char *str) {
    char delim[] = ":";  // Field delimiter
    char *token = NULL;
    char cpy_str[250] = {'\0'};
    char *cpy_str_addr = (char *) &cpy_str;  // strsep() requires char** as the first argument

    if (str == NULL || memo == NULL || str[0] == '\0') {  // Sanity checks
        return ERROR_INVALID_PARAMETERS;
    }

    /* Clear out any junk that might be in struct */
    memset(memo, 0, sizeof(memo_t));

    /* Copy str into new buffer because strsep() modifies the input buffer */
    strncpy(cpy_str, str, (sizeof(cpy_str) / sizeof(char)));

    uint8_t *fields[] = {
        NULL,  // Leave me set to NULL since we're throwing away the message prefix (s,=,etc.)
        (uint8_t *) &(memo->asset),
        (uint8_t *) &(memo->dest_addr),
        (uint8_t *) &(memo->limit),
        (uint8_t *) &(memo->affiliate),
        (uint8_t *) &(memo->fee),
        (uint8_t *) &(memo->dex_aggregator_addr),
        (uint8_t *) &(memo->final_token_addr),
        (uint8_t *) &(memo->min_amount_out)};

    token = strsep(&cpy_str_addr, delim);

    // TODO: Is "swap" a valid message prefix?
    if (strcmp(token, "=") && strcmp(token, "s") &&
        strcmp(token, "SWAP")) {  // Check for invalid message prefix
        return ERROR_INVALID_MEMO_PREFIX;
    }

    /* Parse string data into struct */
    for (uint8_t i = 1; i < (sizeof(fields) / sizeof(uint8_t *)); i++) {
        token = strsep(&cpy_str_addr, delim);
        if (*token != '\0') {
            strncpy((char *) fields[i], token, sizeof(fields[i]) / sizeof(char));
            memo->fields |= 1UL << (i - 1);
        }
    }

    return validate_memo(memo);
}

int validate_memo(memo_t *memo) {
    /* memo->asset, memo->dest_addr are always required */
    if (!HAS_ASSET(memo)) return ERROR_MISSING_ASSET;
    if (!HAS_DEST_ADDR(memo)) return ERROR_MISSING_DEST_ADDR;

    /* memo->affiliate is required if fee is set*/
    if (!HAS_AFFILIATE(memo) && HAS_FEE(memo)) return ERROR_MISSING_AFFILIATE;

    /* memo->final_token_addr, memo->min_amount_out required if memo->dex_aggregator_addr is set*/
    if (HAS_DEX_AGGREGATOR_ADDR(memo)) {
        if (!HAS_FINAL_TOKEN_ADDR(memo)) {
            return ERROR_MISSING_FINAL_TOKEN_ADDR;
        }
        if (!HAS_MIN_AMOUNT_OUT(memo)) {
            return ERROR_MISSING_MIN_AMOUNT_OUT;
        }
    }
    /* memo->dex_aggregator_addr required if memo->final_token_addr, memo->min_amount_out are set */
    if (!HAS_DEX_AGGREGATOR_ADDR(memo) &&
        (HAS_FINAL_TOKEN_ADDR(memo) || HAS_MIN_AMOUNT_OUT(memo))) {
        return ERROR_MISSING_DEX_AGGREGATOR_ADDR;
    }
    return 0;
}