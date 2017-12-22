#include <string.h>

void memcpy(uint8_t *dst, uint8_t *src, uint32_t len)
{
	for (; len > 0; len--) {
		*dst++ = *src++;
	}
}

void memset(void *dst, uint8_t value, uint32_t len)
{
	uint8_t *_dst = dst;

	for (; len > 0; len--) {
		*_dst++ = value;
	}
}

void bzero(void *dst, uint32_t len)
{
	memset(dst, 0, len);
}

int strcmp(const char *dst, const char *src)
{
	while (1) {
		if (*dst && *src) {
			if (*dst > *src) {
				return 1;
			} else if (*dst < *src) {
				return -1;
			}
		} else if (*dst) {
			return 1;
		} else if (*src) {
			return -1;
		} else {
			break;
		}

		dst++;
		src++;
	}

	return 0;
}

int strlen(const char *str)
{
	int i;

	for (i = 0; str[i]; i++);

	return i;
}

char *strcpy(char *dst, const char *src)
{
	char *ret = dst;

	while (*src) {
		*dst++ = *src++;
	}

	return ret;
}
