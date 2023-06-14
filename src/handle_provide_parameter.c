#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "memo.h"
#include "thorswap_plugin.h"
#include "utils.h"

static void handle_vault(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING VAULT PARAMETER ****\n");
    memset(context->vault, 0, sizeof(context->vault));
    memcpy(context->vault,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->vault));
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    copy_address(context->vault, msg->parameter, sizeof(context->vault));
    PRINTF("Parsed vault address: %.*H\n", ADDRESS_LENGTH, context->vault);
}

static void handle_asset(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING ASSET PARAMETER ****\n");
    memset(context->asset, 0, sizeof(context->asset));
    memcpy(context->asset,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->asset));
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    copy_address(context->asset, msg->parameter, sizeof(context->asset));
    PRINTF("Parsed asset address: %.*H\n", ADDRESS_LENGTH, context->asset);
}

static void handle_amount(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING AMOUNT PARAMETER ****\n");
    memset(context->amount, 0, sizeof(context->amount));
    copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    PRINTF("Parsed amount: %.*H\n", PARAMETER_LENGTH, context->amount);
}

static void handle_memo_offset(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING MEMO_OFFSET PARAMETER ****\n");
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    context->memo_offset = U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->memo_offset));
    PRINTF("Parsed memo offset: %d\n", context->memo_offset);
}

static void handle_expiration(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING EXPIRATION PARAMETER ****\n");
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    copy_parameter(context->expiration, msg->parameter, sizeof(context->expiration));
    PRINTF("Parsed expiration: %.*H\n", PARAMETER_LENGTH, context->asset);
}

static void handle_memo_length(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** HANDLING MEMO_LENGTH PARAMETER ****\n");
    // debug_print_context(context, msg->pluginSharedRW->sha3);
    context->memo_length = U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->memo_length));
    context->memo_bytes_remaining = context->memo_length;
    PRINTF("Parsed memo length: %d\n", context->memo_length);
    PRINTF("DEBUG: **** DONE HANDLING MEMO_LENGTH PARAMETER ****\n");
}

static int handle_memo(ethPluginProvideParameter_t *msg, context_t *context) {
    static char memo_str[257] = {0};
    PRINTF("DEBUG: **** HANDLING MEMO PARAMETER ****\n");
    // debug_print_context(context, msg->pluginSharedRW->sha3);

    uint8_t memo_str_offset = context->memo_length - context->memo_bytes_remaining;
    uint8_t n_bytes_to_convert = MIN(PARAMETER_LENGTH, context->memo_bytes_remaining);
    PRINTF("MEMO_STR_OFFSET: %d\n", memo_str_offset);
    PRINTF("N_BYTES_TO_CONVERT: %d\n", n_bytes_to_convert);
    // memcpy((void *) &(memo_str[memo_str_offset]), (void *) msg->parameter, n_bytes_to_convert);
    // memcpy((void *) &(memo_str), (void *) msg->parameter, n_bytes_to_convert);

    // strlcpy(&memo_str[memo_str_offset], msg->parameter, n_bytes_to_convert);
    // PRINTF("CURRENT MEMO STRING: %.*H\n", 32, memo_str);
    PRINTF("CURRENT MEMO STRING: %s\n", memo_str);

    // context->memo_bytes_remaining -= n_bytes_to_convert;
    // PRINTF("MEMO_BYTES_REMAINING: %d\n", context->memo_bytes_remaining);
    if (context->memo_bytes_remaining == 0) {
        // PRINTF("****** DECODING MEMO ****** %s\n", memo_str);
        // PRINTF("****** MEMO LENGTH ****** %d\n", context->memo_length);
        // PRINTF("MEMO: %.*H\n", context->memo_length, memo_str);
        // PRINTF("CONVERTED FROM HEX: %s\n", &memo_str);
        /* Append null terminator to memo and parse */
        // memo_str[MIN((context->memo_length) - 1, THORCHAIN_MEMO_MAX_LEN - 1)] = '\0';
        // PRINTF("****** APPENDED NULL TERMINATOR ******\n");
        // int error = parse_memo(&parsed_memo, memo_str);
        int error = 0;
        // PRINTF("****** PARSED MEMO ******\n");
        // PRINTF("PARSED MEMO DATA: %.*H\n", sizeof(memo_t), &parsed_memo);
        // debug_memo(&parsed_memo);
        switch (error) {
            case ERROR_NONE:
                break;
            case ERROR_INVALID_PARAMETERS:
                PRINTF("Function parse_memo() called with invalid parameters\n");
                break;
            case ERROR_INVALID_MEMO_PREFIX:
                PRINTF("Invalid memo prefix.\n");
                break;
            case ERROR_MISSING_ASSET:
                PRINTF("Memo is missing asset field\n");
                break;
            case ERROR_MISSING_DEST_ADDR:
                PRINTF("Memo is missing dest_addr field\n");
                break;
            case ERROR_MISSING_AFFILIATE:
                PRINTF("Memo is missing affiliate field\n");
                break;
            case ERROR_MISSING_DEX_AGGREGATOR_ADDR:
                PRINTF("Memo is missing dex_aggregator_addr field\n");
                break;
            case ERROR_MISSING_FINAL_TOKEN_ADDR:
                PRINTF("Memo is missing final_token_addr field\n");
                break;
            case ERROR_MISSING_MIN_AMOUNT_OUT:
                PRINTF("Memo is missing min_amount_out field\n");
                break;
            default:
                PRINTF("parse_memo() returned unexpected value: %d", error);
                break;
        }
        return error;
    }
    return ERROR_PARSING_INCOMPLETE;
}

static void handle_deposit_with_expiry(ethPluginProvideParameter_t *msg, context_t *context) {
    PRINTF("DEBUG: **** IN HANDLE_PROVIDE_PARAMETER ****\n");
    // if (context->go_to_offset) {
    //     if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
    //         return;
    //     }
    //     context->go_to_offset = false;
    // }
    switch (context->next_param) {
        case VAULT:
            handle_vault(msg, context);
            context->next_param = ASSET;
            break;

        case ASSET:
            handle_asset(msg, context);
            context->next_param = AMOUNT;
            break;

        case AMOUNT:
            handle_amount(msg, context);
            context->next_param = MEMO_OFFSET;
            break;

        case MEMO_OFFSET:
            handle_memo_offset(msg, context);
            context->next_param = EXPIRATION;
            break;

        case EXPIRATION:
            handle_expiration(msg, context);
            context->next_param = MEMO_LENGTH;
            break;

        case MEMO_LENGTH:
            handle_memo_length(msg, context);
            context->next_param = MEMO;
            break;

        case MEMO:
            if (context->memo_length > THORCHAIN_MEMO_MAX_LEN) {
                PRINTF("Invalid memo length. %d > 250 bytes\n", context->memo_length);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }

            int error = handle_memo(msg, context);

            if (error == ERROR_PARSING_INCOMPLETE) {
                PRINTF("$$ SETTING CONTEXT->NEXT_PARAM: ERROR_PARSING_INCOMPLETE $$\n");
                context->next_param = (uint8_t) MEMO;  // Data still remains to be parsed
            } else if (error > 0) {
                PRINTF("$$ SETTING ETH_PLUGIN_RESULT_ERROR $$\n");
                msg->result = ETH_PLUGIN_RESULT_ERROR;
            } else {
                PRINTF("$$ SETTING CONTEXT->NEXT_PARAM: NONE - MEMO PARSED SUCCESSFULLY $$\n");
                context->next_param = (uint8_t) NONE;  // Memo parsed successfully
            }
            break;

        case NONE:
            PRINTF("DEBUG: ****EXECUTING NONE CASE IN HANDLE PROVIDE PARAMETER SWITCH");
            break;

        default:
            PRINTF("Param not supported: %d\n", context->next_param);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case DEPOSIT_WITH_EXPIRY:
            handle_deposit_with_expiry(msg, context);
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}
