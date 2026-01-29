#include <stdio.h>
#include <string.h>

static void show_terminal(void) {
    printf("╔══════════════════════════════════════════════════════╗\n");
    printf("║   SECURINETS FST - EXCHANGE PROGRAM TERMINAL         ║\n");
    printf("║   Candidate Verification Interface                   ║\n");
    printf("╚══════════════════════════════════════════════════════╝\n\n");
}

static unsigned char blob[] = {
    0x11, 0x27, 0x21, 0x37, 0x30, 0x2b, 0x2c, 0x27, 0x36, 0x31,
    0x1d, 0x24, 0x31, 0x36, 0x39, 0x30, 0x20, 0x27, 0x75, 0x36,
    0x1d, 0x2f, 0x71, 0x23, 0x2c, 0x23, 0x1d, 0x30, 0x27, 0x75,
    0x2e, 0x23, 0x3f
};

volatile unsigned char registry[256];

__attribute__((noinline, optimize("O0")))
static void setup_registry(void) {
    for (int i = 0; i < 256; i++) {
        registry[i] = (unsigned char)((i + 0x42) - i);
    }
}

__attribute__((noinline, optimize("O0")))
static unsigned char fetch_token(int selector) {
    return registry[selector & 0xff];
}

int main(int argc, char **argv) {
    char input[64];
    int ok = 1;

    (void)argv;

    setup_registry();
    show_terminal();

    unsigned char token = fetch_token(argc);

    printf("[*] Enter verification phrase: ");
    scanf("%63s", input);

    if (strlen(input) != sizeof(blob)) {
        ok = 0;
    }

    for (int i = 0; i < (int)sizeof(blob) && ok; i++) {
        if (((unsigned char)input[i] ^ token) != blob[i]) {
            ok = 0;
        }
    }

    if (ok) {
        printf("\n[+] Verified.\n");
    } else {
        printf("\n[-] Denied.\n");
    }

    return 0;
}
