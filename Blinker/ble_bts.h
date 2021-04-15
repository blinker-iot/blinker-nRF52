/**
 * Copyright (c) 2012 - 2019, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_bts Link Loss Service
 * @{
 * @ingroup ble_sdk_srv
 * @brief Link Loss Service module.
 *
 * @details This module implements the Link Loss Service with the Alert Level characteristic.
 *          During initialization it adds the Link Loss Service and Alert Level characteristic
 *          to the BLE stack database.
 *
 *          The application must supply an event handler for receiving Link Loss Service
 *          events. Using this handler, the service will notify the application when the
 *          link has been lost, and which Alert Level has been set.
 *
 *          The service also provides a function for letting the application poll the current
 *          value of the Alert Level characteristic.
 *
 * @note    The application must register this module as BLE event observer using the
 *          NRF_SDH_BLE_OBSERVER macro. Example:
 *          @code
 *              ble_bts_t instance;
 *              NRF_SDH_BLE_OBSERVER(anything, BLE_BTS_BLE_OBSERVER_PRIO,
 *                                   ble_bts_on_ble_evt, &instance);
 *          @endcode
 *
 * @note Attention!
 *  To maintain compliance with Nordic Semiconductor ASA Bluetooth profile
 *  qualification listings, this section of source code must not be modified.
*/

#ifndef BLE_BTS_H__
#define BLE_BTS_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"
#include "ble_link_ctx_manager.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef BLE_BTS_BLE_OBSERVER_PRIO
#define BLE_BTS_BLE_OBSERVER_PRIO 2
#endif

/**@brief   Macro for defining a ble_bts instance.
 *
 * @param   _name   Name of the instance.
 * @hideinitializer
 */
#define BLE_BTS_DEF(_name, _bts_max_clients)                      \
    BLE_LINK_CTX_MANAGER_DEF(CONCAT_2(_name, _link_ctx_storage),  \
                             (_bts_max_clients),                  \
                             sizeof(ble_bts_client_context_t));   \
    static ble_bts_t _name =                                      \
    {                                                             \
        .p_link_ctx_storage = &CONCAT_2(_name, _link_ctx_storage) \
    };                                                            \
    NRF_SDH_BLE_OBSERVER(_name ## _obs,                           \
                     BLE_BTS_BLE_OBSERVER_PRIO,                   \
                     ble_bts_on_ble_evt, &_name)                  \

#define BLE_BTS_MAX_DATA_LEN            20
#define BLE_BTS_MAX_RX_CHAR_LEN         BLE_BTS_MAX_DATA_LEN /**< Maximum length of the RX Characteristic (in bytes). */
#define BLE_BTS_MAX_TX_CHAR_LEN         BLE_BTS_MAX_DATA_LEN /**< Maximum length of the TX Characteristic (in bytes). */


/**@brief Link Loss Service event type. */
typedef enum
{
    BLE_BTS_EVT_RX_DATA,      /**< Data received. */
    BLE_BTS_EVT_TX_RDY,       /**< Service is ready to accept new data to be transmitted. */
    BLE_BTS_EVT_COMM_STARTED, /**< Notification has been enabled. */
    BLE_BTS_EVT_COMM_STOPPED, /**< Notification has been disabled. */

    // BLE_BTS_EVT_LINK_LOSS_ALERT                         /**< Alert Level Updated event. */
} ble_bts_evt_type_t;


// Forward declaration of the ble_bts_t type.
typedef struct ble_bts_s ble_bts_t;


/**@brief   Nordic UART Service @ref BLE_BTS_EVT_RX_DATA event data.
 *
 * @details This structure is passed to an event when @ref BLE_BTS_EVT_RX_DATA occurs.
 */
typedef struct
{
    uint8_t const * p_data; /**< A pointer to the buffer with received data. */
    uint16_t        length; /**< Length of received data. */
} ble_bts_evt_rx_data_t;


/**@brief Nordic UART Service client context structure.
 *
 * @details This structure contains state context related to hosts.
 */
typedef struct
{
    bool is_notification_enabled; /**< Variable to indicate if the peer has enabled notification of the RX characteristic.*/
} ble_bts_client_context_t;


/**@brief Link Loss Service event. */
typedef struct
{
    ble_bts_evt_type_t evt_type;                        /**< Type of event. */
    ble_bts_t                * p_bts;       /**< A pointer to the instance. */
    uint16_t                   conn_handle; /**< Connection handle. */
    ble_bts_client_context_t * p_link_ctx;  /**< A pointer to the link context. */
    union
    {
        uint8_t alert_level;                            /**< New Alert Level value. */
        ble_bts_evt_rx_data_t rx_data; /**< @ref BLE_BTS_EVT_RX_DATA event data. */
    } params;
} ble_bts_evt_t;

/**@brief Link Loss Service event handler type. */
typedef void (*ble_bts_evt_handler_t) (ble_bts_t * p_bts, ble_bts_evt_t * p_evt);


/**@brief Nordic UART Service event handler type. */
typedef void (* ble_bts_data_handler_t) (ble_bts_evt_t * p_evt);


/**@brief   Nordic UART Service initialization structure.
 *
 * @details This structure contains the initialization information for the service. The application
 * must fill this structure and pass it to the service using the @ref ble_bts_init
 *          function.
 */
// typedef struct
// {
//     ble_bts_data_handler_t data_handler; /**< Event handler to be called for handling received data. */
// } ble_bts_init_t;


/**@brief Link Loss Service init structure. This contains all options and data needed for initialization of the service. */
typedef struct
{
    // ble_bts_evt_handler_t     evt_handler;              /**< Event handler to be called for handling events in the Link Loss Service. */
    // ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
    // uint8_t                   initial_alert_level;      /**< Initial value of the Alert Level characteristic. */
    // security_req_t            alert_level_rd_sec;       /**< Security requirement for reading Alert Level characteristic. */
    // security_req_t            alert_level_wr_sec;       /**< Security requirement for writing Alert Level characteristic. */
    ble_bts_data_handler_t    data_handler; /**< Event handler to be called for handling received data. */
} ble_bts_init_t;

/**@brief Link Loss Service structure. This contains various status information for the service. */
struct ble_bts_s
{
    // ble_bts_evt_handler_t     evt_handler;              /**< Event handler to be called for handling events in the Link Loss Service. */
    // ble_srv_error_handler_t   error_handler;            /**< Function to be called in case of an error. */
    uint8_t                         uuid_type;          /**< UUID type for Nordic UART Service Base UUID. */
    uint16_t                  service_handle;           /**< Handle of Link Loss Service (as provided by the BLE stack). */
    // ble_gatts_char_handles_t  alert_level_handles;      /**< Handles related to the Alert Level characteristic. */
    // uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    ble_gatts_char_handles_t        tx_rx_handles;
    blcm_link_ctx_storage_t * const p_link_ctx_storage; /**< Pointer to link context storage with handles of all current connections and its context. */
    ble_bts_data_handler_t          data_handler;       /**< Event handler to be called for handling received data. */
};


/**@brief Function for initializing the Link Loss Service.
 *
 * @param[out]  p_bts       Link Loss Service structure. This structure will have to be supplied by
 *                          the application. It will be initialized by this function, and will later
 *                          be used to identify this particular service instance.
 * @param[in]   p_bts_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_bts_init(ble_bts_t * p_bts, const ble_bts_init_t * p_bts_init);


/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Link Loss Service.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 * @param[in]   p_context   Link Loss Service structure.
 */
void ble_bts_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


/**@brief Function for getting current value of the Alert Level characteristic.
 *
 * @param[in]   p_bts          Link Loss Service structure.
 * @param[out]  p_alert_level  Current Alert Level value.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
// uint32_t ble_bts_alert_level_get(ble_bts_t * p_bts, uint8_t * p_alert_level);


/**@brief   Function for sending a data to the peer.
 *
 * @details This function sends the input string as an RX characteristic notification to the
 *          peer.
 *
 * @param[in]     p_bts       Pointer to the Nordic UART Service structure.
 * @param[in]     p_data      String to be sent.
 * @param[in,out] p_length    Pointer Length of the string. Amount of sent bytes.
 * @param[in]     conn_handle Connection Handle of the destination client.
 *
 * @retval NRF_SUCCESS If the string was sent successfully. Otherwise, an error code is returned.
 */
uint32_t ble_bts_data_send(ble_bts_t * p_bts,
                           uint8_t   * p_data,
                           uint16_t  * p_length,
                           uint16_t    conn_handle);


#ifdef __cplusplus
}
#endif

#endif // BLE_BTS_H__

/** @} */
