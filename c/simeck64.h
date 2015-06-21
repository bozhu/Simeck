#ifndef SIMECK64_H
#define SIMECK64_H

// #include <stdio.h>
#include <stdint.h>

#define LROT32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

#define ROUND64(key, lft, rgt, tmp) do { \
    tmp = (lft); \
    lft = ((lft) & LROT32((lft), 5)) ^ LROT32((lft), 1) ^ (rgt) ^ (key); \
    rgt = (tmp); \
} while (0)


void simeck_64_128(
        const uint32_t master_key[],
        const uint32_t plaintext[],
        uint32_t ciphertext[]
) {
    const int NUM_ROUNDS = 44;
    int idx;

    uint32_t keys[4] = {
        master_key[0],
        master_key[1],
        master_key[2],
        master_key[3],
    };
    ciphertext[0] = plaintext[0];
    ciphertext[1] = plaintext[1];
    uint32_t temp;

    uint32_t constant = 0xFFFFFFFC;
    uint64_t sequence = 0x938BCA3083F;

    for (idx = 0; idx < NUM_ROUNDS; idx++) {
        ROUND64(
                keys[0],
                ciphertext[1],
                ciphertext[0],
                temp
        );

        constant &= 0xFFFFFFFC;
        constant |= sequence & 1;
        sequence >>= 1;
        ROUND64(
                constant,
                keys[1],
                keys[0],
                temp
        );

        // rotate the LFSR of keys
        temp = keys[1];
        keys[1] = keys[2];
        keys[2] = keys[3];
        keys[3] = temp;
    }
}


#endif  // SIMECK64_H
