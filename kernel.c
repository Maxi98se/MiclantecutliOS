#include <stdarg.h>
#include "kernel.h"
#include "functions.h"
#include "interrupts.h"

#define VGA_WIDTH 80

/*typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;*/


int _start()
{

    idt_init();

    clear();

    enable_cursor(0, 15);
    update_cursor(0, 0);

    print("Welcome to Mictlantecutli OS-%d.%d\n", 0, 4);
    print("---------------------------------\n\n");

    readmem(2); 

    while (1)
    {
        
    }

    return 0;
}