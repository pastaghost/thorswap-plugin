#include "thorswap_router_plugin.h"

static void set_vault_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Vault", msg->titleLength);

    // Prefix the address with `0x`.
    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    // We need a random chainID for legacy reasons with `getEthAddressStringFromBinary`.
    // Setting it to `0` will make it work with every chainID :)
    uint64_t chainid = 0;

    // Get the string representation of the address stored in `context->vault`. Put it in
    // `msg->msg`.
    getEthAddressStringFromBinary(
        context->vault,
        msg->msg + 2,  // +2 here because we've already prefixed with '0x'.
        msg->pluginSharedRW->sha3,
        chainid);
}

static void set_asset_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Source Asset", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->asset,
            MIN(sizeof(context->memo->asset) / sizeof(char), msg->msgLength));
}

static void set_amount_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Amount", msg->titleLength);

    uint8_t decimals = context->decimals;
    char *ticker = (char *) context->ticker;

    // If the token look up failed, use the default network ticker along with the default decimals.
    if (!context->token_found) {
        decimals = WEI_TO_ETHER;
        ticker = msg->network_ticker;
    }

    amountToString(context->amount,
                   sizeof(context->amount),
                   decimals,
                   ticker,
                   msg->msg,
                   msg->msgLength);
}

static void set_memo_dest_addr_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Dest. Address", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->dest_addr,
            MIN(sizeof(context->memo->dest_addr) / sizeof(char), msg->msgLength));
}

static void set_memo_limit_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Limit", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->limit,
            MIN(sizeof(context->memo->limit) / sizeof(char), msg->msgLength));
}

static void set_memo_affiliate_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Affiliate", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->affiliate,
            MIN(sizeof(context->memo->affiliate) / sizeof(char), msg->msgLength));
}

static void set_memo_fee_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Fee", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->fee,
            MIN(sizeof(context->memo->fee) / sizeof(char), msg->msgLength));
}

static void set_memo_dex_aggregator_addr_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Dex Aggregator Addr", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->dex_aggregator_addr,
            MIN(sizeof(context->memo->dex_aggregator_addr) / sizeof(char), msg->msgLength));
}

static void set_memo_final_token_addr_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Final Token Addr", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->final_token_addr,
            MIN(sizeof(context->memo->final_token_addr) / sizeof(char), msg->msgLength));
}

static void set_memo_min_amount_out_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Min Amount Out", msg->titleLength);
    strlcpy(msg->msg,
            context->memo->min_amount_out,
            MIN(sizeof(context->memo->min_amount_out) / sizeof(char), msg->msgLength));
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    // msg->title is the upper line displayed on the device.
    // msg->msg is the lower line displayed on the device.

    // Clean the display fields.
    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: Adapt the cases for the screens you'd like to display.
    switch (msg->screenIndex) {
        case 0:
            set_vault_ui(msg, context);
            break;
        case 1:
            set_asset_ui(msg, context);
            break;
        case 2:
            set_amount_ui(msg, context);
            break;
        case 3:
            set_memo_dest_addr_ui(msg, context);
            break;
        case 4:
            set_memo_limit_ui(msg, context);
            break;
        case 5:
            set_memo_affiliate_ui(msg, context);
            break;
        case 6:
            set_memo_fee_ui(msg, context);
            break;
        case 7:
            set_memo_dex_aggregator_addr_ui(msg, context);
            break;
        case 8:
            set_memo_final_token_addr_ui(msg, context);
            break;
        case 9:
            set_memo_min_amount_out_ui(msg, context);
            break;
        // Keep this
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
