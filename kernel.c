#include <stdarg.h>
#include "functions.h"
#include "interrupts.h"

#define VGA_WIDTH 80

typedef unsigned long uint64_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

int _start()
{
    idt_init();

    clear();

    enable_cursor(0, 15);
    update_cursor(24, 10);

    print("Welcome to Mictlantecutli OS-%d.%d\n", 0, 4);

    update_cursor(0, 0);

    while (1)
    {
        
    }

    return 0;
}
