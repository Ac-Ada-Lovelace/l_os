#include "mouse.h"

MouseDriver::MouseDriver(InterruptManager* manager) : InterruptHandler(manager, 0x2C), dataport(0x60), commandport(0x64)
{
    offset = 0;
    buttons = 0;

    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    VideoMemory[80 * 12 + 40] = (VideoMemory[80 * 12 + 40] & 0xF000) >> 4 | (VideoMemory[80 * 12 + 40] & 0x0F00) << 4 | (VideoMemory[80 * 12 + 40] & 0x00FF);

    commandport.Write(0xA8);  // Enable the interrupts
    commandport.Write(0x20);  // Command 0x20 = Read the controller command byte

    uint8_t status = dataport.Read() | 2;

    commandport.Write(0x60);  // Command 0x60 = Set controller command byte
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);

    dataport.Read();
}

MouseDriver::~MouseDriver() {}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();

    if (!(status & 0x20))
    {
        return esp;
    }

    static int8_t x = 0, y = 0;

    uint8_t key = dataport.Read();

    buffer[offset] = key;
    offset = (offset + 1) % 3;

    if (offset == 0)
    {

        static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += buffer[1];
        x < 0 ? x = 0 : x;
        x >= 80 ? x = 79 : x;

        y -= buffer[2];
        y < 0 ? y = 0 : y;
        y >= 25 ? y = 24 : y;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    return esp;
}
