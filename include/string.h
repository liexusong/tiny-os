#ifndef __STRING_H
#define __STRING_H

#include <types.h>

void memcpy(uint8_t *dst, uint8_t *src, uint32_t len);
void memset(void *dst, uint8_t value, uint32_t len);
void bzero(void *dst, uint32_t len);
int strcmp(const char *dst, const char *src);
int strlen(const char *str);
char *strcpy(char *dst, const char *src);

#endif
