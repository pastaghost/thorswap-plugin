#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdint.h>

/** This is included to allow the use of sprintf in the hex_to_ascii() function below.
 * THIS WAS THE SOURCE OF UNTOLD HOURS OF FRUSTRATION. FOR THE LOVE OF GOD TO NOT REMOVE THIS.
 */
void *_sbrk(int incr);

size_t hex_to_ascii(const char *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len);

#endif