
#ifndef __MOUSE_H
#define __MOUSE_H

#include "interrupts.h"
#include "port.h"
#include "types.h"

void printf(const char*);

class MouseDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

  public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif
