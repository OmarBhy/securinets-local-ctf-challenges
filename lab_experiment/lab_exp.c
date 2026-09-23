// lab_exp.c — FINAL FIXED VERSION
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

void print_banner() {
    printf("╔══════════════════════════════════════╗\n");
    printf("║     SECURINETS FST - R&D TERMINAL    ║\n");
    printf("╚══════════════════════════════════════╝\n\n");
}

volatile unsigned char lab_key = 0;
volatile unsigned char experiment_data[8];

__attribute__((noinline, optimize("O0")))
void init_experiment() {
    experiment_data[0] = 0x15;
    experiment_data[1] = 0x28;
    experiment_data[2] = 0x3B;
    experiment_data[3] = 0x4E;
    experiment_data[4] = 0x61;
    experiment_data[5] = 0x74;
    experiment_data[6] = 0x87;
    experiment_data[7] = 0x9A;
}

__attribute__((noinline, optimize("O0")))
unsigned char research_phase_1(int id) {
    unsigned char a = experiment_data[id & 7];
    unsigned char b = experiment_data[(id + 4) & 7];
    return (unsigned char)(a ^ b);
}

__attribute__((noinline, optimize("O0")))
unsigned char research_phase_2(unsigned char input) {
    unsigned char r = input;
    r = (unsigned char)(r + 0x32);
    r = (unsigned char)(r ^ 0x1F);
    r = (unsigned char)(((r << 4) | (r >> 4)) & 0xFF);
    return r;
}

__attribute__((noinline, optimize("O0")))
unsigned char final_synthesis(unsigned char input, int factor) {
    unsigned char r = input;
    r = (unsigned char)(r ^ (unsigned char)(factor * 0x11));
    r = (unsigned char)(r - 0x08);
    return r;
}

/*
 * Final key forced to 0xE1
 */
__attribute__((noinline, optimize("O0")))
unsigned char compute_lab_key(int s1, int s2) {
    unsigned char step1 = research_phase_1(s1);
    unsigned char step2 = research_phase_2(step1);
    unsigned char step3 = final_synthesis(step2, s2);

    volatile unsigned char target = 0xE1;
    step3 = (unsigned char)(step3 + (unsigned char)(target - step3));

    return step3;
}

__attribute__((noinline, optimize("O0")))
void secure_bzero(volatile void *p, size_t n) {
    volatile unsigned char *q = (volatile unsigned char *)p;
    for (size_t i = 0; i < n; i++) q[i] = 0;
}

/*
 * Cipher = "Securinets_fst{ass1stant_thi9a}" XOR 0xE1
 * Byte-exact, LITTLE-ENDIAN SAFE
 */
__attribute__((noinline, optimize("O0")))
void build_cipher(unsigned char *out, size_t n) {

    static const uint32_t A[] = {
        0x948284b2, // b2 84 82 94
        0x848f8893, // 93 88 8f 84
        0x87be9295, // 95 92 be 87   FIXED
        0x809a9592, // 92 95 9a 80
        0x92d09292, // 92 92 d0 92
        0x958f8095, // 95 80 8f 95   FIXED
        0x888995be, // be 95 89 88
        0x009c80d8  // d8 80 9c 00
    };

    size_t idx = 0;
    for (size_t w = 0; idx < n; w++) {
        uint32_t wa = A[w];
        for (int k = 0; k < 4 && idx < n; k++, idx++) {
            out[idx] = (unsigned char)((wa >> (8 * k)) & 0xFF);
        }
    }
}

int main(int argc, char *argv[]) {
    (void)argv;

    char input[64];
    int correct = 1;

    init_experiment();
    print_banner();

    printf("[*] Initializing experiment...\n");
    printf("[*] Loading research parameters...\n");

    int seed1 = argc;       // 1
    int seed2 = argc + 1;   // 2

    lab_key = compute_lab_key(seed1, seed2);

    printf("Experiment ready!\n\n");
    printf("Enter the secret formula: ");

    if (!fgets(input, sizeof(input), stdin)) return 1;

    size_t len = strlen(input);
    if (len && input[len - 1] == '\n') {
        input[--len] = '\0';
    }

    const size_t N = 31;
    if (len != N) correct = 0;

    unsigned char *cipher = malloc(N);
    if (!cipher) return 1;

    build_cipher(cipher, N);

    for (size_t i = 0; i < N && correct; i++) {
        if (((unsigned char)input[i] ^ lab_key) != cipher[i]) {
            correct = 0;
        }
    }

    secure_bzero(cipher, N);
    free(cipher);
    secure_bzero(input, sizeof(input));

    if (correct) {
        printf("\nsa7a lik hek tala3tha!\n");
        printf("You have unlocked the research database!\n");
    } else {
        printf("\nBara El3ab B3id!\n");
        printf("The formula is incorrect\n");
    }

    return 0;
}

