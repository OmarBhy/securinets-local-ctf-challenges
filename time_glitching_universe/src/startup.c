#include <stdint.h>

extern uint32_t _estack;
extern uint32_t _etext, _sdata, _edata, _sbss, _ebss;

int main(void);
void SysTick_Handler(void);

static void Default_Handler(void) {
    while (1) {}
}

void Reset_Handler(void) {
    // Copy .data
    uint32_t *src = &_etext;
    uint32_t *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;

    // Zero .bss
    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;

    main();
    while (1) {}
}

__attribute__((section(".isr_vector")))
void (* const gVectors[])(void) = {
    (void (*)(void))(&_estack),
    Reset_Handler,
    Default_Handler, // NMI
    Default_Handler, // HardFault
    Default_Handler, // MemManage
    Default_Handler, // BusFault
    Default_Handler, // UsageFault
    0,0,0,0,
    Default_Handler, // SVC
    Default_Handler, // DebugMon
    0,
    Default_Handler, // PendSV
    SysTick_Handler
};
