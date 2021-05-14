#ifndef functions
#define functions

#include <stdarg.h>

typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

void clear();
void outb(uint16_t dest, uint16_t src);
uint8_t inb(uint16_t src);
void enable_cursor(int cursor_start, int cursor_end);
void update_cursor(int x, int y);
uint16_t get_cursor_position(void);
char* printnr(int nr);
void print(char *a, ...);
void scroll(int scroll);
void shellprint(char a);
#endif