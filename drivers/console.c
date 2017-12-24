#include <common.h>
#include <console.h>

static uint16_t *video_memory_addr = (uint16_t *)0xB8000;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void move_curosr()
{
	uint16_t cursor_location = cursor_y * 80 + cursor_x;

	outb(0x3D4, 14);
	outb(0x3D5, (cursor_location >> 8) & 0xFF);
	outb(0x3D4, 15);
	outb(0x3D5, cursor_location & 0xFF);
}

static void scroll()
{
	uint8_t attrbyte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attrbyte << 8);

	if (cursor_y >= 25) {
		int i;

		for (i = 0 * 80; i < 24 * 80; i++) {
			video_memory_addr[i] = video_memory_addr[i+80];
		}

		for (i = 24 * 80; i < 25 * 80; i++) {
			video_memory_addr[i] = blank;
		}

		cursor_y = 24;
	}
}

void console_clear()
{
	uint8_t attrbyte = (0 << 4) | (15 & 0x0F);
	uint16_t blank = 0x20 | (attrbyte << 8);
	int i;

	for (i = 0; i < 80 * 25; i++) {
		video_memory_addr[i] = blank;
	}

	cursor_x = 0;
	cursor_y = 0;

	move_curosr();
}

void console_putc_color(char c, real_color_t back, real_color_t fore)
{
	uint8_t backcolor = (uint8_t)back;
	uint8_t forecolor = (uint8_t)fore;

	uint8_t attrbyte = (backcolor << 4) | (forecolor & 0x0F);
	uint16_t attribute = attrbyte << 8;

	if (c == 0x08 && cursor_x > 0) {
		cursor_x--;
	} else if (c == 0x09) {
		cursor_x = (cursor_x + 8) & ~(8-1);
	} else if (c == '\r') {
		cursor_x = 0;
	} else if (c == '\n') {
		cursor_x = 0;
		cursor_y++;
	} else if (c >= ' ') {
		video_memory_addr[cursor_y * 80 + cursor_x] = c | attribute;
		cursor_x++;
	}

	if (cursor_x >= 80) {
		cursor_x = 0;
		cursor_y++;
	}

	scroll();
	move_curosr();
}

void console_write(char *str)
{
	while (*str) {
		console_putc_color(*str++, rc_black, rc_white);
	}
}

void console_write_color(char *str,
	real_color_t back, real_color_t fore)
{
	while (*str) {
		console_putc_color(*str++, back, fore);
	}
}
