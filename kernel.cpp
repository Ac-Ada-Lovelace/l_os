
#include "gdt.h"
#include "interrupts.h"
#include "types.h"

void flushScreen()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    for (int i = 0; i < 80 * 25; i++)
    {
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | ' ';
    }
}

void printf(const char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch (str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
                x++;
                break;
        }
        // 80 characters wide, 25 lines tall
        if (x >= 80)
        {
            x = 0;
            y++;
        }
        if (y >= 25)
        {
            for (y = 0; y < 25; y++)
            {
                for (x = 0; x < 80; x++)
                {
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                }
            }
            x = 0;
            y = 0;
        }
    }
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void* multiboot_structure, unsigned int /*multiboot_magic*/)
{
    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);

    interrupts.Activate();
    flushScreen();
    const char* welcome = "Hello World!\n";

    for (int i = 0; i < 5; i++)
    {
        printf(welcome);
    }

    while (1)
        ;
}
