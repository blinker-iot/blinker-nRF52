#define BLINKER_BLE

#include "BlinkerApi.h"

BlinkerButton button1 = {.name = "btn-abc"};
BlinkerNumber number1 = {.name = "num-abc"};
BlinkerRGB rgb1 = {.name = "rgb"};

int counter = 0;

void button1_callback(const char *data)
{
    BLINKER_LOG("get button data: %s", data);

    blinker_button_config_t config = {
        .icon = "fas fa-alicorn",
        .color = "0xFF",
        .text1 = "test",
    };

    blinker_button_print(&button1, &config);
}

void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value)
{
    BLINKER_LOG("R value: %d", r_value);
    BLINKER_LOG("G value: %d", g_value);
    BLINKER_LOG("B value: %d", b_value);
    BLINKER_LOG("Rrightness value: %d", bright_value);

    blinker_rgb_config_t config = {
        .rgbw = {r_value, g_value, b_value, bright_value},
    };

    blinker_rgb_print(&rgb1, &config);
}

void data_callback(const cJSON *data)
{
    BLINKER_LOG("get json data");

    counter++;

    char count[10];
    sprintf(count, "%d", counter);

    blinker_number_config_t config = {
        .value = count,
    };

    blinker_number_print(&number1, &config);
}

void main(void)
{
    BLINKER_DEBUG_ALL();
    
    blinker_button_init(&button1, button1_callback);
    blinker_rgb_init(&rgb1, rgb1_callback);
    blinker_attach_data(data_callback);
    
    blinker_init();
}
