#ifndef __B_KV_PORT_H__
#define __B_KV_PORT_H__


#include "b_kv_config.h"






bKVS32 b_kv_port_erase(bKVU32 addr, bKVU32 n);
bKVS32 b_kv_port_write(bKVU32 addr, bKVU8 *buf, bKVU32 len);
bKVS32 b_kv_port_read(bKVU32 addr, bKVU8 *buf, bKVU32 len);



#endif


