#include <stdio.h>
#include <stdint.h>
#include "utils.h"

void *_sbrk(int incr) {
    return (void *) -1;
}

size_t hex_to_ascii(const char *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len) {
    size_t i = 0, r = 0;
    for (i = 0; i < hex_buf_len; i++) {
        r += sprintf((char *) ascii_buf + 2 * i, "%02x", hex_buf[i]);
    }
    return r;
}