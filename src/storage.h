#ifndef _STORAGE_H_
#define _STORAGE_H_

#include <stdint.h>
#include "constants.h"
#include "memo.h"

extern uint8_t memo_str[THORCHAIN_MEMO_MAX_LEN + 1];
extern memo_t memo_data;

#endif