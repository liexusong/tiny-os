#include <types.h>

int kernel_start()
{
	uint8_t *input = (uint8_t *)0xB8000;
	uint8_t color = (0 << 4) | (15 & 0x0F);

	*input++ = (uint8_t)'H'; 
	*input++ = color;
	*input++ = (uint8_t)'e'; 
	*input++ = color;
	*input++ = (uint8_t)'l'; 
	*input++ = color;
	*input++ = (uint8_t)'l'; 
	*input++ = color;
	*input++ = (uint8_t)'o'; 
	*input++ = color;
	*input++ = (uint8_t)','; 
	*input++ = color;
	*input++ = (uint8_t)'T'; 
	*input++ = color;
	*input++ = (uint8_t)'i'; 
	*input++ = color;
	*input++ = (uint8_t)'n'; 
	*input++ = color;
	*input++ = (uint8_t)'y'; 
	*input++ = color;
	*input++ = (uint8_t)'O'; 
	*input++ = color;
	*input++ = (uint8_t)'S'; 
	*input++ = color;

	return 0;
}
