#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdint.h>

size_t hex_to_ascii(const char *ascii_buf, const uint8_t *hex_buf, size_t hex_buf_len);

#endif