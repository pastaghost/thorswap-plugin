#include "thorchain_router_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    /* [VAULT, ASSET, AMOUNT, ...Variable number of memo fields] */
    msg->numScreens = 3 + context->memo_num_fields;

    // EDIT THIS: set `tokenLookup1` (and maybe `tokenLookup2`) to point to
    // token addresses you will info for (such as decimals, ticker...).
    msg->tokenLookup1 = context->asset;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
