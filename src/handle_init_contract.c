#include "thorswap_plugin.h"
#include "memo.h"

static int find_selector(uint32_t selector, const uint32_t *selectors, size_t n, selector_t *out) {
    for (selector_t i = 0; i < n; i++) {
        if (selector == selectors[i]) {
            *out = i;
            return 0;
        }
    }
    return -1;
}

// Called once to init.
void handle_init_contract(void *parameters) {
    // Cast the msg to the type of structure we expect (here, ethPluginInitContract_t).
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    // Make sure we are running a compatible version.
    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        // If not the case, return the `UNAVAILABLE` status.
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    // Double check that the `context_t` struct is not bigger than the maximum size (defined by
    // `msg->pluginContextLength`).
    if (msg->pluginContextLength < sizeof(context_t)) {
        PRINTF("Plugin parameters structure is bigger than allowed size\n");
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    context_t *context = (context_t *) msg->pluginContext;

    // Initialize the context (to 0).
    // memset(context, 0, sizeof(*context));

    // Initialize struct to hold memo data and store a reference to the struct in the app context
    // memset(&parsed_memo, 0, sizeof(parsed_memo));
    context->memo = &parsed_memo;

    uint32_t selector = U4BE(msg->selector, 0);
    if (find_selector(selector,
                      THORCHAIN_ROUTER_V4_SELECTORS,
                      NUM_SELECTORS,
                      &context->selectorIndex)) {
        PRINTF("DEBUG: **** SELECTOR NOT FOUND ****\n");
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    switch (context->selectorIndex) {
        case DEPOSIT_WITH_EXPIRY:
            context->next_param = VAULT;
            break;

        default:
            PRINTF("Missing selectorIndex: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    // Return valid status.
    msg->result = ETH_PLUGIN_RESULT_OK;
}
