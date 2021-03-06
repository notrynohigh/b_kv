/**
 ****************************************************************************
 * MIT License
 * @file b_kv_port.c 
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
#include "b_kv_port.h"
#include <stdint.h>
#include <string.h>

uint8_t f_table[4][4096];





bKVS32 b_kv_port_erase(bKVU32 addr, bKVU32 n)
{
	uint8_t *p = (uint8_t *)f_table;
	n = n * 4096;
	memset(p + addr, 0, n);
    return 0;
}

bKVS32 b_kv_port_write(bKVU32 addr, bKVU8 *buf, bKVU32 len)
{
	uint8_t *p = (uint8_t *)f_table;
    memcpy(p + addr, buf, len);
    return 0;
}


bKVS32 b_kv_port_read(bKVU32 addr, bKVU8 *buf, bKVU32 len)
{
	uint8_t *p = (uint8_t *)f_table;
    memcpy(buf, p + addr, len);
    return 0;
}





