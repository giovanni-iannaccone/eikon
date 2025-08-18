#pragma once

#include <fstream>

#include "../include/raccoon.hpp"
#include "../include/shapes.hpp"

#define RUN_TEST(test_function, file) do {          \
    if (test_function(file)) {                      \
        std::cerr << "Error with " << file << "\n"; \
        return 1;                                   \
    }                                               \
} while (0)
