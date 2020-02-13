/**
 * \file tuya_ble_cryption_api.c
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
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "tuya_ble_type.h"
#include "tuya_ble_heap.h"
#include "tuya_ble_mem.h"
#include "tuya_ble_api.h"
#include "tuya_ble_port.h"
#include "tuya_ble_main.h"
#include "tuya_ble_cryption_api.h"
#include "tuya_ble_internal_config.h"
#include "tuya_ble_utils.h"

extern tuya_ble_parameters_settings_t tuya_ble_current_para;

static uint8_t service_rand[16] = {0};


static bool tuya_ble_device_id_key_generate(uint8_t *key_in,uint16_t key_len,uint8_t *key_out);


static bool tuya_ble_generate_key1(uint8_t *input,uint16_t input_len,uint8_t *output)
{
    //
    uint8_t key[16];
    uint8_t iv[16];

    if(input_len%16)
    {
        return false;
    }

    memcpy(key,tuya_ble_current_para.auth_settings.auth_key,16);
    memset(iv,0,16);

    tuya_ble_aes128_cbc_encrypt(key,iv,input,input_len,output);
    //
    return true;
}


bool tuya_ble_device_id_encrypt(uint8_t *key_in,uint16_t key_len,uint8_t *input,uint16_t input_len,uint8_t *output)
{
    uint8_t key[16];
    uint8_t iv[16];
    //
    if(!tuya_ble_device_id_key_generate(key_in,key_len,key))
    {
        TUYA_BLE_LOG("device id key generate error!");
        return false;
    }
    else
    {
        memcpy(iv,key,16);
        TUYA_BLE_HEXDUMP("device id key :",16,(u8*)key,16);//
    }

    if(input_len%16)
    {
        return false;
    }

   // mbedtls_aes_crypt_cbc(&aes_ctx,MBEDTLS_AES_ENCRYPT,input_len,iv,input,output);
    //
    tuya_ble_aes128_cbc_encrypt(key,iv,input,input_len,output);

    return true;
}




#define KEY_IN_BUFFER_LEN_MAX 64

extern uint8_t tuya_ble_pair_rand[6];

static bool tuya_ble_key_generate(uint8_t mode,uint8_t *key_out)
{
    uint16_t len = 0;
    static uint8_t key_out_hex[16] = {0};
    static uint8_t key_out_key1[48] = {0};
    static uint8_t key_in_buffer[KEY_IN_BUFFER_LEN_MAX] = {0};

    if(mode>=ENCRYPTION_MODE_MAX)
    {
        return false;
    }
    if(mode==ENCRYPTION_MODE_NONE)
    {
        return true;
    }

    memset(key_in_buffer,0,sizeof(key_in_buffer));
    memset(key_out_key1,0,sizeof(key_out_key1));
    len = 0;
    //首先基于device_id和auth_key计算secret_key_1
    // memcpy(key_in_buffer,tuya_para.auth_settings.device_id,DEVICE_ID_LEN);
    // len += DEVICE_ID_LEN;

//   hextoascii(key_out_hex,16,key_out_char);
//   memcpy(key_out_hex,key_out_char+8,sizeof(key_out_hex));
    memcpy(key_in_buffer+len,tuya_ble_current_para.auth_settings.auth_key,AUTH_KEY_LEN);
    len += AUTH_KEY_LEN;
//    for (int i = 0; i<16; i++)
//    {
//        tuya_log_d("%02x", key_out_hex[i]);
//    }

    if(mode==ENCRYPTION_MODE_KEY_1)
    {
        memcpy(key_in_buffer+len,service_rand,16);
        len += 16;
        if(!tuya_ble_generate_key1(key_in_buffer,AUTH_KEY_LEN+16,key_out_key1))
        {
            TUYA_BLE_LOG("tuya_aes_generate_key1 error\n");
            return false;
        }
        memcpy(key_out,key_out_key1+32,16);
        //TUYA_BLE_HEXDUMP("secret_key_1",16,(u8*)key_out_key1+32,16);//
        return true;
    }
    else
    {
        tuya_ble_md5_crypt(key_in_buffer,len,key_out_hex);
    }

    memset(key_in_buffer,0,sizeof(key_in_buffer));
    if((mode!=ENCRYPTION_MODE_KEY_1)&&(mode!=ENCRYPTION_MODE_KEY_4))
    {
        if(memcmp(tuya_ble_pair_rand,key_in_buffer,PAIR_RANDOM_LEN)==0)
        {
            return false;
        }
    }
    if((mode==ENCRYPTION_MODE_KEY_3)&&(TUYA_BLE_ADVANCED_ENCRYPTION_DEVICE==1))
    {
        if(memcmp(tuya_ble_current_para.sys_settings.user_rand,key_in_buffer,PAIR_RANDOM_LEN)==0)  //没有 user_rand
        {
            return false;
        }
    }

    if((16+PAIR_RANDOM_LEN*2)>KEY_IN_BUFFER_LEN_MAX)
    {
        return false;
    }
    len = 0;
    //TUYA_BLE_HEXDUMP("secret_key_base",16,(u8*)key_out_hex,16);//
   // TUYA_BLE_HEXDUMP("pair_rand data",6,(u8*)tuya_ble_pair_rand,6);//
    switch(mode)
    {
    case ENCRYPTION_MODE_KEY_2:
        memcpy(key_in_buffer,key_out_hex,16);
        len += 16;
        memcpy(key_in_buffer+len,tuya_ble_pair_rand,PAIR_RANDOM_LEN);
        len += PAIR_RANDOM_LEN;
        break;
    case ENCRYPTION_MODE_KEY_3:
        memcpy(key_in_buffer,key_out_hex,16);
        len += 16;
        memcpy(key_in_buffer+len,tuya_ble_pair_rand,PAIR_RANDOM_LEN);
        len += PAIR_RANDOM_LEN;
        memcpy(key_in_buffer+len,tuya_ble_current_para.sys_settings.user_rand,PAIR_RANDOM_LEN);
        len += PAIR_RANDOM_LEN;
        break;
    case ENCRYPTION_MODE_KEY_4:
        memcpy(key_in_buffer+len,tuya_ble_current_para.sys_settings.login_key,LOGIN_KEY_LEN);
        len += LOGIN_KEY_LEN;
        break;
    case ENCRYPTION_MODE_SESSION_KEY:
        memcpy(key_in_buffer+len,tuya_ble_current_para.sys_settings.login_key,LOGIN_KEY_LEN);
        len += LOGIN_KEY_LEN;
        memcpy(key_in_buffer+len,tuya_ble_pair_rand,PAIR_RANDOM_LEN);
        len += PAIR_RANDOM_LEN;
        break;
    default:
        break;
    };
    if(len==0)
    {
        return false;
    }

    memset(key_out_hex,0,sizeof(key_out_hex));
    // tuya_log_dumpHex("key_in_buffer data",32,(u8*)key_in_buffer,len);//
    tuya_ble_md5_crypt(key_in_buffer,len,key_out_hex);
    //TUYA_BLE_HEXDUMP("key_out_hex data",16,(u8*)key_out_hex,16);//
    //  hextoascii(key_out_hex,16,key_out_char);
    memcpy(key_out,key_out_hex,16);

    return true;
}


void tuya_ble_key_generate_test(uint8_t mode,uint8_t *key_out)
{
    tuya_ble_key_generate(mode,key_out);
}


static bool tuya_ble_device_id_key_generate(uint8_t *key_in,uint16_t key_len,uint8_t *key_out)
{
    uint16_t len = 0;
    static uint8_t device_id_key_out_hex[16] = {0};
    // static uint8_t device_id_key_out_char[32] = {0};
    static uint8_t device_id_key_in_buffer[64] = {0};

    memset(device_id_key_in_buffer,0,sizeof(device_id_key_in_buffer));
    len = 0;

    memcpy(device_id_key_in_buffer+len,key_in,key_len);
    len += key_len;
    tuya_ble_md5_crypt(device_id_key_in_buffer,len,device_id_key_out_hex);
    // hextoascii(device_id_key_out_hex,16,device_id_key_out_char);
    // memcpy(key_out,device_id_key_out_char+8,16);
    memcpy(key_out,device_id_key_out_hex,16);

    return true;
}


static uint16_t tuya_ble_Add_Pkcs(uint8_t *p, uint16_t len)
{
    uint8_t pkcs[16];
    uint16_t out_len;

    if(len%16==0)
    {
        out_len = len;
    }
    else
    {
        uint16_t i = 0, cz = 16-len%16;
        memset(pkcs, 0, 16);
        for( i=0; i<cz; i++ ) {
            pkcs[i]=cz;
        }
        memcpy(p + len, pkcs, cz);
        out_len = len + cz;
    }
    return (out_len);
}


uint8_t  tuya_ble_encrypt_old_with_key(uint8_t *key,uint8_t *in_buf,uint8_t in_len,uint8_t *out_buf)
{
    uint16_t i = 0,tmplen = 0;
    if(in_len>((TUYA_BLE_AIR_FRAME_MAX)/2-1))
        return 1;//超过输出缓冲区长度

    tuya_ble_hextoascii(in_buf,in_len,out_buf+1);
    tmplen = tuya_ble_Add_Pkcs(out_buf+1,in_len*2+1);

    if(tmplen>255)
        return 2;//超过输出缓冲区长度

    //tuya_log_v("tmplen=%d-%d",in_len,tmplen);
    tuya_ble_aes128_ecb_encrypt(key,&out_buf[1+i],tmplen,&out_buf[1+i]);
    out_buf[0] = tmplen;

    return 0;
}



//加密函数
uint8_t tuya_ble_encryption(uint8_t encryption_mode,uint8_t *iv,uint8_t *in_buf,uint32_t in_len,uint32_t *out_len,uint8_t *out_buf)
{
    uint16_t len = 0;
    uint8_t key[16];
    bool key_ok = false;

    if(encryption_mode>=ENCRYPTION_MODE_MAX)
    {
        return 2;//加密模式无效
    }

    if(encryption_mode==ENCRYPTION_MODE_NONE)
    {
        memcpy(out_buf,in_buf,in_len);
        *out_len = in_len;
        return 0;
    }
    else
    {
        len = tuya_ble_Add_Pkcs(in_buf,in_len);
    }

    memset(key,0,sizeof(key));
    key_ok = tuya_ble_key_generate(encryption_mode,key);
    // tuya_log_dumpHex("in_buf data 1",32,(u8*)in_buf,len);//
    // tuya_log_d("key generated ok,len = %d",len);
    if(key_ok)
    {
        // tuya_log_dumpHex("in_buf data 2",32,(u8*)in_buf,len);//
        if(tuya_ble_aes128_cbc_encrypt(key,iv,in_buf,len,out_buf)==true)
        {
            *out_len = len;
            //  tuya_log_d("len = %d",*out_len);
            return 0;//成功
        }
        else
        {
            return 3;//加密失败
        }
    }
    else
    {
        TUYA_BLE_LOG("key error!");
        return 4; //key计算失败
    }

}


//static uint8_t decry_buf[AIR_FRAME_MAX];
//解密函数
uint8_t tuya_ble_decryption(uint8_t const *in_buf,uint32_t in_len,uint32_t *out_len,uint8_t *out_buf)
{
    uint16_t len = 0;
    uint8_t key[16];
    uint8_t IV[16];
    uint8_t mode = 0;
    if(in_len<17)
    {
        return 1;  //数据长度错误
    }
    if(in_buf[0]>=ENCRYPTION_MODE_MAX)
    {
        return 2;//加密模式无效
    }

    if(in_buf[0]==ENCRYPTION_MODE_NONE)
    {
        len = in_len - 1;
        memcpy(out_buf,in_buf+1,len);
        *out_len = len;
        return 0;
    }

    len = in_len-17;

    memset(key,0,sizeof(key));
    memset(IV,0,sizeof(IV));
    mode = in_buf[0];
    //  memcpy(decry_buf,in_buf,AIR_FRAME_MAX);
    if(mode==ENCRYPTION_MODE_KEY_1)
    {
        memcpy(service_rand,in_buf+1,16); //iv==rand
    }
    if(tuya_ble_key_generate(mode,key))
    {
        //tuya_log_dumpHex("in_buf data",32,(u8*)in_buf,in_len);//
        //     tuya_log_dumpHex("key",16,(u8*)key,16);//
        memcpy(IV,in_buf+1,16);
        //     tuya_log_dumpHex("IV",16,(u8*)IV,16);//
        //tuya_log_dumpHex("in_buf data",len,(u8*)(in_buf+17),len);//
        //    tuya_log_dumpHex("received encry data",air_recv_packet.recv_len,(u8*)air_recv_packet.recv_data,air_recv_packet.recv_len);//
        if(tuya_ble_aes128_cbc_decrypt(key,IV,(uint8_t *)(in_buf+17),len,out_buf))
        {
            //tuya_log_dumpHex("out_buf data",len,(u8*)out_buf,len);//
            *out_len = len;
            return 0;//成功
        }
        else
        {
            return 3;//解密失败
        }
    }
    else
    {

        return 4; //key计算失败
    }

}
