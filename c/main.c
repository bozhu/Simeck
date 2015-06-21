#include <stdio.h>
#include <stdint.h>

#include "simeck32.h"
#include "simeck48.h"
#include "simeck64.h"

int main() {
    uint16_t text32[] = {
        0x6877,
        0x6565,
    };
    const uint16_t key64[] = {
        0x0100,
        0x0908,
        0x1110,
        0x1918,
    };
    simeck_32_64(key64, text32, text32);
    printf("Simeck32/64 %04x %04x\n", text32[1], text32[0]);

    uint32_t text48[] = {
        0x20646e,
        0x726963,
    };
    const uint32_t key96[] = {
        0x020100,
        0x0a0908,
        0x121110,
        0x1a1918,
    };
    simeck_48_96(key96, text48, text48);
    printf("Simeck48/96 %06x %06x\n", text48[1], text48[0]);

    uint32_t text64[] = {
        0x20646e75,
        0x656b696c,
    };
    const uint32_t key128[] = {
        0x03020100,
        0x0b0a0908,
        0x13121110,
        0x1b1a1918,
    };
    simeck_64_128(key128, text64, text64);
    printf("Simeck64/128 %08x %08x\n", text64[1], text64[0]);

    return 0;
}
