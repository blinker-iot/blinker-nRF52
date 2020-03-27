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

int8_t isJson(const char *data)
{
    if (data[0] != '{' || data[strlen(data) - 1] != '}') return 0;

    return 1;

    // cJSON *root = cJSON_Parse(data);

    // if (!root)
    // {
    //     cJSON_Delete(root);
    //     return 0;
    // }
    // else
    // {
    //     cJSON_Delete(root);
    //     return 1;
    // }
    
}

uint32_t blinker_millis(void)
{
    return 0;//app_timer_cnt_get()* ( (1 + 1 ) * 1000 ) / APP_TIMER_CLOCK_FREQ;
}

void blinker_substring(char * dest, char * src, uint16_t left, uint16_t right)
{
    memset(dest, '\0', strlen(dest));

    uint16_t len = strlen(src);

    for (uint16_t num = left; num < right; num++)
    {
        dest[num - left] = src[num];
    }
}

#endif
