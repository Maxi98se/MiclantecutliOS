#include "functions.h"
#include <stdarg.h>

typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;

#define VGA_WIDTH 80

void clear()
{
    char* vidmem=(char*)0xb8000;
    for (int i = 0; i < 0xFFF; i+=2)
    {
            vidmem[i] = 0x20;
            vidmem[i+1] = 0x0a;
    }
}

void outb(uint16_t dest, uint16_t src)
{
  asm("outb %%al, %[ret]"
      :
      : [port]"a" (src), [ret]"d" (dest)
      );
}

uint8_t inb(uint16_t src)
{
  uint8_t dest;
  asm("inb %[port], %[ret];"
      : [ret]"=a" (dest)
      : [port]"d" (src)
      );
  return dest;
}

void enable_cursor(int cursor_start, int cursor_end)
{
  outb(0x3D4, 0x0A);
  outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

  outb(0x3D4, 0x0B);
  outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);

 }

void update_cursor(int x, int y)
{
  uint16_t pos = (y * VGA_WIDTH) + x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t) (pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));

}

uint16_t get_cursor_position(void)
{
    uint16_t pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16_t)inb(0x3D5)) << 8;
    return pos;
}



char* printnr(int nr)
{
  int div=0;
  int size=0;

  if(nr==0)
  {
    return "0";
  }

  for(int i=1; i <= nr; i=i*10)
  {
    div=i;
    size++;
  }

  char nrstring[size];

  for(int i=0; i < size; i++)
  {
    nrstring[i] = (nr/div)+'0';
    nr=nr-((nr/div)*div);
    div=div/10;
  }

  char* nrstring2=nrstring;
  return nrstring2;
}

void print(char *a, ...)
{
    va_list ap;
    int pos = get_cursor_position();
    char *vidmem=(char*)0xb8000;
    int varnr=1;

/*    if(pos>=1920)
        {
          pos=1920;
          scroll(1);
        }*/

    for(unsigned int i = 0; a[i] != 0; i++)
    {
      if(pos>=1920)
        {
          pos=1840;
          scroll(1);
        }
      if(a[i]=='%')
      {
            if(a[i+1]=='d')
            {
                va_start(ap, varnr);
                char* nrstring;
                for(int gg=0; gg < varnr; gg++)
                {
                    nrstring = printnr(va_arg(ap, int));
                }
                for(unsigned int g = 0; nrstring[g] != 0; g++)
                {
                  vidmem[pos*2] = nrstring[g];
                  pos++;
                }
                va_end(ap);
            }
            else if(a[i+1]=='c')
            {
                va_start(ap, varnr);
                char nrstring;
                nrstring = va_arg(ap, int);
                if(nrstring=='\n')
                {
                  int posn=0;
                  for(int g = 0; pos >= g; g=g+80) 
                  {
                    posn=g+80;
                  }
                  pos=posn;
                }
                else if(nrstring=='\b')
                {
                  vidmem[(pos-1)*2]=' ';
                  pos--;
                }
                else
                {
                vidmem[pos*2] = nrstring;
                pos++;
                }
                va_end(ap);
            }
            else if(a[i+1]=='\b')
            {
              vidmem[(pos-1)*2]=' ';
              pos--;
            }
            i++;
            varnr++;

      }
      else if(a[i]=='\n')
      {
        int posn=0;
        for(int g = 0; pos > g; g=g+80) 
        {
          posn=g+80;
        }
        pos=posn;
      }
      else
      {
            vidmem[pos*2] = a[i];
            pos++;
      }
    }
    update_cursor(pos,0);
}

void scroll(int scroll)
{
  char *vidmem=(char*)0xb8000;
  for (int count = 0; count < scroll; count++)
  {    
  for (int i = 0; i <= 2000; i++)
  {
    vidmem[i*2]=vidmem[(80+i)*2];
  }
}
}