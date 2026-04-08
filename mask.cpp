#include "mask.hpp"

void empty_mask(Mask mask) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            mask[i][j] = 0;
        }
    }
}

void clear_mask(Mask mask) {
    empty_mask(mask);
}

int get_mask(const Mask mask, int i, int j) {
    return mask[i][j];
}

void set_mask(Mask mask, int i, int j, int value) {
    mask[i][j] = value;
}
