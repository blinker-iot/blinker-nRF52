#ifndef BLINKER_DEBUG_H__
#define BLINKER_DEBUG_H__

#include <stdint.h>
#include <string.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "BlinkerUtility.h"

static bool is_debug_all = false;

void blinker_log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);

    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

#define BLINKER_LOG_MODULE(...) { \
    printf("[%d]: ", blinker_millis()); \
    printf( __VA_ARGS__ ); \
    printf("\r\n"); \
}

#define BLINKER_ERR_LOG_MODULE(...) { \
    printf("[%d]: ERROR: ", blinker_millis()); \
    printf( __VA_ARGS__ ); \
    printf("\r\n"); \
}

#define BLINKER_LOG_ALL_MODULE(...) { \
    if (is_debug_all) { \
        BLINKER_LOG_MODULE(__VA_ARGS__); \
    } \
}

#define BLINKER_DEBUG_ALL()     is_debug_all = true;

#define BLINKER_LOG(...)        BLINKER_LOG_MODULE( __VA_ARGS__ );

#define BLINKER_LOG_ALL(...)    BLINKER_LOG_ALL_MODULE( __VA_ARGS__ );

#define BLINKER_ERR_LOG(...)    BLINKER_ERR_LOG_MODULE( __VA_ARGS__ );

#endif
