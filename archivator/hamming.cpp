#pragma once
#include "hamming.h"

std::vector<int> generateHammingCode(std::vector<int> msgBits, int size, int number_parity_bit) {
    long long size_of_vector = number_parity_bit + size;
    std::vector<int> hammingCode(size_of_vector);
    int j = 0;

    for (int i = 0; i < number_parity_bit; ++i) {
        hammingCode[pow(2, i) - 1] = -1;
    }

    for (int i = 0; i < size_of_vector; i++) {
        if (hammingCode[i] != -1) {
            hammingCode[i] = msgBits[j];
            j++;
        }
    }

    for (int i = 0; i < size_of_vector; i++) {
        if (hammingCode[i] != -1) {
            continue;
        }

        int x = log2(i + 1);
        int one_count = 0;

        for (int j = i + 2; j <= size_of_vector; ++j) {
            if (j & (1 << x)) {
                if (hammingCode[j - 1] == 1) {
                    one_count++;
                }
            }
        }

        if (one_count % 2 == 0) {
            hammingCode[i] = 0;
        } else {
            hammingCode[i] = 1;
        }
    }

    return hammingCode;
}

std::vector<int> findHammingCode(std::vector<int> &msgBit) {
    int size = msgBit.size();
    int number_parity_bit = 1;
    while (pow(2, number_parity_bit) < (size + number_parity_bit + 1)) {
        number_parity_bit++;
    }
    return generateHammingCode(msgBit, size, number_parity_bit);
}
