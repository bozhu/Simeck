#ifndef SIMECK32_H
#define SIMECK32_H

#include <stdint.h>

#define LROT16(x, r) (((x) << (r)) | ((x) >> (16 - (r))))

#define ROUND32(key, lft, rgt, tmp) do { \
    tmp = (lft); \
    lft = ((lft) & LROT16((lft), 5)) ^ LROT16((lft), 1) ^ (rgt) ^ (key); \
    rgt = (tmp); \
} while (0)


void simeck_32_64(
        const uint16_t master_key[],
        const uint16_t plaintext[],
        uint16_t ciphertext[]
) {
    const int NUM_ROUNDS = 32;
    int idx;

    uint16_t keys[4] = {
        master_key[0],
        master_key[1],
        master_key[2],
        master_key[3],
    };
    ciphertext[0] = plaintext[0];
    ciphertext[1] = plaintext[1];
    uint16_t temp;

    uint16_t constant = 0xFFFC;
    uint32_t sequence = 0x9A42BB1F;

    for (idx = 0; idx < NUM_ROUNDS; idx++) {
        ROUND32(
                keys[0],
                ciphertext[1],
                ciphertext[0],
                temp
        );

        constant &= 0xFFFC;
        constant |= sequence & 1;
        sequence >>= 1;
        ROUND32(
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


#endif  // SIMECK32_H
