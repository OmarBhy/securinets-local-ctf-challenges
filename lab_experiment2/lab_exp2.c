#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>

volatile uint32_t SEED_A = 0xDEADBEEF;
volatile uint32_t SEED_B = 0xCAFEBABE;
volatile uint8_t MATRIX[16];

typedef enum {
    STATE_ALPHA,
    STATE_BETA,
    STATE_OVERRIDE
} quantum_state_t;

volatile quantum_state_t current_state = STATE_ALPHA;
volatile int observer_detected = 0;

void display_banner() {
    printf("\n");
    printf("===============================================================\n");
    printf("    SECURINETS FST - LAB R&D  ACCESS SYSTEM\n");
    printf("    Multiverse Research Division - Security Level ALPHA-7\n");
    printf("===============================================================\n");
    printf("\n");
}

__attribute__((noinline, optimize("O0")))
uint8_t rotate_left(uint8_t x, int n) {
    n = n % 8;
    return ((x << n) | (x >> (8 - n))) & 0xFF;
}

__attribute__((noinline, optimize("O0")))
uint8_t rotate_right(uint8_t x, int n) {
    n = n % 8;
    return ((x >> n) | (x << (8 - n))) & 0xFF;
}

__attribute__((noinline, optimize("O0")))
uint8_t reverse_bits(uint8_t x) {
    uint8_t r = 0;
    for (int i = 0; i < 8; i++) {
        r = (r << 1) | (x & 1);
        x >>= 1;
    }
    return r;
}

__attribute__((noinline, optimize("O0")))
void init_matrix() {
    volatile uint32_t s = SEED_A;
    for (int i = 0; i < 16; i++) {
        s = s * 0x41C64E6D + 0x3039;
        MATRIX[i] = (s >> 16) & 0xFF;
    }
}

__attribute__((noinline, optimize("O0")))
int check_ptrace() {
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
        return 1;
    }
    return 0;
}

__attribute__((noinline, optimize("O0")))
int check_parent_process() {
    FILE *fp;
    char status[256];
    int tracer_pid = 0;
    
    fp = fopen("/proc/self/status", "r");
    if (fp == NULL) {
        return 0;
    }
    
    while (fgets(status, sizeof(status), fp)) {
        if (strncmp(status, "TracerPid:", 10) == 0) {
            sscanf(status, "TracerPid: %d", &tracer_pid);
            break;
        }
    }
    
    fclose(fp);
    return (tracer_pid != 0);
}

__attribute__((noinline, optimize("O0")))
void determine_quantum_state() {
    printf("[*] Initializing ...\n");
    
    int ptrace_result = check_ptrace();
    int parent_result = check_parent_process();
    
    observer_detected = ptrace_result || parent_result;
    
    if (observer_detected) {
        current_state = STATE_BETA;
        printf("[*] Quantum state: COLLAPSED (Observer detected)\n");
        printf("[*] Entering dimensional path BETA...\n");
    } else {
        current_state = STATE_ALPHA;
        printf("[*] Quantum state: PURE (No observation)\n");
        printf("[*] Entering dimensional path ALPHA...\n");
    }
    
    sleep(1);
}

__attribute__((noinline, optimize("O0")))
int check_override_code(uint32_t code) {
    volatile uint32_t expected = SEED_A;
    if (code == expected) {
        return 1;
    }
    return 0;
}

__attribute__((noinline, optimize("O0")))
uint8_t generate_alpha_key() {
    volatile uint8_t key = 0;
    
    key ^= (SEED_A >> 24) & 0xFF;
    key ^= (SEED_A >> 16) & 0xFF;
    key ^= (SEED_A >> 8) & 0xFF;
    key ^= SEED_A & 0xFF;
    
    for (int i = 0; i < 4; i++) {
        key = rotate_left(key, 3);
        key ^= MATRIX[i * 3];
        key = reverse_bits(key);
        key ^= 0xA5;
    }
    
    key = rotate_right(key, 2);
    key ^= MATRIX[7];
    key ^= 0x5A;
    
    return key;
}

__attribute__((noinline, optimize("O0")))
uint8_t generate_beta_key() {
    volatile uint8_t key = 0;
    
    key ^= (SEED_B >> 24) & 0xFF;
    key ^= (SEED_B >> 16) & 0xFF;
    key ^= (SEED_B >> 8) & 0xFF;
    key ^= SEED_B & 0xFF;
    
    for (int i = 0; i < 3; i++) {
        key = rotate_right(key, 2);
        key ^= MATRIX[i * 4 + 1];
        key = rotate_left(key, 1);
        key ^= 0x3C;
    }
    
    key = reverse_bits(key);
    key ^= MATRIX[11];
    
    return key;
}

__attribute__((noinline, optimize("O0")))
void decrypt_alpha_credentials(uint8_t key) {
    // "Akr1ch_m4_Yo7s0lch" XOR 0xDB
    uint8_t encrypted[] = {
        0x9a, 0xb0, 0xa9, 0xea, 0xb8, 0xb3, 0x84, 0xb6, 0xef, 0x84,
        0x82, 0xb4, 0xec, 0xa8, 0xeb, 0xb7, 0xb8, 0xb3
    };
    
    size_t len = sizeof(encrypted);
    
    printf("\n");
    printf("===============================================================\n");
    printf("    DIMENSION ALPHA - AUTHENTIC RESEARCH DATA\n");
    printf("===============================================================\n");
    printf("\n");
    printf("[+] Access granted to the research vault\n");
    printf("[+] Decrypting authentic credentials...\n");
    printf("[+] Quantum observer effect: STABILIZED\n\n");
    
    printf("Credentials: Securinets_fst{");
    for (size_t i = 0; i < len; i++) {
        printf("%c", encrypted[i] ^ key);
    }
    printf("}\n\n");
    
    printf("[+] Dimensional integrity: VERIFIED\n");
    printf("[+] You have accessed .....\n");
    printf("\n");
}

__attribute__((noinline, optimize("O0")))
void decrypt_beta_credentials(uint8_t key) {
    // "Hek_7solt_m3an4" XOR 0x47
    uint8_t encrypted[] = {
        0x0f, 0x22, 0x2c, 0x18, 0x70, 0x34, 0x28, 0x2b, 0x33, 0x18,
        0x2a, 0x74, 0x26, 0x29, 0x73
    };
    
    size_t len = sizeof(encrypted);
    
    printf("\n");
    printf("===============================================================\n");
    printf("    DIMENSION BETA - HONEYPOT RESEARCH DATA\n");
    printf("===============================================================\n");
    printf("\n");
    printf("[+] Access granted to parallel reasearch vault\n");
    printf("[+] Decrypting credentials...\n");
    printf("[!] Quantum observer effect: DETECTED\n\n");
    
    printf("Credentials: Securinets_fst{");
    for (size_t i = 0; i < len; i++) {
        printf("%c", encrypted[i] ^ key);
    }
    printf("}\n\n");
    
    printf("[!] WARNING: Anomaly detected!\n");
    printf("[!] This is a PARALLEL reasearch  with altered data\n");
    printf("\n");
}

void access_vault() {
    uint8_t key;
    
    printf("\n[*] Accessing ...\n");
    sleep(1);
    
    if (current_state == STATE_ALPHA || current_state == STATE_OVERRIDE) {
        key = generate_alpha_key();
        printf("[*] Generating authentic access key: 0x%02X\n", key);
        sleep(1);
        decrypt_alpha_credentials(key);
    } else {
        key = generate_beta_key();
        printf("[*] Generating parallel access key: 0x%02X\n", key);
        sleep(1);
        decrypt_beta_credentials(key);
    }
}

int main(int argc, char *argv[]) {
    display_banner();
    
    init_matrix();
    
    if (argc > 1) {
        uint32_t input_code = strtoul(argv[1], NULL, 16);
        if (check_override_code(input_code)) {
            printf("[*] EMERGENCY OVERRIDE CODE ACCEPTED\n");
            printf("[*] Bypassing all quantum observations...\n");
            printf("[*] Trust level: MAXIMUM\n\n");
            current_state = STATE_OVERRIDE;
            sleep(1);
            access_vault();
            return 0;
        } else {
            printf("[!] Invalid override code: 0x%X\n", input_code);
            printf("[!] Proceeding with standard authentication...\n\n");
        }
    }
    

   
    
    determine_quantum_state();
    access_vault();
    
    return 0;
}
