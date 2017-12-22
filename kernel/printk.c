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

void cprintk(real_color_t color, const char * format, ...)
{
	static char buf[10240];
	va_list args;
	int len;

	va_start(args, format);
	len = vsprintf(buf, format, args);
	va_end(args);

	if (len > 0) {
		console_write_color(buf, rc_black, color);
	}
}

char *itoa(int num, int base)
{
	static char buf[36];
	static char *hexc = "0123456789abcdef";
	int tmp, last;
	int start, end;

	for (last = 0; ; last++) {
		buf[last] = hexc[do_div(num, base)];
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
				str = itoa(va_arg(args, int), 10);
				for (; *str; str++) {
					buf[i++] = *str;
				}
				break;
			case 'x':
			case 'p':
				str = itoa(va_arg(args, int), 16);
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
