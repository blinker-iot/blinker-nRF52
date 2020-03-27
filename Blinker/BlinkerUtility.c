#include "BlinkerUtility.h"

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
    return app_timer_cnt_get()* ( (1 + 1 ) * 1000 ) / APP_TIMER_CLOCK_FREQ;
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