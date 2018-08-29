#include "./b_kv/inc/b_kv.h"
#include "./b_kv/port/b_kv_config.h"
#include <stdio.h>
#include <stdint.h>
int main()
{
	int i = 0,test,len;
	int ret = 0;
	printf("hello world\n\r");
	b_kv_init();
	for(i = 0;i < 200;i++)
	{
	    ret = b_kv_add_value((uint8_t *)"notrynohigh", (uint8_t *)&i, sizeof(int));
		if(ret < 0)
		{
			printf("i = %d\n\r", i);
			break;
		}
	}
	b_kv_get_value((uint8_t *)"notrynohigh", (uint8_t *)&test, &len);
	printf("test = %d\n\r", test);
	return 0;
	
}