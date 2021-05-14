#include "functions.h"
#include <stdarg.h>

/*typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
typedef unsigned short int uint16_t;
typedef unsigned char uint8_t;*/

#define VGA_WIDTH 80

uint8_t lastshell=30;
char *commandstr;



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
  uint16_t cache = (y * VGA_WIDTH) + x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t) (cache & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t) ((cache >> 8) & 0xFF));

}

struct pos get_cursor_position(void)
{
    struct pos posn;
    posn.pos=0;
    posn.x=0;
    posn.y=0;

    outb(0x3D4, 0x0F);
    posn.pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    posn.pos |= ((uint16_t)inb(0x3D5)) << 8;

    posn.x = (posn.pos % 80);
    posn.y = (posn.pos - posn.x)/80; 

    return posn;
}

struct memory readmem(uint8_t doprint)
{
    char *memaddress = (char*)0x7e00;

    struct memorymap *memmap;
    memmap = 0x7e04;

if (doprint >= 2)
{
    print("\nBase Address        | Length            | Type              \n");
    print("------------------------------------------------------------\n");
}

    uint64_t ram=0;
    uint64_t usableram=0;

    for (int i = 0; i < memaddress[0]; i++)
    {
      if(doprint >= 2)
      {
        print("%x", memmap[i].base);
        update_cursor(20,get_cursor_position().y);
        print("| %x", memmap[i].lenght);
        update_cursor(40,get_cursor_position().y);
        print("| %x", memmap[i].type);
        print("\n");
      }
        if (memmap[i].type == 1)
        {
            usableram += memmap[i].lenght;
        }
    }
    ram = (memmap[memaddress[0]-1].base)+(memmap[memaddress[0]-1].lenght);
if(doprint >= 1)
{
    print("\nMemory: %d", memoryformat(ram));
    print(memoryformat(ram).type);
    print("\nUsable Memory: %d", memoryformat(usableram).value);
    print(memoryformat(usableram).type);
    print("\n");
}
}

struct memory memoryformat(uint64_t mem)
{
  struct memory cache;
  cache.type=0;
  cache.value=0;

    if(mem >= 1<<30)
    {
      cache.type="GB";
      cache.value=mem>>30;
    }
    else if (mem >= 1<<20)
    {
      cache.type="MB";
      cache.value=mem>>20;
    }
    else if (mem >= 1<<10)
    {
      cache.type="KB";
      cache.value=mem>>10;
    }
    else
    {
      cache.type="B";
      cache.value=mem;
    }
   return cache;
    
}

char* printnr(uint32_t nr, char format)
{
  uint32_t div=0;
  uint32_t size=0;
  uint32_t divf=10;
  
  switch(format)
  {
    case 'd':
      divf=10;
      break;
    case 'x':
      divf=16;
      break;
    default:
    break;
  }
  

  if(nr==0)
  {
    return "0";
  }

  for(uint64_t i=1; i <= nr; i=i*divf)
  {
    div=i;
    size++;
  }

  char nrstring[size];

  for(uint64_t i=0; i < size; i++)
  {
    if(nr/div<=9)
    {
    nrstring[i] = (nr/div)+'0';
    }
    else
    {
      nrstring[i] = (nr/div+'W');
    }
    nr=nr-((nr/div)*div);
    div=div/divf;
  }

  nrstring[size]=0;

  char* nrstring2=nrstring;
  return nrstring2;
}

//Nahuatl
void shellprint(char a)
{
  int pos = get_cursor_position().pos;
  //lastshell=pos;
  char *vidmem=(char*)0xb8000;
  for (int i = 0; i <= 24; i++)
  {
    if(pos == i*80 && lastshell == 30)
    {
      print("<root@Nahuatl> ");
      lastshell=get_cursor_position().pos-1;
    }
  }
  if(a=='\n')
  {
    print("\n");
    print(commandstr);
    print("\n");

    commandstr=0;
    lastshell=30;
    shellprint(0);
    print("%d-%d",get_cursor_position().pos,lastshell);
  }
  else if(a!=0)
  {
    commandstr[pos-lastshell-1]=a;
    commandstr[pos-lastshell]=0;
    print("%c", a);

  }  
}


void print(char *a, ...)
{
    va_list ap;
    uint16_t posn = get_cursor_position().pos;
    char *vidmem=(char*)0xb8000;
    uint64_t varnr=1;

    //vidmem[posn]=a[0];

/*    if(posn>=1920)
        {
          posn=1920;
          scroll(1);
        }*/

    for(unsigned int i = 0; a[i] != 0; i++)
    {
      if(posn>=1920)
        {
          posn=1840;
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
                    nrstring = printnr(va_arg(ap, uint32_t),'d');
                }
                for(unsigned int g = 0; nrstring[g] != 0; g++)
                {
                  vidmem[posn*2] = nrstring[g];
                  posn++;
                }
                va_end(ap);
            }
            else if(a[i+1]=='x')
            {
                va_start(ap, varnr);
                char* nrstring;
                for(int gg=0; gg < varnr; gg++)
                {
                    nrstring = printnr(va_arg(ap, uint32_t),'x');
                }
                for(unsigned int g = 0; nrstring[g] != 0; g++)
                {
                  vidmem[posn*2] = nrstring[g];
                  posn++;
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
                          posn = ((((posn - (posn%80))/80)+1)*VGA_WIDTH); 
                }
                else if(nrstring=='\b')
                {
                  if(posn-1 > lastshell)
                  {
                  vidmem[(posn-1)*2]=' ';
                  posn--;
                  }
                }
                else
                {
                vidmem[posn*2] = nrstring;
                posn++;
                }
                va_end(ap);
            }
            i++;
            varnr++;

      }
      else if(a[i]=='\b')
                {
                  if(posn-1 > lastshell)
                  {
                  vidmem[(posn-1)*2]=' ';
                  posn--;
                  }
                }
      else if(a[i]=='\n')
      {
        //vidmem = vidmem - (vidmem-(char*)0xb8000)%(160)
        /*int posn=0;
        for(int g = 0; pos >= g; g=g+80) 
        {
          posn=g+80;
        }
        pos=posn;*/
        //posn = ((get_cursor_position().y + 1) * VGA_WIDTH);

        posn = ((((posn - (posn%80))/80)+1)*VGA_WIDTH); 
      }
      else
      {
            vidmem[posn*2] = a[i];
            posn++;
      }
    }
    update_cursor(posn,0);
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