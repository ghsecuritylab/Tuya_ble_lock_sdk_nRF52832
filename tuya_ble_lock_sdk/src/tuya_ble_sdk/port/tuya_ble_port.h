/**
 * \file tuya_ble_port.h
 *
 * \brief 
 */
/*
 *  Copyright (C) 2014-2019, Tuya Inc., All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  This file is part of tuya ble sdk 
 */


#ifndef TUYA_BLE_PORT_H__
#define TUYA_BLE_PORT_H__


#include <stdbool.h>
#include <stdint.h>
#include "tuya_ble_type.h"
#include "nrfs_common.h" //please remove this head file if your platform is not "nRF52832"


#ifdef TUYA_BLE_LOG_ENABLED

#define TUYA_BLE_LOG(...)   log_d(__VA_ARGS__)
#define TUYA_BLE_HEXDUMP    nrfs_log_hexdump

#else

#define TUYA_BLE_LOG(...)
#define TUYA_BLE_HEXDUMP    nrfs_log_hexdump_empty

#endif


/*
 *@brief   ble port api 
 *@param    
 *        
 *@note 
 for example the data format of the tuya_ble_gap_advertising_adv_data_update() function:
 
static const uint8_t adv_data[] =
{
    0x02,             // length 
    GAP_ADTYPE_FLAGS, // type="Flags" 
    GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
    
    0x0B,             // length 
    GAP_ADTYPE_LOCAL_NAME_COMPLETE,// Local name 
    'I', 'P', 'C', '_', 'C', 'A', 'M', 'E', 'R', 'A',
    0x03,
    GAP_ADTYPE_16BIT_MORE,
    0x00, 0xA3,
}; 

static const uint8_t scan_rsp_data[] =
{    
    0x0E,                             // length 
    GAP_ADTYPE_MANUFACTURER_SPECIFIC, // Manufacturer Specific Data 
    HI_WORD(COMPANY_ID),
    LO_WORD(COMPANY_ID),
    0x00,0x02,
    LO_WORD(DEVICE_TYPE),
    'I', 'P', 'C','C', 'A', 'M', 'E', 'R',
    
}; 

tuya_ble_gap_advertising_adv_data_update(adv_data,sizeof(adv_data));

tuya_ble_gap_advertising_scan_rsp_data_update(scan_rsp_data,sizeof(scan_rsp_data));
 *           
 * */
 
//tuya_ble_status_t tuya_ble_gap_adv_start(void);

tuya_ble_status_t tuya_ble_gap_advertising_adv_data_update(uint8_t const * p_ad_data, uint8_t ad_len);

tuya_ble_status_t tuya_ble_gap_advertising_scan_rsp_data_update(uint8_t const *p_sr_data, uint8_t sr_len);

//tuya_ble_status_t tuya_ble_gap_adv_stop(void);

//tuya_ble_status_t tuya_ble_gap_update_conn_params(tuya_ble_gap_conn_param_t conn_params);

tuya_ble_status_t tuya_ble_gap_disconnect(void);

tuya_ble_status_t tuya_ble_gap_address_get(tuya_ble_addr_t mac);

tuya_ble_status_t tuya_ble_gatt_send_data(const uint8_t *p_data,uint16_t len);


/**
 * @brief Initialize uart peripheral.
 * @note   UART_PARITY_NO_PARTY,UART_STOP_BITS_1,UART_WROD_LENGTH_8BIT;
 *         9600 baud rate.
 * @param No parameter.
 *
 * @return tuya_ble_status_t
 */
tuya_ble_status_t tuya_ble_uart_init(void);


/**
 * @brief Send data to uart.
 * @note 
 *
 * @param p_data the send data buffer
 * @param len to send bytes size
 *
 * @return result
 */
tuya_ble_status_t tuya_ble_uart_send_data(const uint8_t *p_data,uint16_t len);



/**
 * @brief   Create a timer.
 * @param   [out] p_timer_id: a pointer to timer id address which can uniquely identify the timer.
 *          [in] timeout_handler: a pointer to a function which can be
 * called when the timer expires.
 *          [in] mode: repeated or single shot.
 * @return  TUYA_BLE_SUCCESS             If the timer was successfully created.
 *          TUYA_BLE_ERR_INVALID_PARAM   Invalid timer id supplied.
 *          TUYA_BLE_ERR_INVALID_STATE   timer module has not been initialized or the
 * timer is running.
 *          TUYA_BLE_ERR_NO_MEM          timer pool is full.
 *
 * */
tuya_ble_status_t tuya_ble_timer_create(void** p_timer_id,uint32_t timeout_value_ms, tuya_ble_timer_mode mode,tuya_ble_timer_handler_t timeout_handler);

/**
 * @brief   Delete a timer.
 * @param   [in] timer_id: timer id
 * @return  TUYA_BLE_SUCCESS             If the timer was successfully deleted.
 *          TUYA_BLE_ERR_INVALID_PARAM   Invalid timer id supplied..
 * */
tuya_ble_status_t tuya_ble_timer_delete(void* timer_id);


/**
 * @brief   Start a timer.
 * @param   [in] timer_id: timer id
 *          [in] timeout_value: Number of milliseconds to time-out event
 * (minimum 10 ms).
 *          [in] p_context: parameters that can be passed to
 * timeout_handler
 *
 * @return  TUYA_BLE_SUCCESS             If the timer was successfully started.
 *          TUYA_BLE_ERR_INVALID_PARAM   Invalid timer id supplied.
 *          TUYA_BLE_ERR_INVALID_STATE   If the application timer module has not been
 * initialized or the timer has not been created.
 *          TUYA_BLE_ERR_NO_MEM          If the timer operations queue was full.
 * @note    If the timer has already started, it will start counting again.
 * */
tuya_ble_status_t tuya_ble_timer_start(void* timer_id);


/**
 * @brief   Stop a timer.
 * @param   [in] timer_id: timer id
 * @return  TUYA_BLE_SUCCESS             If the timer was successfully stopped.
 *          TUYA_BLE_ERR_INVALID_PARAM   Invalid timer id supplied.
 *
 * */
tuya_ble_status_t tuya_ble_timer_stop(void* timer_id);


/**
 * @brief Function for delaying execution for a number of milliseconds.
 *
 * @param ms_time Number of milliseconds to wait.
 */
void tuya_ble_device_delay_ms(uint32_t ms);


/**
 * @brief Function for delaying execution for a number of microseconds.
 *
 * @param us_time Number of microseconds to wait.
 */
void tuya_ble_device_delay_us(uint32_t us);



/**
 * @brief   Function for RESET device.
 *
 * @note    
 *.
 * */
tuya_ble_status_t tuya_ble_device_reset(void);



/**@brief Function for entering a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       tuya_ble_device_exit_critical() for each call to tuya_ble_device_enter_critical(), and they must be located
 *       in the same scope.
 */
void tuya_ble_device_enter_critical(void);

/**@brief Macro for leaving a critical region.
 *
 * @note Due to implementation details, there must exist one and only one call to
 *       tuya_ble_device_exit_critical() for each call to tuya_ble_device_enter_critical(), and they must be located
 *       in the same scope.
 */

void tuya_ble_device_exit_critical(void);

/**
 * @brief   Get ture random bytes .
 * @param   [out] p_buf: pointer to data
 *          [in] len: Number of bytes to take from pool and place in
 * p_buff
 * @return  TUYA_BLE_SUCCESS          The requested bytes were written to
 * p_buff
 *          TUYA_BLE_ERR_NO_MEM       No bytes were written to the buffer, because
 * there were not enough random bytes available.
 * @note    SHOULD use TRUE random num generator
 * */
tuya_ble_status_t tuya_ble_rand_generator(uint8_t* p_buf, uint8_t len);


/**
 * @brief   Function for get the unix timestamp.
 *
 * @note    
 *.
 * */

tuya_ble_status_t tuya_ble_rtc_get_timestamp(uint32_t *timestamp,int32_t *timezone);

/**
 * @brief   Function for set the unix timestamp.
 *
 * @note    
 *.
 * */

tuya_ble_status_t tuya_ble_rtc_set_timestamp(uint32_t timestamp,int32_t timezone);


/**
 * @brief Initialize the NV module.
 * @note  
 * @note  
 *
 * @param 
 * @param 
 *
 * @return result
 */
tuya_ble_status_t tuya_ble_nv_init(void);

/**
 * @brief Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
tuya_ble_status_t tuya_ble_nv_erase(uint32_t addr,uint32_t size);

/**
 * @brief Write data to flash.
 * 
 * @note This operation must after erase. @see tuya_ble_nv_erase.
 *
 * @param addr flash address
 * @param p_data the write data buffer
 * @param size write bytes size
 *
 * @return result
 */
tuya_ble_status_t tuya_ble_nv_write(uint32_t addr,const uint8_t * p_data, uint32_t size);



/**
 * @brief Read data from flash.
 * @note 
 *
 * @param addr flash address
 * @param buf buffer to store read data
 * @param size read bytes size
 *
 * @return result
 */
tuya_ble_status_t tuya_ble_nv_read(uint32_t addr,uint8_t * p_data, uint32_t size);



#if TUYA_BLE_USE_OS
/**
 * 
 *
 * \brief Create a new task and add it to the list of tasks that are ready to run.
 *
 * \param[out]  pp_handle  Used to pass back a handle by which the created task
 *                         can be referenced.
 *
 * \param[in]   p_name     A descriptive name for the task.
 *
 * \param[in]   p_routine  Pointer to task routine function that must be implemented
 *                         to never return.
 *
 * \param[in]   p_param    Pointer parameter passed to the task routine function.
 *
 * \param[in]   stack_size The size of the task stack that is specified as the number
 *                         of bytes.
 *
 * \param[in]   priority   The priority at which the task should run. Higher priority
 *                         task has higher priority value.
 *
 * \return           The status of the task creation.
 * \retval true      Task was created successfully and added to task ready list.
 * \retval false     Task was failed to create.
 */
bool tuya_ble_os_task_create(void **pp_handle, const char *p_name, void (*p_routine)(void *),void *p_param, uint16_t stack_size, uint16_t priority);


/**
 * 
 *
 * \brief Remove a task from RTOS's task management. The task being deleted will be removed
 * from RUNNING, READY or WAITING state.
 *
 * \param[in] p_handle  The handle of the task to be deleted.
 *
 * \return           The status of the task deletion.
 * \retval true      Task was deleted successfully.
 * \retval false     Task was failed to delete.
 */
bool tuya_ble_os_task_delete(void *p_handle);


/**
 * 
 *
 * \brief Suspend the task. The suspended task will not be scheduled and never get
 * any microcontroller processing time.
 *
 * \param[in] p_handle  The handle of the task to be suspended.
 *
 * \return           The status of the task suspension.
 * \retval true      Task was suspended successfully.
 * \retval false     Task was failed to suspend.
 */
bool tuya_ble_os_task_suspend(void *p_handle);

/**
 * 
 *
 * \brief Resume the suspended task.
 *
 * \param[in] p_handle  The handle of the task to be resumed.
 *
 * \return           The status of the task resume.
 * \retval true      Task was resumed successfully.
 * \retval false     Task was failed to resume.
 */
bool tuya_ble_os_task_resume(void *p_handle);


/**
 * 
 *
 * \brief   Creates a message queue instance. This allocates the storage required by the
 *          new queue and passes back a handle for the queue.
 *
 * \param[out]  pp_handle  Used to pass back a handle by which the message queue
 *                         can be referenced.
 *
 * \param[in]   msg_num    The maximum number of items that the queue can contain.
 *
 * \param[in]   msg_size   The number of bytes each item in the queue will require. Items
 *                         are queued by copy, not by reference, so this is the number of
 *                         bytes that will be copied for each posted item. Each item on the
 *                         queue must be the same size.
 *
 * \return           The status of the message queue creation.
 * \retval true      Message queue was created successfully.
 * \retval false     Message queue was failed to create.
 */
bool tuya_ble_os_msg_queue_create(void **pp_handle, uint32_t msg_num, uint32_t msg_size) ;

/**
 *
 *
 * \brief   Delete the specified message queue, and free all the memory allocated for
 *          storing of items placed on the queue.
 *
 * \param[in]   p_handle   The handle to the message queue being deleted.
 *
 * \return           The status of the message queue deletion.
 * \retval true      Message queue was deleted successfully.
 * \retval false     Message queue was failed to delete.
 */
bool tuya_ble_os_msg_queue_delete(void *p_handle);


/**
 * 
 *
 * \brief    Peek the number of items sent and resided on the message queue.
 *
 * \param[in]   p_handle   The handle to the message queue being peeked.
 *
 * \param[out]  p_msg_num  Used to pass back the number of items residing on the message queue.
 *
 * \return           The status of the message queue peek.
 * \retval true      Message queue was peeked successfully.
 * \retval false     Message queue was failed to peek.
 */
bool tuya_ble_os_msg_queue_peek(void *p_handle, uint32_t *p_msg_num);

/**
 * 
 *
 * \brief   Send an item to the back of the specified message queue. The item is
 *          queued by copy, not by reference.
 *
 * \param[in]   p_handle The handle to the message queue on which the item is to be sent.
 *
 * \param[in]   p_msg    Pointer to the item that is to be sent on the queue. The referenced
 *                       item rather than pointer itself will be copied on the queue.
 *
 * \param[in]   wait_ms  The maximum amount of time in milliseconds that the task should
 *                       block waiting for the item to sent on the queue.
 * \arg \c 0           No blocking and return immediately.
 * \arg \c 0xFFFFFFFF  Block infinitely until the item sent.
 * \arg \c others      The timeout value in milliseconds.
 *
 * \return           The status of the message item sent.
 * \retval true      Message item was sent successfully.
 * \retval false     Message item was failed to send.
 */
bool tuya_ble_os_msg_queue_send(void *p_handle, void *p_msg, uint32_t wait_ms);


/**
 * 
 *
 * \brief   Receive an item from the specified message queue. The item is received by
 *          copy rather than by reference, so a buffer of adequate size must be provided.
 *
 * \param[in]   p_handle The handle to the message queue from which the item is to be received.
 *
 * \param[out]  p_msg    Pointer to the buffer into which the received item will be copied.
 *                       item rather than pointer itself will be copied on the queue.
 *
 * \param[in]   wait_ms  The maximum amount of time in milliseconds that the task should
 *                       block waiting for an item to be received from the queue.
 * \arg \c 0           No blocking and return immediately.
 * \arg \c 0xFFFFFFFF  Block infinitely until the item received.
 * \arg \c others      The timeout value in milliseconds.
 *
 * \return           The status of the message item received.
 * \retval true      Message item was received successfully.
 * \retval false     Message item was failed to receive.
 */
bool tuya_ble_os_msg_queue_recv(void *p_handle, void *p_msg, uint32_t wait_ms);

#endif


/**
    * @brief  128 bit AES ECB encryption on speicified plaintext and keys
    * @param  input    specifed plain text to be encypted
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @param  key          keys to encrypt the plaintext
    * @param  output    output buffer to store encrypted data
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
bool tuya_ble_aes128_ecb_encrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output);

/**
    * @brief  128 bit AES ECB decryption on speicified encrypted data and keys
    * @param  input    specifed encypted data to be decypted
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @return decryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
*/
bool tuya_ble_aes128_ecb_decrypt(uint8_t *key,uint8_t *input,uint16_t input_len,uint8_t *output);

/**
    * @brief  128 bit AES CBC encryption on speicified plaintext and keys
    * @param  input    specifed plain text to be encypted
    * @param  key          keys to encrypt the plaintext
    * @param  output    output buffer to store encrypted data
    * @param  iv         initialization vector (IV) for CBC mode
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
bool tuya_ble_aes128_cbc_encrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output);

/**
    * @brief  128 bit AES CBC descryption on speicified plaintext and keys
    * @param  input    specifed encypted data to be decypted
    * @param  key          keys to decrypt the data
    * @param  output    output buffer to store plain data
    * @param  iv         initialization vector (IV) for CBC mode
    * @param  input_len    byte length of the data to be descrypted, must be multiples of 16
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   least significant octet of encrypted data corresponds to encypted[0]
    */
bool tuya_ble_aes128_cbc_decrypt(uint8_t *key,uint8_t *iv,uint8_t *input,uint16_t input_len,uint8_t *output);

/**
    * @brief  MD5 checksum 
    * @param  input    specifed plain text to be encypted
    * @param  output    output buffer to store md5 result data,output data len is always 16
    * @param  input_len    byte length of the data to be encypted
    * @return encryption results
    * @retval true      successful
    * @retval false     fail
    * @note   
    */
bool tuya_ble_md5_crypt(uint8_t *input,uint16_t input_len,uint8_t *output);


#endif





