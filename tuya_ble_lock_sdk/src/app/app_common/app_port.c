#include "app_port.h"




/*********************************************************************
 * LOCAL CONSTANTS
 */

/*********************************************************************
 * LOCAL STRUCT
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTION
 */

/*********************************************************************
 * VARIABLES
 */




/*********************************************************  Storage  *********************************************************/

/*********************************************************
FN: 
*/
void *app_port_malloc(uint32_t size)
{
    return tuya_ble_malloc(size);
}

/*********************************************************
FN: 
*/
uint32_t app_port_free(void* buf)
{
    return tuya_ble_free(buf);
}

/*********************************************************
FN: 
*/
uint32_t app_port_kv_init(void)
{
    nrfs_flash_init();
    return easyflash_init();
}

/*********************************************************
FN: 
*/
uint32_t app_port_kv_set(const char *key, const void *buf, size_t size)
{
    return ef_set_env_blob(key, buf, size);
}

/*********************************************************
FN: 
*/
uint32_t app_port_kv_get(const char *key, void *buf, size_t size)
{
    return ef_get_env_blob(key, buf, size, NULL);
}

/*********************************************************
FN: 
*/
uint32_t app_port_kv_del(const char *key)
{
    return ef_del_env(key);
}

/*********************************************************
FN: 
*/
uint32_t app_port_nv_write(uint32_t addr, const uint8_t* p_data, uint32_t size)
{
    return tuya_ble_nv_write(addr, p_data, size);
}

/*********************************************************
FN: 
*/
uint32_t app_port_nv_erase(uint32_t addr, uint32_t size)
{
    return tuya_ble_nv_erase(addr, size);
}


/*********************************************************  timer  *********************************************************/

/*********************************************************
FN: 
*/
uint32_t app_port_timer_create(void** p_timer_id, uint32_t timeout_value_ms, tuya_ble_timer_mode mode, tuya_ble_timer_handler_t timeout_handler)
{
    return tuya_ble_timer_create(p_timer_id, timeout_value_ms, mode, timeout_handler);
}

/*********************************************************
FN: 
*/
uint32_t app_port_timer_delete(void* timer_id)
{
    return tuya_ble_timer_delete(timer_id);
}

/*********************************************************
FN: 
*/
uint32_t app_port_timer_start(void* timer_id)
{
    return tuya_ble_timer_start(timer_id);
}

/*********************************************************
FN: 
*/
uint32_t app_port_timer_stop(void* timer_id)
{
    return tuya_ble_timer_stop(timer_id);
}

/*********************************************************
FN: 
*/
uint32_t app_port_update_timestamp(uint32_t app_timestamp)
{
    nrfs_update_timestamp(app_timestamp);
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: if timestamp is not updated from poweron, return value is local timestamp
*/
uint32_t app_port_get_timestamp(void)
{
    return nrfs_get_timestamp();
}

/*********************************************************
FN: 
*/
uint32_t app_port_get_old_timestamp(uint32_t old_local_timestamp)
{
    return nrfs_get_old_timestamp(old_local_timestamp);
}


/*********************************************************  ble  *********************************************************/

/*********************************************************
FN: 
*/
uint32_t app_port_dp_data_report(uint8_t *buf, uint32_t size)
{
    if(app_port_get_connect_status() == BONDING_CONN)
    {
        APP_DEBUG_HEXDUMP("dp_rsp", 20, buf, size);
        return tuya_ble_dp_data_report(buf, size);
    } else {
        APP_DEBUG_HEXDUMP("dp_rsp_unconn", 20, buf, size);
        return APP_PORT_ERROR_COMMON;
    }
}

/*********************************************************
FN: 
*/
uint32_t app_port_dp_data_with_time_report(uint32_t timestamp, uint8_t *buf, uint32_t size)
{
    if(app_port_get_connect_status() == BONDING_CONN)
    {
        APP_DEBUG_PRINTF("timestamp: %d", timestamp);
        APP_DEBUG_HEXDUMP("dp_rsp_timestamp", 20, buf, size);
        return tuya_ble_dp_data_with_time_report(timestamp, buf, size);
    } else {
        APP_DEBUG_PRINTF("timestamp: %d", timestamp);
        APP_DEBUG_HEXDUMP("dp_rsp_timestamp_unconn", 20, buf, size);
        return APP_PORT_ERROR_COMMON;
    }
}

/*********************************************************
FN: 
*/
uint32_t app_port_ota_rsp(tuya_ble_ota_data_response_t *rsp)
{
    if(rsp->type != TUYA_BLE_OTA_DATA)
    {
        APP_DEBUG_PRINTF("ota_rsp_type: %d", rsp->type);
        APP_DEBUG_HEXDUMP("ota_rsp_data", 20, rsp->p_data, rsp->data_len);
    }
    return tuya_ble_ota_response(rsp);
}

/*********************************************************
FN: 
*/
tuya_ble_connect_status_t app_port_get_connect_status(void)
{
    return tuya_ble_connect_status_get();
}

/*********************************************************
FN: 
*/
void app_port_set_bt_mac_addr(uint8_t* addr)
{
    nrfs_set_bt_mac_addr(addr);
}

/*********************************************************
FN: 
*/
void app_port_get_bt_mac_addr(uint8_t* addr)
{
    if(addr == NULL)
    {
        return;
    }
    
    uint32_t err_code;
    
    ble_gap_addr_t p_addr;
    err_code = sd_ble_gap_addr_get(&p_addr);
    APP_ERROR_CHECK(err_code);
    
    memcpy(addr, p_addr.addr, 6);
}

/*********************************************************
FN: 
*/
uint32_t app_port_conn_param_update(uint16_t cMin, uint16_t cMax, uint16_t latency, uint16_t timeout)
{
    return nrfs_conn_param_update(cMin, cMax, latency, timeout);
}

/*********************************************************
FN: 
*/
uint32_t app_port_ble_gap_disconnect(void)
{
    return tuya_ble_gap_disconnect();
}

/*********************************************************
FN: 
*/
uint32_t app_port_ble_conn_evt_ext(void)
{
    uint32_t err_code;
    ble_opt_t  opt;
    memset(&opt, 0x00, sizeof(opt));
    opt.common_opt.conn_evt_ext.enable = true;
    err_code = sd_ble_opt_set(BLE_COMMON_OPT_CONN_EVT_EXT, &opt);
    APP_ERROR_CHECK(err_code);
    return err_code;
}




/*********************************************************  device  *********************************************************/

/*********************************************************
FN: 
*/
uint32_t app_port_device_reset(void)
{
    APP_DEBUG_PRINTF("device reset");
    tuya_ble_device_reset();
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
uint32_t app_port_local_clock_start(void)
{
    nrfs_local_timer_start();
    return APP_PORT_SUCCESS;
}

/*********************************************************
FN: 
*/
uint32_t app_port_ble_sdk_init(tuya_ble_device_param_t * param_data)
{
    return tuya_ble_sdk_init(param_data);
}

/*********************************************************
FN: 
*/
uint32_t app_port_ble_callback_queue_register(tuya_ble_callback_t cb)
{
    return tuya_ble_callback_queue_register(cb);
}

/*********************************************************
FN: 
*/
uint8_t app_port_ble_custom_event_send(tuya_ble_custom_evt_t evt)
{
    return tuya_ble_custom_event_send(evt);
}

/*********************************************************
FN: 
*/
uint32_t app_port_uart_send_data(const uint8_t* buf,uint16_t size)
{
    return tuya_ble_uart_send_data(buf, size);
}

/*********************************************************
FN: 
*/
void app_port_factory_test_process(uint8_t* p_in_data, uint16_t in_len, uint8_t* p_out_data, uint16_t* out_len)
{
    factory_test_process(p_in_data, in_len, p_out_data, out_len);
}

/*********************************************************
FN: 
*/
uint32_t app_port_dfu_settings_init(bool sd_irq_initialized)
{
    return nrf_dfu_settings_init(sd_irq_initialized);
}

/*********************************************************
FN: 
*/
uint32_t app_port_dfu_settings_write_and_backup(nrf_dfu_flash_callback_t callback)
{
    return nrf_dfu_settings_write_and_backup(callback);
}

/*********************************************************
FN: 
*/
uint32_t app_port_tuya_ble_device_factory_reset(void)
{
    return tuya_ble_device_factory_reset();
}




/*********************************************************  check  *********************************************************/

/*********************************************************
FN: 
*/
uint8_t app_port_check_sum(uint8_t *buf, uint32_t size)
{
    return cpt_check_sum(buf, size);
}

/*********************************************************
FN: 
*/
uint16_t app_port_check_sum_16(uint8_t *buf, uint32_t size)
{
    return cpt_check_sum_16(buf, size);
}

/*********************************************************
FN: crc16
*/
uint16_t app_port_crc16_compute(uint8_t* buf, uint16_t size, uint16_t* p_crc)
{
    return cpt_crc16_compute(buf, size, p_crc);
}

/*********************************************************
FN: crc32
*/
uint32_t app_port_crc32_compute(uint8_t* buf, uint32_t size, uint32_t* p_crc)
{
    return cpt_crc32_compute(buf, size, p_crc);
}

/*********************************************************
FN: 
*/
void app_port_reverse_byte(void* buf, uint32_t size)
{
    cpt_reverse_byte(buf, size);
}


/*********************************************************  string  *********************************************************/

/*********************************************************
FN: string handle
*/
uint8_t app_port_string_op_hexstr2hex(uint8_t *hexstr, int len, uint8_t* hex)
{
	return ty_string_op_hexstr2hex(hexstr, len, hex);
}

uint8_t app_port_string_op_hex2hexstr(uint8_t *hex, int len, uint8_t* hexstr)
{
	return ty_string_op_hex2hexstr(hex, len, hexstr);
}










