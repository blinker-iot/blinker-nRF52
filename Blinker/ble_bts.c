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
/* Attention!
 * To maintain compliance with Nordic Semiconductor ASA's Bluetooth profile
 * qualification listings, this section of source code must not be modified.
 */
#include "sdk_common.h"
// #if NRF_MODULE_ENABLED(BLE_LLS)
#include "ble_bts.h"
#include <string.h>
#include "ble_hci.h"
#include "ble_srv_common.h"





/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_bts   Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_bts_t * p_bts, ble_evt_t const * p_ble_evt)
{
    ret_code_t                 err_code;
    // Link reconnected, notify application with a no_alert event
    ble_bts_evt_t evt;

    ble_gatts_value_t          gatts_val;
    uint8_t                    cccd_value[2];
    ble_bts_client_context_t * p_client = NULL;

    // p_bts->conn_handle     = p_ble_evt->evt.gap_evt.conn_handle;

    // evt.evt_type           = BLE_LLS_EVT_LINK_LOSS_ALERT;
    // evt.params.alert_level = BLE_CHAR_ALERT_LEVEL_NO_ALERT;
    // p_bts->evt_handler(p_bts, &evt);

    err_code = blcm_link_ctx_get(p_bts->p_link_ctx_storage,
                                 p_ble_evt->evt.gap_evt.conn_handle,
                                 (void *) &p_client);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.",
                      p_ble_evt->evt.gap_evt.conn_handle);
    }

    NRF_LOG_INFO("on_connect NRF_SUCCESS");

    /* Check the hosts CCCD value to inform of readiness to send data using the RX characteristic */
    memset(&gatts_val, 0, sizeof(ble_gatts_value_t));
    gatts_val.p_value = cccd_value;
    gatts_val.len     = sizeof(cccd_value);
    gatts_val.offset  = 0;

    // err_code = sd_ble_gatts_value_get(p_ble_evt->evt.gap_evt.conn_handle,
    //                                   p_bts->tx_handles.cccd_handle,
    //                                   &gatts_val);
    err_code = sd_ble_gatts_value_get(p_ble_evt->evt.gap_evt.conn_handle,
                                      p_bts->tx_rx_handles.cccd_handle,
                                      &gatts_val);

    if ((err_code == NRF_SUCCESS)     &&
        (p_bts->data_handler != NULL) &&
        ble_srv_is_notification_enabled(gatts_val.p_value))
    {
        if (p_client != NULL)
        {
            p_client->is_notification_enabled = true;
        }

        memset(&evt, 0, sizeof(ble_bts_evt_t));
        evt.evt_type    = BLE_BTS_EVT_COMM_STARTED;
        evt.p_bts       = p_bts;
        evt.conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
        evt.p_link_ctx  = p_client;

        p_bts->data_handler(&evt);
    }
}


/**@brief Function for handling the @ref BLE_GATTS_EVT_WRITE event from the SoftDevice.
 *
 * @param[in] p_bts     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_bts_t * p_bts, ble_evt_t const * p_ble_evt)
{
    ret_code_t                    err_code;
    ble_bts_evt_t                 evt;
    ble_bts_client_context_t    * p_client;
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    err_code = blcm_link_ctx_get(p_bts->p_link_ctx_storage,
                                 p_ble_evt->evt.gatts_evt.conn_handle,
                                 (void *) &p_client);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_INFO("Link context for 0x%02X connection handle could not be fetched.",
                      p_ble_evt->evt.gatts_evt.conn_handle);
    }
    NRF_LOG_INFO("on_write NRF_SUCCESS");

    memset(&evt, 0, sizeof(ble_bts_evt_t));
    evt.p_bts       = p_bts;
    evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
    evt.p_link_ctx  = p_client;

    // if ((p_evt_write->handle == p_bts->tx_handles.cccd_handle) &&
    //     (p_evt_write->len == 2))
    if ((p_evt_write->handle == p_bts->tx_rx_handles.cccd_handle) &&
        (p_evt_write->len == 2))
    {
        NRF_LOG_INFO("on_write if 1");
        if (p_client != NULL)
        {
            if (ble_srv_is_notification_enabled(p_evt_write->data))
            {
                p_client->is_notification_enabled = true;
                evt.evt_type                          = BLE_BTS_EVT_COMM_STARTED;
            }
            else
            {
                p_client->is_notification_enabled = false;
                evt.evt_type                          = BLE_BTS_EVT_COMM_STOPPED;
            }

            if (p_bts->data_handler != NULL)
            {
                p_bts->data_handler(&evt);
            }

        }
    }
    else if ((p_evt_write->handle == p_bts->tx_rx_handles.value_handle) &&
             (p_bts->data_handler != NULL))    
    // else if ((p_evt_write->handle == p_bts->rx_handles.value_handle) &&
    //          (p_bts->data_handler != NULL))
    {
        NRF_LOG_INFO("on_write else if 2");
        evt.evt_type                  = BLE_BTS_EVT_RX_DATA;
        evt.params.rx_data.p_data = p_evt_write->data;
        evt.params.rx_data.length = p_evt_write->len;

        p_bts->data_handler(&evt);
    }
    else
    {
        NRF_LOG_INFO("on_write else 3");
        // Do Nothing. This event is not relevant for this service.
    }
}


/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_bts       Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
// static void on_disconnect(ble_bts_t * p_bts, ble_evt_t const * p_ble_evt)
// {
//     uint8_t reason = p_ble_evt->evt.gap_evt.params.disconnected.reason;

//     if (reason == BLE_HCI_CONNECTION_TIMEOUT)
//     {
//         // Link loss detected, notify application
//         uint32_t      err_code;
//         ble_bts_evt_t evt;

//         evt.evt_type = BLE_LLS_EVT_LINK_LOSS_ALERT;

//         err_code = ble_bts_alert_level_get(p_bts, &evt.params.alert_level);
//         if (err_code == NRF_SUCCESS)
//         {
//             p_bts->evt_handler(p_bts, &evt);
//         }
//         else
//         {
//             if (p_bts->error_handler != NULL)
//             {
//                 p_bts->error_handler(err_code);
//             }
//         }
//     }
// }


/**@brief Function for handling the Authentication Status event.
 *
 * @param[in]   p_bts       Link Loss Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
// static void on_auth_status(ble_bts_t * p_bts, ble_evt_t const * p_ble_evt)
// {
//     if (p_ble_evt->evt.gap_evt.params.auth_status.auth_status == BLE_GAP_SEC_STATUS_SUCCESS)
//     {
//         ble_bts_evt_t evt;

//         evt.evt_type           = BLE_LLS_EVT_LINK_LOSS_ALERT;
//         evt.params.alert_level = BLE_CHAR_ALERT_LEVEL_NO_ALERT;

//         p_bts->evt_handler(p_bts, &evt);
//     }
// }


/**@brief Function for handling the @ref BLE_GATTS_EVT_HVN_TX_COMPLETE event from the SoftDevice.
 *
 * @param[in] p_bts     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_hvx_tx_complete(ble_bts_t * p_bts, ble_evt_t const * p_ble_evt)
{
    ret_code_t                 err_code;
    ble_bts_evt_t              evt;
    ble_bts_client_context_t * p_client;

    err_code = blcm_link_ctx_get(p_bts->p_link_ctx_storage,
                                 p_ble_evt->evt.gatts_evt.conn_handle,
                                 (void *) &p_client);
    if (err_code != NRF_SUCCESS)
    {
        NRF_LOG_ERROR("Link context for 0x%02X connection handle could not be fetched.",
                      p_ble_evt->evt.gatts_evt.conn_handle);
        return;
    }

    if (p_client->is_notification_enabled)
    {
        memset(&evt, 0, sizeof(ble_bts_evt_t));
        evt.evt_type        = BLE_BTS_EVT_TX_RDY;
        evt.p_bts       = p_bts;
        evt.conn_handle = p_ble_evt->evt.gatts_evt.conn_handle;
        evt.p_link_ctx  = p_client;

        // p_bts->data_handler(&evt);
    }
}


void ble_bts_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_bts_t * p_bts = (ble_bts_t *)p_context;

    if (p_bts == NULL || p_ble_evt == NULL)
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_bts, p_ble_evt);
            NRF_LOG_INFO("BLE_GAP_EVT_CONNECTED");
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_bts, p_ble_evt);
            NRF_LOG_INFO("BLE_GATTS_EVT_WRITE");
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            // on_disconnect(p_bts, p_ble_evt);
            NRF_LOG_INFO("BLE_GAP_EVT_DISCONNECTED");
            break;

        case BLE_GAP_EVT_AUTH_STATUS:
            // on_auth_status(p_bts, p_ble_evt);
            NRF_LOG_INFO("BLE_GAP_EVT_AUTH_STATUS");
            break;

        case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            on_hvx_tx_complete(p_bts, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_bts_init(ble_bts_t * p_bts, const ble_bts_init_t * p_bts_init)
{
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;

    if (p_bts == NULL || p_bts_init == NULL)
    {
        return NRF_ERROR_NULL;
    }

    if (p_bts_init->data_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    // Initialize service structure
    // p_bts->evt_handler   = p_bts_init->evt_handler;
    // p_bts->error_handler = p_bts_init->error_handler;
    p_bts->data_handler  = p_bts_init->data_handler;

    // Add service
    // BLE_UUID_BLE_ASSIGN(ble_uuid, BLE_UUID_LINK_LOSS_SERVICE);
    BLE_UUID_BLE_ASSIGN(ble_uuid, 0xFFE0);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_bts->service_handle);
    // if (err_code != NRF_SUCCESS)
    // {
    //     return err_code;
    // }

    VERIFY_SUCCESS(err_code);

    // Add alert level characteristic
    memset(&add_char_params, 0, sizeof(add_char_params));

    add_char_params.uuid             = 0xFFE1;//BLE_UUID_ALERT_LEVEL_CHAR;
    add_char_params.uuid_type        = p_bts->uuid_type;
    add_char_params.max_len          = 20;//sizeof(uint8_t);
    add_char_params.char_props.read  = 1;
    // add_char_params.char_props.write = 1;
    add_char_params.char_props.write_wo_resp = 1;
    add_char_params.char_props.notify        = 1;
    add_char_params.write_access     = 1;//p_bts_init->alert_level_wr_sec;
    add_char_params.read_access      = 1;//p_bts_init->alert_level_rd_sec;
    add_char_params.cccd_write_access= 1;
    add_char_params.init_len         = sizeof(uint8_t);
    add_char_params.is_var_len               = true;
    //add_char_params.p_init_value     = (uint8_t *) &(p_bts_init->initial_alert_level);

    return characteristic_add(p_bts->service_handle,
                              &add_char_params,
                              &p_bts->tx_rx_handles);
}


// uint32_t ble_bts_alert_level_get(ble_bts_t * p_bts, uint8_t * p_alert_level)
// {
//     ble_gatts_value_t gatts_value;

//     if (p_bts == NULL || p_alert_level == NULL)
//     {
//         return NRF_ERROR_NULL;
//     }

//     // Initialize value struct.
//     memset(&gatts_value, 0, sizeof(gatts_value));

//     gatts_value.len     = sizeof(uint8_t);
//     gatts_value.offset  = 0;
//     gatts_value.p_value = p_alert_level;

//     return sd_ble_gatts_value_get(p_bts->conn_handle,
//                                   p_bts->alert_level_handles.value_handle,
//                                   &gatts_value);
// }


uint32_t ble_bts_data_send(ble_bts_t * p_bts,
                           uint8_t   * p_data,
                           uint16_t  * p_length,
                           uint16_t    conn_handle)
{
    ret_code_t                 err_code;
    ble_gatts_hvx_params_t     hvx_params;
    ble_bts_client_context_t * p_client;

    NRF_LOG_INFO("ble_bts_data_send in");

    VERIFY_PARAM_NOT_NULL(p_bts);

    err_code = blcm_link_ctx_get(p_bts->p_link_ctx_storage, conn_handle, (void *) &p_client);
    VERIFY_SUCCESS(err_code);

    if ((conn_handle == BLE_CONN_HANDLE_INVALID) || (p_client == NULL))
    {
        NRF_LOG_INFO("NRF_ERROR_NOT_FOUND");
        return NRF_ERROR_NOT_FOUND;
    }

    if (!p_client->is_notification_enabled)
    {
        NRF_LOG_INFO("NRF_ERROR_INVALID_STATE");
        return NRF_ERROR_INVALID_STATE;
    }

    if (*p_length > BLE_BTS_MAX_DATA_LEN)
    {
        NRF_LOG_INFO("NRF_ERROR_INVALID_PARAM");
        return NRF_ERROR_INVALID_PARAM;
    }

    NRF_LOG_INFO("ble_bts_data_send");

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_bts->tx_rx_handles.value_handle;//tx_handles.value_handle;
    hvx_params.p_data = p_data;
    hvx_params.p_len  = p_length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
}

// #endif // NRF_MODULE_ENABLED(BLE_LLS)
