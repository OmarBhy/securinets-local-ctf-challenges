#!/usr/bin/env python3
"""
Mirror Universe - Solver
Author:omar BHY
Category: Reverse Engineering

Données extraites:
  - KEY_1 = 0xCE (via GDB sur derive_key_1)
  - KEY_2 = 0x0D (via GDB sur derive_key_2)
  - D_1 @ 0x00404040
  - D_2 @ 0x00404050
  - L_1 = L_2 = 11

Transformations identifiées:
  - Part 1: REVERSE → ROT13 → XOR KEY_1
  - Part 2: SWAP_NIBBLES → (REVERSE_BITS si pos impaire) → XOR KEY_2
"""

# ============================================
# Fonctions de Transformation
# ============================================

def rot13(c):
    """
    ROT13: Décale chaque lettre de 13 positions dans l'alphabet.
    ROT13 est son propre inverse: rot13(rot13(x)) = x
    
    Exemples:
      'a' → 'n'
      'n' → 'a'
      'A' → 'N'
      '1' → '1' (non-lettres inchangées)
    """
    if 'a' <= c <= 'z':
        # Lettres minuscules
        return chr((ord(c) - ord('a') + 13) % 26 + ord('a'))
    elif 'A' <= c <= 'Z':
        # Lettres majuscules
        return chr((ord(c) - ord('A') + 13) % 26 + ord('A'))
    else:
        # Autres caractères inchangés
        return c

def swap_nibbles(x):
    """
    Échange les 4 bits hauts avec les 4 bits bas.
    swap_nibbles est son propre inverse: swap(swap(x)) = x
    
    Exemple:
      0x6F (0110 1111) → 0xF6 (1111 0110)
    """
    return ((x << 4) | (x >> 4)) & 0xFF

def reverse_bits(x):
    """
    Inverse l'ordre des 8 bits.
    reverse_bits est son propre inverse: reverse(reverse(x)) = x
    
    Exemple:
      0xF6 (1111 0110) → 0x6F (0110 1111)
    """
    result = 0
    for _ in range(8):
        result = (result << 1) | (x & 1)
        x >>= 1
    return result & 0xFF

# ============================================
# Données Extraites
# ============================================

# Données chiffrées (depuis GDB @ 0x00404040 et 0x00404050)
D_1 = [0x91, 0xaf, 0xac, 0xff, 0xa9, 0xa0, 0xa8, 0xff, 0xa9, 0xac, 0xbe]
D_2 = [0xfb, 0x69, 0xcb, 0xc5, 0x7b, 0xcf, 0x4a, 0x62, 0x1e, 0xe9, 0x5b]

# Clés (depuis GDB)
KEY_1 = 0xCE
KEY_2 = 0x0D

# Longueurs
L_1 = 11
L_2 = 11

print("[*] Mirror Universe - Solver")
print(f"[*] KEY_1 = 0x{KEY_1:02X}")
print(f"[*] KEY_2 = 0x{KEY_2:02X}")
print(f"[*] L_1 = {L_1}, L_2 = {L_2}")
print()

# ============================================
# Décryptage Part 1
# ============================================

print("="*60)
print("PART 1: Decryption")
print("="*60)

# Algorithme:
# D_1[i] → XOR KEY_1 → ROT13 → caractère
# Puis REVERSE le string final

temp_part1 = ""

for i, encrypted_byte in enumerate(D_1):
    # Étape 1: XOR avec la clé
    xored = encrypted_byte ^ KEY_1
    
    # Étape 2: Appliquer ROT13 (inverse de ROT13 = ROT13)
    char = chr(xored)
    decrypted_char = rot13(char)
    
    temp_part1 += decrypted_char
    
    print(f"  [{i:2d}] D_1=0x{encrypted_byte:02x} → XOR 0x{KEY_1:02X} → 0x{xored:02x} ('{char}') → ROT13 → '{decrypted_char}'")

# Étape 3: REVERSE le string (car l'encryption fait REVERSE en premier)
part1 = temp_part1[::-1]

print(f"\n  Avant reverse: '{temp_part1}'")
print(f"  Après reverse: '{part1}'")

# ============================================
# Décryptage Part 2
# ============================================

print()
print("="*60)
print("PART 2: Decryption")
print("="*60)

# Algorithme:
# D_2[i] → XOR KEY_2 → (REVERSE_BITS si i impair) → SWAP_NIBBLES → caractère

part2 = ""

for i, encrypted_byte in enumerate(D_2):
    # Étape 1: XOR avec la clé
    xored = encrypted_byte ^ KEY_2
    
    # Étape 2: Reverse bits si position impaire (inverse de l'encryption)
    if i % 2 == 1:
        after_reverse = reverse_bits(xored)
        step_info = f"XOR → 0x{xored:02x} → REV_BITS → 0x{after_reverse:02x}"
    else:
        after_reverse = xored
        step_info = f"XOR → 0x{xored:02x} (no reverse)"
    
    # Étape 3: Swap nibbles (inverse de swap = swap)
    decrypted_byte = swap_nibbles(after_reverse)
    decrypted_char = chr(decrypted_byte)
    
    part2 += decrypted_char
    
    print(f"  [{i:2d}] D_2=0x{encrypted_byte:02x} → {step_info} → SWAP → 0x{decrypted_byte:02x} ('{decrypted_char}')")

# ============================================
# Résultat Final
# ============================================

print()
print("="*60)
print("RESULT")
print("="*60)
print(f"Part 1: {part1}")
print(f"Part 2: {part2}")
print()
print(f"🚩 FLAG: Securinets_fst{{{part1}{part2}}}")
print("="*60)
