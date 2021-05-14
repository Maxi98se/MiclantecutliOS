#ifndef kernel
#define kernel

typedef unsigned long uint64_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

extern int keyboardswitch;

void shell(char kc);
int _start();

#endif