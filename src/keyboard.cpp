
#include "keyboard.h"

KeyboardDriver::KeyboardDriver(InterruptManager* manager) : InterruptHandler(manager, 0x21), dataport(0x60), commandport(0x64) {
    printf("Keyboard driver constructor\n");
}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::Activate()
{
    printf("Keyboard driver activated\n");
    while (commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xae);  // activate interrupts
    commandport.Write(0x20);  // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60);  // command 0x60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);
}

void printf(const char*);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();

    static bool shift = false;
    static bool caps = false;

    if (key < 0x80)
    {
        switch (key)
        {
            case 0x2A:  // Left Shift Pressed
            case 0x36:  // Right Shift Pressed
                shift = true;
                break;
            case 0x3A:  // Caps Lock
                caps = !caps;
                break;
            case 0x02:
                printf(shift ^ caps ? "!" : "1");
                break;
            case 0x03:
                printf(shift ? "@" : "2");
                break;
            case 0x04:
                printf(shift ? "#" : "3");
                break;
            case 0x05:
                printf(shift ? "$" : "4");
                break;
            case 0x06:
                printf(shift ? "%" : "5");
                break;
            case 0x07:
                printf(shift ? "^" : "6");
                break;
            case 0x08:
                printf(shift ? "&" : "7");
                break;
            case 0x09:
                printf(shift ? "*" : "8");
                break;
            case 0x0A:
                printf(shift ? "(" : "9");
                break;
            case 0x0B:
                printf(shift ? ")" : "0");
                break;
            case 0x10:
                printf(shift ^ caps ? "Q" : "q");
                break;
            case 0x11:
                printf(shift ^ caps ? "W" : "w");
                break;
            case 0x12:
                printf(shift ^ caps ? "E" : "e");
                break;
            case 0x13:
                printf(shift ^ caps ? "R" : "r");
                break;
            case 0x14:
                printf(shift ^ caps ? "T" : "t");
                break;
            case 0x15:
                printf(shift ^ caps ? "Z" : "z");
                break;
            case 0x16:
                printf(shift ^ caps ? "U" : "u");
                break;
            case 0x17:
                printf(shift ^ caps ? "I" : "i");
                break;
            case 0x18:
                printf(shift ^ caps ? "O" : "o");
                break;
            case 0x19:
                printf(shift ^ caps ? "P" : "p");
                break;
            case 0x1E:
                printf(shift ^ caps ? "A" : "a");
                break;
            case 0x1F:
                printf(shift ^ caps ? "S" : "s");
                break;
            case 0x20:
                printf(shift ^ caps ? "D" : "d");
                break;
            case 0x21:
                printf(shift ^ caps ? "F" : "f");
                break;
            case 0x22:
                printf(shift ^ caps ? "G" : "g");
                break;
            case 0x23:
                printf(shift ^ caps ? "H" : "h");
                break;
            case 0x24:
                printf(shift ^ caps ? "J" : "j");
                break;
            case 0x25:
                printf(shift ^ caps ? "K" : "k");
                break;
            case 0x26:
                printf(shift ^ caps ? "L" : "l");
                break;
            case 0x2C:
                printf(shift ^ caps ? "Y" : "y");
                break;
            case 0x2D:
                printf(shift ^ caps ? "X" : "x");
                break;
            case 0x2E:
                printf(shift ^ caps ? "C" : "c");
                break;
            case 0x2F:
                printf(shift ^ caps ? "V" : "v");
                break;
            case 0x30:
                printf(shift ^ caps ? "B" : "b");
                break;
            case 0x31:
                printf(shift ^ caps ? "N" : "n");
                break;
            case 0x32:
                printf(shift ^ caps ? "M" : "m");
                break;
            case 0x33:
                printf(shift ? "<" : ",");
                break;
            case 0x34:
                printf(shift ? ">" : ".");
                break;
            case 0x35:
                printf(shift ? "_" : "-");
                break;
            case 0x1C:
                printf("\n");
                break;
            case 0x39:
                printf(" ");
                break;
            default: {
                char* foo = "KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";
                foo[11] = hex[(key >> 4) & 0xF];
                foo[12] = hex[key & 0xF];
                printf(foo);
                break;
            }
        }
    }
    else
    {
        switch (key)
        {
            case 0xAA:  // Left Shift Released
            case 0xB6:  // Right Shift Released
                shift = false;
                break;

            default:

                break;
        }
    }
    return esp;
}
