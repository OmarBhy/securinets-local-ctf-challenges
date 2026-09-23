#pragma once
#include <stdint.h>

extern volatile uint32_t g_tick;

extern volatile uint8_t  g_auth_in_progress;
extern volatile uint8_t  g_auth_ok;
extern volatile uint32_t g_auth_start_tick;

extern volatile uint8_t  g_adhan_glitch;

// NEW: latched unlock flag (set by ISR on success)
extern volatile uint8_t  g_unlocked;

void systick_init(uint32_t reload);
