#ifndef BLINKER_UTILITY_H__
#define BLINKER_UTILITY_H__

#include <stdint.h>
#include <string.h>

#include "app_timer.h"
// #include "Blinker_cJSON.h"
#include "cJSON.h"

#include "BlinkerConfig.h"

typedef void (*blinker_callback_t)(void);
typedef void (*blinker_callback_with_arg_t)(void*);
typedef void (*blinker_callback_with_json_arg_t)(const cJSON *data);
typedef void (*blinker_callback_with_string_arg_t)(const char *data);
typedef void (*blinker_callback_with_string_uint8_arg_t)(const char *data, uint8_t num);
typedef void (*blinker_callback_with_int32_arg_t)(int32_t data);
typedef void (*blinker_callback_with_uint8_arg_t)(uint8_t data);
typedef void (*blinker_callback_with_tab_arg_t)(uint8_t data);
typedef void (*blinker_callback_with_begin_t)(const char * k1, ...);
typedef void (*blinker_callback_with_rgb_arg_t)(uint8_t r_data, uint8_t g_data, uint8_t b_data, uint8_t bright_data);
typedef void (*blinker_callback_with_int32_uint8_arg_t)(int32_t data, uint8_t num);

int8_t isJson(const char *data);
uint32_t blinker_millis(void);
void blinker_substring(char * dest, char * src, uint16_t left, uint16_t right);

#endif
