#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

static inline uint8_t key1(void) {
    uintptr_t x = (uintptr_t)&key1;
    return (uint8_t)((x >> 3) ^ (x >> 7) ^ 0xA3);
}

static inline uint8_t key2(void) {
    uintptr_t x = (uintptr_t)&key2;
    return (uint8_t)((x >> 5) + (x >> 11) ^ 0x5C);
}

static inline uint8_t key3(void) {
    uintptr_t x = (uintptr_t)&key3;
    return (uint8_t)((x >> 9) ^ (x >> 2) ^ 0xF1);
}

void xor_stage(char *buf, size_t len, uint8_t key) {
    for (size_t i = 0; i < len; i++) {
        buf[i] ^= (key + (uint8_t)i);
    }
}

void verify(const char *input, const uint8_t *secret, size_t len, uint8_t key) {
    char tmp[128];

    memcpy(tmp, input, len);
    xor_stage(tmp, len, key);

    if (memcmp(tmp, secret, len) != 0) {
        puts("Wrong!");
        exit(0);
    }
}

int main(void) {
    char buf[128];

    // hedha part lowel
    const uint8_t secret1[] = {
        0xD0, 0xC8, 0xC6, 0xD2, 0xD7, 0xC2, 0xCB,
        0xD6, 0xC6, 0xD0, 0xF8, 0xF1, 0xBA
    }; 
    
    printf("welcome to dimension C-14598");
    printf("To return home, u need to pass these three portals!");

    printf("Enter part 1: ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = 0;

    if (strlen(buf) != sizeof(secret1)) return 0;
    verify(buf, secret1, sizeof(secret1), key1());


    printf("Good to the next portal !!!!");
    // hedha part 2
    // m4yB3_XOR_15Nt_
    const uint8_t secret2[] = {
        0x99, 0xA2, 0xBD, 0xAF, 0x9A, 0xE4, 0xF2,
        0xE9, 0xF9, 0xB3, 0xBE, 0xCC, 0xC0, 0xF1
    };


    printf("Enter part 2: ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = 0;

    if (strlen(buf) != sizeof(secret2)) return 0;
    verify(buf, secret2, sizeof(secret2), key2());


    printf("Well done! Final pôrtal ahead");
    // hedha part 3!!!!!!
    // Th4t_Bad_R4nd0mXyZ}
    const uint8_t secret3[] = {
        0x82, 0x9D, 0x91, 0x8E, 0xD4, 0x96, 0x9F,
        0x92, 0xD4, 0x88, 0x9B, 0x8F, 0x91, 0xC4,
        0xF2, 0xE6, 0xFC, 0xE9
    }; 

    printf("Enter part 3: ");
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    buf[strcspn(buf, "\n")] = 0;

    if (strlen(buf) != sizeof(secret3)) return 0;
    verify(buf, secret3, sizeof(secret3), key3());

    puts("Correct! Now u get to go home.");
    return 0;
}
