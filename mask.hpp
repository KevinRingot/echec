#ifndef MASK_HPP_
#define MASK_HPP_

#include "types.hpp"

void empty_mask(Mask mask);
void clear_mask(Mask mask);
int get_mask(const Mask mask, int i, int j);
void set_mask(Mask mask, int i, int j, int value);

#endif
