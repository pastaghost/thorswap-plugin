#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdint.h>
#include "thorswap_plugin.h"

/** This is included to allow the use of sprintf in the hex_to_ascii() function below.
 * THIS WAS THE SOURCE OF UNTOLD HOURS OF FRUSTRATION. FOR THE LOVE OF GOD TO NOT REMOVE THIS.
 */
void *_sbrk(int incr);

size_t hex_to_ascii(const char *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len);
void debug_print_context(context_t *context, cx_sha3_t *sha_context);
void debug_memo(memo_t *memo);

#endif