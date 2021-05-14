#ifndef functions
#define functions

#include <stdarg.h>
#include <stdint.h>

/*typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;*/

struct memorymap
    {
        uint64_t base;
        uint64_t lenght;
        uint32_t type;
        uint32_t acpi;
    };

struct pos
{
  uint16_t x;
  uint16_t y;
  uint16_t pos;
};

struct memory
{
    uint32_t value;
    char *type;
};



void clear();
void outb(uint16_t dest, uint16_t src);
uint8_t inb(uint16_t src);
void enable_cursor(int cursor_start, int cursor_end);
void update_cursor(int x, int y);
struct pos get_cursor_position(void);
char* printnr(uint32_t nr, char format);
void print(char *a, ...);
void scroll(int scroll);
void shellprint(char a);
struct memory memoryformat(uint64_t mem);
struct memory readmem(uint8_t doprint);

#endif