/**
 ****************************************************************************
 * MIT License
 * @file b_kv.c  
 * @version v0.0.2
 * Copyright (c) [2018-2019] [Bean  email: notrynohigh@outlook.com]
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************
 */
#include "../inc/b_kv.h"
#include "../port/b_kv_port.h"
#include "string.h"

static bKVU32 b_kv_code_table[B_KV_ITEM_MAX_NUMBER];
static bKVU32 b_kv_current_number = 0;
static bKVU8  b_kv_full_flag = 0;
#define bKV_PER_INFO_SIZE      (sizeof(b_kv_info_t))

static bKVU32 _b_kv_create_code(bKVU8 *pstr)
{
    bKVU32 u32_tmp = 0, i = 0, sum = 0;
    bKVU32 len = strlen((const char *)pstr);
    if(pstr == bKVNULL || len == 0)
    {
        return 0;
    }
    for(i = 0;i < len;i++)
    {
        sum += pstr[i] * (i + 1);
    }
    sum = sum / len;
    for(i = 0;i < len;i++)
    {
        u32_tmp += pstr[i] * sum; 
        u32_tmp ^= pstr[i];
    }
    return u32_tmp;
}



static bKVS32 _b_kv_search_code(bKVU32 code)
{
    bKVU32 i;
    if(code == 0)   
    {
        return B_KV_STA_ERR;
    }
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1] == code)
        {
            return (B_KV_ITEM_MAX_NUMBER - i - 1);
        }
    }
    return B_KV_STA_NO_ITEM;
}

static bKVS32 _b_kv_area_clear()
{
    return b_kv_port_erase(B_KV_INFO_SADDR, (B_KV_TOTAL_SIZE / B_KV_MIN_ERASE_UNIT));
}


static bKVS32 _b_kv_update_ctable()
{
    bKVU32 i = 0, addr = B_KV_INFO_SADDR;
    b_kv_info_t b_kv_info;
    memset(b_kv_code_table, 0, sizeof(b_kv_code_table));
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        b_kv_port_read(addr + (i * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
        if(b_kv_info.length == 0x0)
        {
            continue;
        }
        b_kv_code_table[i] = b_kv_info.code;
    }
    b_kv_current_number = 0;
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1] != 0)
        {
            break;
        }
    }   
    b_kv_current_number = B_KV_ITEM_MAX_NUMBER - i;  
    return B_KV_STA_OK;    
}




bKVS32 b_kv_init()
{
    return _b_kv_update_ctable();
}

static bKVS32 _b_kv_get_last_info(b_kv_info_t *pinfo)
{
    if(pinfo == bKVNULL)
    {
        return B_KV_STA_ERR;
    }
    if(b_kv_current_number == 0)
    {
        _b_kv_area_clear();
        pinfo->address = B_KV_VALUE_SADDR;
        pinfo->length = 0;
        return B_KV_STA_OK;
    }
    return b_kv_port_read(B_KV_INFO_SADDR + ((b_kv_current_number - 1) * bKV_PER_INFO_SIZE), (bKVU8 *)pinfo, bKV_PER_INFO_SIZE);
}


#define TMP_BUF_SIZE         16
#if ((B_KV_MIN_ERASE_UNIT % TMP_BUF_SIZE) != 0)
#error "TMP_BUF_SIZE ERROR ...."
#endif

static bKVS32 _b_kv_recycle()
{
    bKVU32 i = 0, j = 0, len, addr, t_index = 0;
    bKVU32 c_index = 0, v_addr = B_KV_VALUE_SADDR;
    b_kv_info_t b_kv_info;
    bKVU8 tmp_buf[TMP_BUF_SIZE];
    /** erase info and value */
    b_kv_port_erase(B_KV_INFO_SADDR, B_KV_INFO_SIZE / B_KV_MIN_ERASE_UNIT);
    b_kv_port_erase(B_KV_VALUE_SADDR, B_KV_VALUE_SIZE / B_KV_MIN_ERASE_UNIT);
    /** update code_table[] delete the same code */
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1] == 0)
        {
            continue;
        }
        for(j = (i + 1);j < B_KV_ITEM_MAX_NUMBER;j++)
        {
            if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - j - 1] == 0)
            {
                continue;
            }
            if(b_kv_code_table[B_KV_ITEM_MAX_NUMBER - j - 1] == b_kv_code_table[B_KV_ITEM_MAX_NUMBER - i - 1])
            {
                b_kv_code_table[B_KV_ITEM_MAX_NUMBER - j - 1] = 0x0;
            }
        }
    }
    /** rewrite value and info */
    for(c_index = 0;c_index < B_KV_ITEM_MAX_NUMBER;c_index++)
    {
        if(b_kv_code_table[c_index] != 0)
        {
            /**      update code_table[]      */
            if(t_index != c_index)
            {
                b_kv_code_table[t_index] = b_kv_code_table[c_index];
                b_kv_code_table[c_index] = 0;
            }
            //read b_kv_info from info_bak
            b_kv_port_read(B_KV_INFOBAK_SADDR + (c_index * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
            len = b_kv_info.length;    
            addr = b_kv_info.address;
            b_kv_info.address = v_addr;     //adjust address
            for(;len >= TMP_BUF_SIZE;)
            {
                b_kv_port_read(addr + B_KV_VALUE_SIZE, tmp_buf, TMP_BUF_SIZE);
                b_kv_port_write(v_addr, tmp_buf, TMP_BUF_SIZE);    //copy value from bak to new address
                v_addr += TMP_BUF_SIZE;
                addr += TMP_BUF_SIZE;
                len -= TMP_BUF_SIZE;
            }
            if(len > 0)
            {
                b_kv_port_read(addr + B_KV_VALUE_SIZE, tmp_buf, len);
                b_kv_port_write(v_addr, tmp_buf, len);
                v_addr += len;
            }
            /***   copy value finished    **/
            /***   write info             **/
            b_kv_port_write(B_KV_INFO_SADDR + (t_index * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
            t_index++;
        }
    }
    /**  update current_number  **/
    b_kv_current_number = t_index;
    if((b_kv_current_number == B_KV_ITEM_MAX_NUMBER) || 
        (v_addr >= (B_KV_VALUE_SADDR + B_KV_VALUE_SIZE - 5)))
    {
        b_kv_full_flag = 0x1;
    }
    else
    {
        b_kv_full_flag = 0x0;
    }
    /** erase bak and rewrite bak */
    b_kv_port_erase(B_KV_INFOBAK_SADDR, B_KV_INFOBAK_SIZE / B_KV_MIN_ERASE_UNIT);
    b_kv_port_erase(B_KV_VALUEBAK_SADDR, B_KV_VALUEBAK_SIZE / B_KV_MIN_ERASE_UNIT);
    for(i = 0;i < (B_KV_INFOBAK_SIZE / TMP_BUF_SIZE);i++)
    {
        b_kv_port_read(B_KV_INFO_SADDR + (i * TMP_BUF_SIZE), tmp_buf, TMP_BUF_SIZE);
        b_kv_port_write(B_KV_INFOBAK_SADDR + (i * TMP_BUF_SIZE), tmp_buf, TMP_BUF_SIZE);      
    }
    for(i = 0;i < (B_KV_VALUEBAK_SIZE / TMP_BUF_SIZE);i++)
    {
        b_kv_port_read(B_KV_VALUE_SADDR + (i * TMP_BUF_SIZE), tmp_buf, TMP_BUF_SIZE);
        b_kv_port_write(B_KV_VALUEBAK_SADDR + (i * TMP_BUF_SIZE), tmp_buf, TMP_BUF_SIZE);      
    }   
    return B_KV_STA_OK;
}


static bKVS32 _b_kv_add_new(bKVU32 code, bKVU8* pbuf, bKVU32 len)
{
    b_kv_info_t b_kv_info, b_kv_last_info;
    if(b_kv_full_flag == 0x1)
    {
        return B_KV_STA_FULL;
    }
    _b_kv_get_last_info(&b_kv_last_info);
    if((b_kv_current_number == B_KV_ITEM_MAX_NUMBER) || ((b_kv_info.address + b_kv_info.length) >= B_KV_VALUEBAK_SADDR))
    {
        _b_kv_recycle();
        if(b_kv_full_flag == 0x1)
        {
            return B_KV_STA_FULL;
        } 
    } 
    _b_kv_get_last_info(&b_kv_last_info);
    b_kv_info.code = code;
    b_kv_info.address = b_kv_last_info.address + b_kv_last_info.length;
    b_kv_info.length = len;
    b_kv_info.reserved = 0xffffffff;
    
    if((B_KV_STA_OK == b_kv_port_write(b_kv_info.address, pbuf, len))
        && (B_KV_STA_OK == b_kv_port_write(b_kv_info.address + B_KV_VALUE_SIZE, pbuf, len)))
    {
        if((B_KV_STA_OK == b_kv_port_write(B_KV_INFO_SADDR + (b_kv_current_number * bKV_PER_INFO_SIZE), (bKVU8*)&b_kv_info, bKV_PER_INFO_SIZE))
            && (B_KV_STA_OK == b_kv_port_write(B_KV_INFOBAK_SADDR + (b_kv_current_number * bKV_PER_INFO_SIZE), (bKVU8*)&b_kv_info, bKV_PER_INFO_SIZE)))
        {
            b_kv_code_table[b_kv_current_number] = code;
            b_kv_current_number++;
            return B_KV_STA_OK;
        }
    }
    b_kv_code_table[b_kv_current_number] = 0X0;
    b_kv_current_number++;
    return B_KV_STA_ERR;
}




bKVS32 b_kv_add_value(bKVU8 *pname, bKVU8* pbuf, bKVU32 len)
{
    bKVU32 code_tmp = 0;
    if(pname == bKVNULL || pbuf == bKVNULL || len == 0)
    {
        return B_KV_STA_ERR;
    }
    code_tmp = _b_kv_create_code(pname);
    if(code_tmp == 0)
    {
        return B_KV_STA_ERR;
    }
    return _b_kv_add_new(code_tmp, pbuf, len);
}


bKVS32 b_kv_get_value(bKVU8 *pname, bKVU8* pbuf, bKVU32* len)
{
    bKVS32 index = -1;
    bKVU32 code_tmp = 0;
    b_kv_info_t b_kv_info;
    if(pname == bKVNULL || pbuf == bKVNULL || len == 0)
    {
        return B_KV_STA_ERR;
    }
    code_tmp = _b_kv_create_code(pname); 
    if(code_tmp == 0)
    {
        return B_KV_STA_ERR;
    }    
    index = _b_kv_search_code(code_tmp);
    if(index >= 0)
    {
        if(B_KV_STA_OK == b_kv_port_read(B_KV_INFO_SADDR + (index * bKV_PER_INFO_SIZE), (bKVU8*)&b_kv_info, bKV_PER_INFO_SIZE))
        {
            if(B_KV_STA_OK == b_kv_port_read(b_kv_info.address, pbuf, b_kv_info.length))
            {
                *len = b_kv_info.length;
                return B_KV_STA_OK;
            }
        }
    }
    return B_KV_STA_NO_ITEM;
}



bKVS32 b_kv_delete_value(bKVU8 *pname)
{
    bKVU32 code_tmp = 0, i;
    b_kv_info_t b_kv_info;
    if(pname == bKVNULL)
    {
        return B_KV_STA_ERR;
    }
    code_tmp = _b_kv_create_code(pname); 
    if(code_tmp == 0)
    {
        return B_KV_STA_ERR;
    }   
    memset(&b_kv_info, 0, sizeof(b_kv_info_t));
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
        if(b_kv_code_table[i] == code_tmp)
        {
            b_kv_code_table[i] = 0;
            b_kv_port_write(B_KV_INFO_SADDR + (i * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
            b_kv_port_write(B_KV_INFOBAK_SADDR + (i * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
        }
    }
    if(b_kv_full_flag == 0x1)
    {
        b_kv_full_flag = 0;
    }
    return B_KV_STA_OK;
}












