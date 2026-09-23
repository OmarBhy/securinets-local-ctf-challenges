#include "systick.h"

#define SYST_CSR (*(volatile uint32_t*)0xE000E010u)
#define SYST_RVR (*(volatile uint32_t*)0xE000E014u)
#define SYST_CVR (*(volatile uint32_t*)0xE000E018u)

volatile uint32_t g_tick = 0;

volatile uint8_t  g_auth_in_progress = 0;
volatile uint8_t  g_auth_ok = 0;
volatile uint32_t g_auth_start_tick = 0;

volatile uint8_t  g_adhan_glitch = 0;

// NEW: latched unlock state
volatile uint8_t  g_unlocked = 0;

#define ADHAN_PHASE 0x5Eu

void systick_init(uint32_t reload) {
    SYST_RVR = reload;
    SYST_CVR = 0;
    // enable, tickint, clksource=cpu
    SYST_CSR = (1u<<0) | (1u<<1) | (1u<<2);
}

void SysTick_Handler(void) {
    g_tick++;

    // glitch condition: only in tiny window during AUTH
    if (g_auth_in_progress) {
        uint32_t dt = g_tick - g_auth_start_tick;
        if (dt <= 3 && ((g_tick & 0xFFu) == ADHAN_PHASE)) {
            g_auth_ok = 1;       // injected "fault"
            g_adhan_glitch = 1;  // visible via STATUS
            g_unlocked = 1;      // LATCH unlock forever for this connection/session
        }
    }
}
