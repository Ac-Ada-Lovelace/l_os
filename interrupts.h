#ifndef __INTERRUPTS_H
#define __INTERRUPTS_H

#include "port.h"
#include "types.h"

class InterruptManager
{
  public:
    static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
    static void HandleInterruptRequest();
};

#endif
