#ifndef SIMECK48_H
#define SIMECK48_H

#include <stdint.h>

#define LROT24(x, r) ((((x) << (r)) % (1 << 24)) | ((x) >> (24 - (r))))

#define ROUND48(key, lft, rgt, tmp) do { \
    tmp = (lft); \
    lft = ((lft) & LROT24((lft), 5)) ^ LROT24((lft), 1) ^ (rgt) ^ (key); \
    rgt = (tmp); \
} while (0)


void simeck_48_96(
        const uint32_t master_key[],
        const uint32_t plaintext[],
        uint32_t ciphertext[]
) {
    const int NUM_ROUNDS = 36;
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

    uint32_t constant = 0xFFFFFC;
    uint32_t sequence = 0x9A42BB1F;

    for (idx = 0; idx < NUM_ROUNDS; idx++) {
        ROUND48(
                keys[0],
                ciphertext[1],
                ciphertext[0],
                temp
        );

        constant &= 0xFFFFFC;
        constant |= sequence & 1;
        sequence >>= 1;
        ROUND48(
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


#endif  // SIMECK48_H
