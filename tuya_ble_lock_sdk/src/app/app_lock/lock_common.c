#include "lock_common.h"




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
uint8_t g_open_with_bt_flag = 1;




/*********************************************************
FN: 
*/
void lock_common_init(void)
{
    app_port_kv_init();
    
    app_port_local_clock_start();
    
    lock_timer_creat();
    
    app_common_init();
    
    APP_DEBUG_HEXDUMP("Auth key", 40, tuya_ble_current_para.auth_settings.auth_key, AUTH_KEY_LEN);
    APP_DEBUG_HEXDUMP("Device id", 20, tuya_ble_current_para.auth_settings.device_id, DEVICE_ID_LEN);
}

/*********************************************************
FN: 
*/
void lock_factory_handler(bool is_delete_app_test_data)
{
    app_port_tuya_ble_device_factory_reset();
    
    //erase all lock falsh
    lock_flash_erease_all(is_delete_app_test_data);
}




/*********************************************************
FN: 
*/
uint32_t lock_open_with_bt(void)
{
    //open success
    if(g_open_with_bt_flag)
    {
        APP_DEBUG_PRINTF("lock_open_with_bt");
        return APP_PORT_SUCCESS;
    }
    return APP_PORT_ERROR_COMMON;
}

/*********************************************************
FN: 
*/
uint32_t lock_remote_anti_lock(bool anti_lock)
{
    if(anti_lock)
    {
        APP_DEBUG_PRINTF("remote anti_lock true");
        return APP_PORT_SUCCESS;
    }
    else
    {
        APP_DEBUG_PRINTF("remote anti_lock false");
        return APP_PORT_SUCCESS;
    }
}




























