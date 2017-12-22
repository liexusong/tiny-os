#include <console.h>
#include <printk.h>
#include <math.h>

void printk(const char *format, ...)
{
	static char buf[10240];
	va_list args;
	int len;

	va_start(args, format);
	len = vsprintf(buf, format, args);
	va_end(args);

	if (len > 0) {
		console_write(buf);
	}
}

char *itoa(int num)
{
	static char buf[36];
	int tmp, last;
	int start, end;

	for (last = 0; ; last++) {
		buf[last] = do_div(num, 10) + '0';
		if (num == 0) {
			break;
		}
	}

	for (start = 0, end = last; 
		start < end; start++, end--) 
	{
		tmp = buf[start];
		buf[start] = buf[end];
		buf[end] = tmp;
	}

	buf[last+1] = '\0';

	return buf;
}

int vsprintf(char *buf, const char *format, va_list args)
{
	int state, i;
	char *str;

	for (i = 0, state = 0; *format; format++) {

		if (*format == '%') {
			state = 1;
			continue;
		}

		if (state) {
			switch (*format) {
			case 'd':
			case 'i':
				str = itoa(va_arg(args, int));
				for (; *str; str++) {
					buf[i++] = *str;
				}
				break;
			case 's':
				str = va_arg(args, char *);
				for (; *str; str++) {
					buf[i++] = *str;
				}
				break;
			case 'c':
				buf[i++] = va_arg(args, int);
				break;
			case '%':
				buf[i++] = '%';
				break;
			default:
				return -1;
			}

		} else {
			buf[i++] = *format;
		}

		state = 0;
	}

	buf[i] = '\0';

	return i;
}
