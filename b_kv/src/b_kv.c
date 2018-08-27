#include "b_kv.h"
#include "../port/b_kv_port.h"
#include "string.h"

static bKVU32 b_kv_code_table[B_KV_ITEM_MAX_NUMBER];


#define bKV_PER_INFO_SIZE      (sizeof(b_kv_info_t))




static bKVU32 _b_kv_str2u32(bKVU8 *pstr)
{
	bKVU32 u32_tmp = 0, i = 0, sum = 0;
	bKVU32 len = strlen(pstr);
	if(pstr == bFDB_NULL || len == 0)
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
    if(code == 0)	
	
}








bKVS32 b_kv_init()
{
    bKVU32 i = 0, addr = B_KV_START_ADDRESS;
	b_kv_info_t b_kv_info;
    memset(b_kv_code_table, 0, sizeof(b_kv_code_table));
    for(i = 0;i < B_KV_ITEM_MAX_NUMBER;i++)
    {
		b_kv_port_read(addr + (i * bKV_PER_INFO_SIZE), (bKVU8 *)&b_kv_info, bKV_PER_INFO_SIZE);
        if(b_kv_info.length == 0xffffffff)
		{
			continue;
		}
		b_kv_code_table[i] = b_kv_info.code;
    }
}



bKVS32 b_kv_add_value(bKVU8 *pname, bKVU32 value)
{
    
    
}



bKVS32 b_kv_add_string(bKVU8 *pname, bKVU8 *pstr);
{
    
    
}

bKVS32 b_kv_get_value(bKVU8 *pname, bKVU32 *value)
{
    
    
}


bKVS32 b_kv_get_string(bKVU8 *pname, bKVU8 *pstr)
{
    
    
}

















