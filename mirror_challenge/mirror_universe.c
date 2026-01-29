#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

uint8_t D_1[] = {0x91, 0xaf, 0xac, 0xff, 0xa9, 0xa0, 0xa8, 0xff, 0xa9, 0xac, 0xbe};
uint8_t D_2[] = {0xfb, 0x69, 0xcb, 0xc5, 0x7b, 0xcf, 0x4a, 0x62, 0x1e, 0xe9, 0x5b};

size_t L_1 = 11;
size_t L_2 = 11;

volatile uint8_t M_A[16];
volatile uint8_t M_B[16];
volatile uint8_t K_1 = 0;
volatile uint8_t K_2 = 0;

__attribute__((noinline, optimize("O0")))
uint8_t _rb(uint8_t x) {
    uint8_t r = 0;
    for (int i = 0; i < 8; i++) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

__attribute__((noinline, optimize("O0")))
uint8_t _rl(uint8_t x, int n) {
    n = n % 8;
    return ((x << n) | (x >> (8 - n))) & 0xFF;
}

__attribute__((noinline, optimize("O0")))
uint8_t _rr(uint8_t x, int n) {
    n = n % 8;
    return ((x >> n) | (x << (8 - n))) & 0xFF;
}

__attribute__((noinline, optimize("O0")))
uint8_t _sn(uint8_t x) {
    return ((x << 4) | (x >> 4)) & 0xFF;
}

__attribute__((noinline, optimize("O0")))
void _ia(uint32_t s) {
    volatile uint32_t x = s;
    for (int i = 0; i < 16; i++) {
        x = x * 0x41C64E6D + 0x3039;
        M_A[i] = (x >> 16) & 0xFF;
    }
}

__attribute__((noinline, optimize("O0")))
void _ib(uint32_t s) {
    volatile uint32_t x = s;
    for (int i = 0; i < 16; i++) {
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        M_B[i] = x & 0xFF;
    }
}

__attribute__((noinline, optimize("O0")))
uint8_t _dk1() {
    volatile uint8_t k = M_A[3] ^ M_A[11];
    for (int i = 0; i < 5; i++) {
        k = _rl(k, 3);
        k ^= M_A[(i * 2) % 16];
        k = _rb(k);
        k ^= 0x5A;
    }
    k = _rr(k, 2);
    k ^= M_B[7];
    k = _rb(k);
    K_1 = k;
    return k;
}

__attribute__((noinline, optimize("O0")))
uint8_t _dk2() {
    volatile uint8_t k = _rb(M_B[5]);
    k ^= M_A[9];
    for (int i = 0; i < 6; i++) {
        k = _rr(k, i % 5);
        k ^= M_B[(i * 3) % 16];
        k = _rl(k, 2);
        k ^= 0xC3;
    }
    k = _rb(k);
    k ^= M_A[13];
    K_2 = k;
    return k;
}

__attribute__((noinline, optimize("O0")))
uint8_t _l1(uint8_t c) {
    if (c >= 0x61 && c <= 0x7a) {
        return ((c - 0x61 + 13) % 26) + 0x61;
    } else if (c >= 0x41 && c <= 0x5a) {
        return ((c - 0x41 + 13) % 26) + 0x41;
    }
    return c;
}

__attribute__((noinline, optimize("O0")))
uint8_t _l2(uint8_t c, int p) {
    uint8_t r = _sn(c);
    if (p % 2 == 1) {
        r = _rb(r);
    }
    return r;
}

__attribute__((noinline, optimize("O0")))
int _cp1(uint8_t *in, size_t il, uint8_t k) {
    uint8_t t[16];
    if (il < L_1) return 0;
    for (size_t i = 0; i < L_1; i++) {
        t[i] = in[L_1 - 1 - i];
    }
    for (size_t i = 0; i < L_1; i++) {
        uint8_t tr = _l1(t[i]);
        if ((tr ^ k) != D_1[i]) return 0;
    }
    return 1;
}

__attribute__((noinline, optimize("O0")))
int _cp2(uint8_t *in, size_t il, uint8_t k) {
    if (il < L_2) return 0;
    for (size_t i = 0; i < L_2; i++) {
        uint8_t tr = _l2(in[i], i);
        if ((tr ^ k) != D_2[i]) return 0;
    }
    return 1;
}

void _b() {
    printf("\n");
    printf("  ╔══════════════════════════════════════════════════════════════╗\n");
    printf("  ║  ░▒▓███████▓▒░  MIRROR UNIVERSE  ░▒▓███████▓▒░               ║\n");
    printf("  ║                                                              ║\n");
    printf("  ║     SecuriNets FST - Dimensional Recovery System             ║\n");
    printf("  ║     [!] BREACH DETECTED FROM PARALLEL DIMENSION              ║\n");
    printf("  ╚══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void _pf(int v1, int v2) {
    printf("\n");
    printf("  ╔════════════════════════════════════════════════════════════════╗\n");
    printf("  ║  DIMENSIONAL COLLAPSE                                          ║\n");
    printf("  ║                                                                ║\n");
    if (!v1 && !v2) {
        printf("  ║  Laws violated: I & II                                         ║\n");
    } else if (!v1) {
        printf("  ║  Law violated: I                                               ║\n");
    } else if (!v2) {
        printf("  ║  Law violated: II                                              ║\n");
    }
    printf("  ╚════════════════════════════════════════════════════════════════╝\n");
}

void _ps() {
    printf("\n");
    printf("  ╔════════════════════════════════════════════════════════════════╗\n");
    printf("  ║  DIMENSIONAL SYNC COMPLETE                                     ║\n");
    printf("  ║                                                                 ║\n");
    printf("  ║  Welcome back to the real Securinets FST!                      ║\n");
    printf("  ║  You can now edit and manage your data.                        ║\n");
    printf("  ╚════════════════════════════════════════════════════════════════╝\n");
}

int main() {
    char in[64];
    char pf[] = "Securinets_fst{";
    
    _b();
    
    printf("[*] Initializing mirror matrices...\n");
    
    _ia(0xDEADBEEF);
    _ib(0xCAFEBABE);
    
    printf("[*] Deriving dimensional keys...\n");
    
    uint8_t k1 = _dk1();
    uint8_t k2 = _dk2();
    
    printf("[*] Mirror synchronization complete.\n\n");
    
    printf("Please enter the recovery key: %s", pf);
    fflush(stdout);
    
    if (fgets(in, sizeof(in), stdin) == NULL) return 1;
    
    size_t len = strlen(in);
    if (len > 0 && in[len-1] == '\n') {
        in[len-1] = '\0';
        len--;
    }
    
    if (len < 1 || in[len-1] != '}') {
        printf("\n  [-] INVALID FORMAT\n");
        return 1;
    }
    
    in[len-1] = '\0';
    len--;
    
    int v1 = 0, v2 = 0;
    
    if (len >= L_1) {
        v1 = _cp1((uint8_t*)in, L_1, k1);
    }
    
    if (len >= L_1 + L_2) {
        v2 = _cp2((uint8_t*)in + L_1, L_2, k2);
    }
    
    if (len == L_1 + L_2 && v1 && v2) {
        _ps();
    } else {
        if (len != L_1 + L_2) {
            if (len < L_1) { v1 = 0; v2 = 0; }
            else if (len < L_1 + L_2) { v2 = 0; }
            else { v1 = 0; v2 = 0; }
        }
        _pf(v1, v2);
    }
    
    return 0;
}
