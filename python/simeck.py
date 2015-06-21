#!/usr/bin/env python


NUM_ROUNDS = {
    # (block_size, key_size): num_rounds
    (32, 64): 32,
    (48, 96): 36,
    (64, 128): 44,
}


def get_sequence(num_rounds):
    if num_rounds < 40:
        states = [1] * 5
    else:
        states = [1] * 6

    for i in range(num_rounds - 5):
        if num_rounds < 40:
            feedback = states[i + 2] ^ states[i]
        else:
            feedback = states[i + 1] ^ states[i]
        states.append(feedback)

    return tuple(states)


class Simeck:
    def __init__(self, block_size, key_size, master_key):
        assert (block_size, key_size) in NUM_ROUNDS
        assert 0 <= master_key < (1 << key_size)
        self._block_size = block_size
        self._key_size = key_size
        self._word_size = block_size / 2
        self._num_rounds = NUM_ROUNDS[(block_size, key_size)]
        self._sequence = get_sequence(self._num_rounds)
        self._modulus = 1 << self._word_size
        self.change_key(master_key)

    def _LROT(self, x, r):
        assert 0 <= x < self._modulus
        res = (x << r) % self._modulus
        res |= x >> (self._word_size - r)
        return res

    def _round(self, round_key, left, right):
        assert 0 <= round_key < self._modulus
        assert 0 <= left < self._modulus
        assert 0 <= right < self._modulus
        temp = left
        left = right ^ (left & self._LROT(left, 5)) \
            ^ self._LROT(left, 1) ^ round_key
        right = temp
        # print hex(round_key), hex(left), hex(right)
        return left, right

    def change_key(self, master_key):
        assert 0 <= master_key < (1 << self._key_size)
        states = []
        for i in range(self._key_size / self._word_size):
            states.append(master_key % self._modulus)
            master_key >>= self._word_size

        constant = self._modulus - 4
        round_keys = []
        for i in range(self._num_rounds):
            round_keys.append(states[0])
            left, right = states[1], states[0]
            left, right = self._round(constant ^ self._sequence[i],
                                      left, right)
            states.append(left)
            states.pop(0)
            states[0] = right

        self.__round_keys = tuple(round_keys)

    def encrypt(self, plaintext):
        assert 0 <= plaintext < (1 << self._block_size)
        left = plaintext >> self._word_size
        right = plaintext % self._modulus

        for idx in range(self._num_rounds):
            left, right = self._round(self.__round_keys[idx],
                                      left, right)

        ciphertext = (left << self._word_size) | right
        return ciphertext


def print_test_vector(block_size, key_size, key, plain, cipher):
    print 'Simeck', block_size, key_size
    print 'key', hex(key)[2:].rstrip('L').zfill(key_size / 4)
    print 'plaintext', hex(plain)[2:].rstrip('L').zfill(block_size / 4)
    print 'ciphertext', hex(cipher)[2:].rstrip('L').zfill(block_size / 4)


def main():
    plaintext32 = 0x65656877
    key64 = 0x1918111009080100
    simeck32 = Simeck(32, 64, key64)
    ciphertext32 = simeck32.encrypt(plaintext32)
    print_test_vector(32, 64, key64, plaintext32, ciphertext32)
    print

    plaintext48 = 0x72696320646e
    key96 = 0x1a19181211100a0908020100
    simeck48 = Simeck(48, 96, key96)
    ciphertext48 = simeck48.encrypt(plaintext48)
    print_test_vector(48, 96, key96, plaintext48, ciphertext48)
    print

    plaintext64 = 0x656b696c20646e75
    key128 = 0x1b1a1918131211100b0a090803020100
    simeck64 = Simeck(64, 128, key128)
    ciphertext64 = simeck64.encrypt(plaintext64)
    print_test_vector(64, 128, key128, plaintext64, ciphertext64)


if __name__ == '__main__':
    main()
