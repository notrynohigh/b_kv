#ifndef __B_KV_H__
#define __B_KV_H__



#include "../port/b_kv_config.h"







#pragma pack(1)


typedef struct
{
	bKVU32  code;
	bKVU32  address;
	bKVU32  length;
	bKVU32  reserved;
}b_kv_info_t;






#pragma pack()





bKVS32 b_kv_init(void);

bKVS32 b_kv_add_value(bKVU8 *pname, bKVU32 value);
bKVS32 b_kv_add_string(bKVU8 *pname, bKVU8 *pstr);

bKVS32 b_kv_get_value(bKVU8 *pname, bKVU32 *value);
bKVS32 b_kv_get_string(bKVU8 *pname, bKVU8 *pstr);



#endif



