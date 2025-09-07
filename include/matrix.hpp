#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>

#include "utils.hpp"

uint32_t convolute(uint32_t *matrix[], uint n);

void reverse_matrix(uint32_t *matrix[], uint n, uint m);
void transpose_matrix(uint32_t *matrix[], uint n, uint m);
