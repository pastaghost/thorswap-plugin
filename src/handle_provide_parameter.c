#include <string.h>
#include <stdio.h>
#include "thorchain_router_plugin.h"

// EDIT THIS: Remove this function and write your own handlers!
static void handle_deposit_with_expiry(ethPluginProvideParameter_t *msg, context_t *context) {
    if (context->go_to_offset) {
        if (msg->parameterOffset != context->offset + SELECTOR_SIZE) {
            return;
        }
        context->go_to_offset = false;
    }
    switch (context->next_param) {
        case VAULT:  // THORChain vault address
            copy_address(context->vault, msg->parameter, sizeof(context->vault));
            context->next_param = ASSET;
            break;
        case ASSET:  // Asset contract address
            copy_address(context->asset, msg->parameter, sizeof(context->asset));
            context->next_param = AMOUNT;
            break;
        case AMOUNT:  // to
            copy_parameter(context->amount, msg->parameter, sizeof(context->amount));
            context->next_param = MEMO_OFFSET;
            break;
        case MEMO_OFFSET:
            context->memo_offset =
                U2BE(msg->memo_offset, PARAMETER_LENGTH - sizeof(context->memo_offset));
            context->next_param = EXPIRATION;
            break;
        case EXPIRATION:  // path[1] -> contract address of token received
            copy_parameter(context->expiration, msg->parameter, sizeof(context->expiration));
            context->next_param = MEMO_LENGTH;
            break;
        case MEMO_LENGTH:
            context->memo_length =
                U2BE(msg->memo_length, PARAMETER_LENGTH - sizeof(context->memo_length));
            context->memo_bytes_remaining = context->memo_length;
            context->next_param = MEMO;
            break;
        case MEMO:
            if (context->memo_bytes_remaining) {
                uint8_t offset = context->memo_length - context->memo_bytes_remaining;
                uint8_t bytes_to_convert = MIN(PARAMETER_LENGTH, context->memo_bytes_remaining);
                hex_to_ascii(context->memo + offset, msg->parameter, bytes_to_convert);
                context->memo_bytes_remaining -= bytes_to_convert;
            } else {
                /* Append null terminator to memo */
                context->memo[MIN(memo_length - 1, THORCHAIN_MEMO_MAX_LEN)] = '/0';
                parse_memo(context);
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
        case BOILERPLATE_DUMMY_2:
            break;
        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void parse_memo(context_t *context) {
    char *token;
    char *tokens[6];
    uint8_t dest_size;
    uint8_t copy_len;

    /* Bail early if we don't have any memo data to parse */
    if (!strlen(context->memo)) {
        return;
    }

    /* Parse and discard handler token ex: (Swap, s, =) */
    tokens[0] = strtok(context->memo, ":");
    for (int i = 1; i < 6; i++) {
        tokens[i] = strtok(NULL, ":");
    }

    context->memo_num_fields = 0;

    /* Parse remaining memo data from string (asset, dest_addr, limit, affiliate, fee) */
    if (tokens[1] != NULL) {
        strncpy((char *) &(context->memo_data.asset),
                tokens[1],
                sizeof(context->memo_data.asset) / sizeof(uint8_t));
        context->memo_num_fields++;
    }

    if (tokens[2] != NULL) {
        strncpy((char *) &(context->memo_data.dest_addr),
                tokens[2],
                sizeof(context->memo_data.dest_addr) / sizeof(uint8_t));
        context->memo_num_fields++;
    }

    if (tokens[3] != NULL) {
        strncpy((char *) &(context->memo_data.limit),
                tokens[3],
                sizeof(context->memo_data.limit) / sizeof(uint8_t));
        context->memo_num_fields++;
    }

    if (tokens[4] != NULL) {
        strncpy((char *) &(context->memo_data.affiliate),
                tokens[4],
                sizeof(context->memo_data.affiliate) / sizeof(uint8_t));
        context->memo_num_fields++;
    }

    if (tokens[5] != NULL) {
        strncpy((char *) &(context->memo_data.fee),
                tokens[5],
                sizeof(context->memo_data.fee) / sizeof(uint8_t));
        context->memo_num_fields++;
    }
}

size_t hex_to_ascii(uint8_t *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len) {
    size_t i;
    size_t r;
    for (i = 0; i < hex_buf_len; i++) {
        r += sprintf((char *) ascii_buf + 2 * i, "%02x", hex_buf[i]);
    }
    return r;
}