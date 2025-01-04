# As title said

```cpp
// previous code
static uint32_t handleInterrupt(uint8_t interruptNumber, uint32_t esp);
// I uppercase h to H
static uint32_t HandleInterrupt(uint8_t interruptNumber, uint32_t esp);
```

and then the code in interruptstubs.s

```asm
.macro HandleException num
.global _ZN16InterruptManager16handleException\num\()Ev
    movb $\num, (interruptnumber)
    jmp interrupt_bottom
.endm
```

\_ZN16InterruptManager16handleException not change so it will not work. I have to change it to \_ZN16InterruptManager16HandleException

