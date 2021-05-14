void print(char *a)
{
    char *vidmem=(char*)0xb8000;
    for(unsigned int i = 0; a[i] != 0; i++)
    {
            vidmem[i*2] = a[i];
        }
}

int main()
{
    char* vidmem=(char*)0xb8000;
    for (int i = 0; i < 0xFFF; i+=2)
    {
            vidmem[i] = 0x20;
            vidmem[i+1] = 0x0a;
        }

    print("Welcome to Mictlantecutli OS-0.1");
    
    return 0;
}

