// lab_exp.c
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
 * Key computed through phases, then normalized to 0xE1.
 * Intended for dynamic analysis: break here and inspect AL/RAX after return.
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
 * Ciphertext for:
 *   "Securinets_fst{ass1stant_thi9a}" XOR 0xE1
 * but not stored as a plain byte array; reconstructed from words.
 */
__attribute__((noinline, optimize("O0")))
void build_cipher(unsigned char *out, size_t n) {
    static const uint32_t A[] = {
        0x948284b2, 0x848f8893, 0xbe929584, 0x809a9592,
        0x92d09292, 0x958f8080, 0x888995be, 0x009c80d8
    };
    static const uint32_t B[] = {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000
    };

    size_t idx = 0;
    for (size_t w = 0; idx < n; w++) {
        uint32_t wa = A[w];
        uint32_t wb = B[w];
        for (int k = 0; k < 4 && idx < n; k++, idx++) {
            unsigned char ba = (unsigned char)((wa >> (8 * k)) & 0xFF);
            unsigned char bb = (unsigned char)((wb >> (8 * k)) & 0xFF);
            out[idx] = (unsigned char)(ba ^ bb);
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

    int seed1 = argc;       // normally 1
    int seed2 = argc + 1;   // normally 2

    lab_key = compute_lab_key(seed1, seed2);

    printf("Experiment ready!\n\n");
    printf("Enter the secret formula: ");

    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    const size_t N = 31;

    if (len != N) {
        correct = 0;
    }

    unsigned char *cipher = (unsigned char *)malloc(N);
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
        printf("\n sa7a lik hek tala3tha ! \n");
        printf("You have unlocked the research database!\n");
    } else {
        printf("\nBara El3ab B3id !\n");
        printf(" The formula is incorrect\n");
    }

    return 0;
}
