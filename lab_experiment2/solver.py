#!/usr/bin/env python3

def rotate_left(x, n):
    n = n % 8
    return ((x << n) | (x >> (8 - n))) & 0xFF

def rotate_right(x, n):
    n = n % 8
    return ((x >> n) | (x << (8 - n))) & 0xFF

def reverse_bits(x):
    r = 0
    for _ in range(8):
        r = (r << 1) | (x & 1)
        x >>= 1
    return r & 0xFF

def init_matrix(seed):
    """Linear Congruential Generator"""
    matrix = []
    s = seed
    for i in range(16):
        s = (s * 0x41C64E6D + 0x3039) & 0xFFFFFFFF
        matrix.append((s >> 16) & 0xFF)
    return matrix

def generate_alpha_key(seed_a, matrix):
    """Reverse engineered from Ghidra"""
    key = 0
    
    # XOR SEED_A bytes
    key ^= (seed_a >> 24) & 0xFF
    key ^= (seed_a >> 16) & 0xFF
    key ^= (seed_a >> 8) & 0xFF
    key ^= seed_a & 0xFF
    
    # Complex transformations
    for i in range(4):
        key = rotate_left(key, 3)
        key ^= matrix[i * 3]
        key = reverse_bits(key)
        key ^= 0xA5
    
    # Finalization
    key = rotate_right(key, 2)
    key ^= matrix[7]
    key ^= 0x5A
    
    return key & 0xFF

# Constants
SEED_A = 0xDEADBEEF
encrypted = [
    0x9a, 0xb0, 0xa9, 0xea, 0xb8, 0xb3, 0x84, 0xb6, 0xef, 0x84,
    0x82, 0xb4, 0xec, 0xa8, 0xeb, 0xb7, 0xb8, 0xb3
]

# Calculate
matrix = init_matrix(SEED_A)
key = generate_alpha_key(SEED_A, matrix)

# Decrypt
flag = ''.join(chr(b ^ key) for b in encrypted)
print(f"Securinets_fst{{{flag}}}")
