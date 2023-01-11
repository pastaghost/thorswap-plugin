#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "memo.h"
#include "thorchain_router_plugin.h"
#include "utils.h"

static void handle_deposit_with_expiry(ethPluginProvideParameter_t *msg, context_t *context) {
    static char memo_str[THORCHAIN_MEMO_MAX_LEN] = {'\0'};

    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case VAULT:
            copy_address(context->vault, msg->parameter, sizeof(context->vault));
            context->next_param = ASSET;
            break;
        case ASSET:
            copy_address(context->asset, msg->parameter, sizeof(context->asset));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = MEMO_OFFSET;
            break;
        case MEMO_OFFSET:
            context->memo_offset =
                U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->memo_offset));
            context->next_param = EXPIRATION;
            break;
        case EXPIRATION:
            copy_parameter(context->expiration, msg->parameter, sizeof(context->expiration));
            context->next_param = MEMO_LENGTH;
            break;
        case MEMO_LENGTH:
            context->memo_length =
                U2BE(msg->parameter, PARAMETER_LENGTH - sizeof(context->memo_length));
            context->memo_bytes_remaining = context->memo_length;
            context->next_param = MEMO;
            break;
        case MEMO:
            if (context->memo_length > THORCHAIN_MEMO_MAX_LEN) {
                PRINTF("Invalid memo length. %d > 250 bytes\n", context->memo_length);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
            }
            if (context->memo_bytes_remaining) {
                uint8_t memo_str_offset = context->memo_length - context->memo_bytes_remaining;
                uint8_t n_bytes_to_convert = MIN(PARAMETER_LENGTH, context->memo_bytes_remaining);
                hex_to_ascii((const char *) (&memo_str + memo_str_offset),
                             msg->parameter,
                             n_bytes_to_convert);
                context->memo_bytes_remaining -= n_bytes_to_convert;
            } else {
                /* Append null terminator to memo and parse */
                memo_str[MIN((context->memo_length) - 1, THORCHAIN_MEMO_MAX_LEN)] = '\0';
                int error = parse_memo(context->memo, (char *) memo_str);
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
                if (error != 0) {
                    msg->result = ETH_PLUGIN_RESULT_ERROR;
                    break;
                }
                context->next_param = UNEXPECTED_PARAMETER;
            }

            break;

        // Keep this
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
