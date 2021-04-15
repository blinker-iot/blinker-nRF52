#ifndef BLINKER_BLE_H__
#define BLINKER_BLE_H__

#include "sdk_common.h"
#include <stdint.h>
#include <string.h>
#include "stdlib.h"
#include "BlinkerUtility.h"
#include "BlinkerDebug.h"
#include "ble_bts.h"
#if NRF_MODULE_ENABLED(BLINKER_BLE)
void blinker_ble_flush(void);
void blinker_ble_init(blinker_callback_with_string_arg_t func);
void blinker_ble_print(char * data, bool need_check);
#endif

#endif
