#include "driver.h"
#include "gdt.h"
#include "interrupts.h"
#include "keyboard.h"
#include "mouse.h"

void flush()
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

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

        if (x >= 80)
        {
            x = 0;
            y++;
        }

        if (y >= 25)
        {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
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

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    flush();

    const char* hello = "Hello, World!\n";

    printf(hello);

    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    printf("Initializing hardware, stage 1\n");

    Driver::DriverManager drvManager;

    // KeyboardDriver keyboard(&interrupts);
    // drvManager.AddDriver(&keyboard);
    MouseDriver mouse(&interrupts);
    drvManager.AddDriver(&mouse);

    printf("Initializing Hardware, Stage 2\n");
    drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");
    interrupts.Activate();

    while (1)
        ;
}
