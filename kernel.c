#include <stdarg.h>
#include "kernel.h"
#include "functions.h"
#include "interrupts.h"

#define VGA_WIDTH 80

typedef unsigned long uint64_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

void shell(char kc)
{
        uint8_t sc = inb(0x60);
        if(!(sc & 1 << 7))
        {
        shellprint(kc);
        }
}

int _start()
{
    idt_init();

    clear();

    enable_cursor(0, 15);
    update_cursor(0, 0);

    print("Welcome to Mictlantecutli OS-%d.%d\n\n", 0, 4);
    print("--------------------------------------------------------------------------------");

    shellprint(0);

    while (1)
    {
        
    }

    return 0;
}