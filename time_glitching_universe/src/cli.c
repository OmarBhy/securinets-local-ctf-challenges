#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "systick.h"



// Public build: FLAG_STR not defined => no real flag in binary.
// Remote build: compile with -DFLAG_STR="\"CyberQuest{9Rib_i2adhan}\""
#ifndef FLAG_STR
#define FLAG_STR ((const char*)0)
#endif

// ---------------- line input ----------------
static int read_line(char *buf, int max) {
    int i = 0;
    while (i < max - 1) {
        char c = uart_getc();
        if (c == '\r') continue;
        if (c == '\n') break;
        buf[i++] = c;
    }
    buf[i] = 0;
    return i;
}

// ---------------- slow auth work ----------------
static void slow_work(volatile uint32_t n) {
    while (n--) __asm volatile ("nop");
}

static int slow_check_password(const char *pw) {
    const char *real = "ramadhan_portal";
    size_t n1 = strlen(pw), n2 = strlen(real);
    size_t n = (n1 > n2) ? n1 : n2;

    int ok = 1;
    for (size_t i = 0; i < n; i++) {
        char a = (i < n1) ? pw[i] : 0;
        char b = (i < n2) ? real[i] : 0;
        if (a != b) ok = 0;

        // makes AUTH slow => interrupt can hit inside
        slow_work(4500);
    }
    return ok;
}

// ---------------- small printing helpers (no printf) ----------------
static void put_u32_dec(uint32_t x) {
    char rev[16];
    int k = 0;
    if (x == 0) { uart_putc('0'); return; }
    while (x && k < (int)sizeof(rev)) { rev[k++] = '0' + (x % 10); x /= 10; }
    while (k--) uart_putc(rev[k]);
}

static void put_u8_hex(uint8_t v) {
    const char h[] = "0123456789abcdef";
    uart_putc(h[(v >> 4) & 0xF]);
    uart_putc(h[v & 0xF]);
}

// ---------------- case-insensitive command parsing ----------------
static char to_upper(char c) {
    if (c >= 'a' && c <= 'z') return (char)(c - 'a' + 'A');
    return c;
}

// Compare cmd with uppercase literal (HELP/STATUS/AUTH/IFTAR)
static int cmd_eq(const char *cmd, const char *lit_upper) {
    while (*cmd && *lit_upper) {
        if (to_upper(*cmd) != *lit_upper) return 0;
        cmd++; lit_upper++;
    }
    return (*cmd == 0 && *lit_upper == 0);
}

// Split line into cmd and arg (modifies line by inserting '\0')
static void split_cmd(char *line, char **cmd_out, char **arg_out) {
    while (*line == ' ') line++;

    char *cmd = line;
    while (*line && *line != ' ') line++;

    if (*line) { *line = 0; line++; }

    while (*line == ' ') line++;
    char *arg = (*line) ? line : 0;

    *cmd_out = cmd;
    *arg_out = arg;
}

// ---------------- commands ----------------
static void cmd_help(void) {
    uart_puts("HELP\nSTATUS\nAUTH <pw>\nIFTAR\n");
}

static void cmd_status(void) {
    uint32_t t = g_tick;
    uint8_t phase = (uint8_t)(t & 0xFFu);
    const uint8_t ADHAN_PHASE = 0x5E;
    uint8_t adhan_in = (uint8_t)((ADHAN_PHASE - phase) & 0xFFu);

    uart_puts("tick=");
    put_u32_dec(t);
    uart_puts(" phase=0x");
    put_u8_hex(phase);
    uart_puts(" adhan_in=0x");
    put_u8_hex(adhan_in);
    uart_puts(" muadhin=");
    uart_putc(g_unlocked ? '1' : '0');
    uart_puts(" glitch=");
    uart_putc(g_adhan_glitch ? '1' : '0');
    uart_puts("\n");
}

static void cmd_auth(const char *pw) {
    if (!g_unlocked) g_adhan_glitch = 0;
    g_auth_ok = 0;
    g_auth_in_progress = 1;
    g_auth_start_tick = g_tick;

    int ok = slow_check_password(pw);
    if (ok) g_auth_ok = 1;

    g_auth_in_progress = 0;

    // Vulnerable decision (race/TOCTOU with ISR)
    if (g_auth_ok) {
        g_unlocked = 1;
        if (g_adhan_glitch) uart_puts("Adhan soon... time anomaly! Mu'adhin mode enabled.\n");
        else uart_puts("Authenticated.\n");
    } else {
        uart_puts("Auth failed.\n");
    }
}

static void cmd_iftar(void) {
    if (!g_unlocked) {
       uart_puts("IFTAR not ready. The adhan will be soon.\n");
       return;
    }

    if (FLAG_STR) {
        uart_puts(FLAG_STR);
        uart_puts("\n");
    } else {
        uart_puts("IFTAR is only served in the real universe portal.\n");
        uart_puts("Connect to the remote device via netcat.\n");
    }
}

// ---------------- main CLI loop ----------------
void cli_run(void) {
    char buf[128];

    uart_puts("=== Ramadhan Time Glitching Universe ===\n");
    uart_puts("the adhan will be soon\n");
    uart_puts("Type HELP.\n");

    while (1) {
        uart_puts("> ");
        read_line(buf, sizeof(buf));

        char *cmd = 0, *arg = 0;
        split_cmd(buf, &cmd, &arg);

        if (!cmd || cmd[0] == 0) continue;

        if (cmd_eq(cmd, "HELP")) {
            cmd_help();
        } else if (cmd_eq(cmd, "STATUS")) {
            cmd_status();
        } else if (cmd_eq(cmd, "AUTH")) {
            if (!arg) uart_puts("Usage: AUTH <pw>\n");
            else cmd_auth(arg);
        } else if (cmd_eq(cmd, "IFTAR")) {
            cmd_iftar();
        } else {
            uart_puts("Unknown.\n");
        }
    }
}
