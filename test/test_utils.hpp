#pragma once

#include <fstream>

#define RUN_TEST(test_function, file) do {          \
    if (!test_function(file)) {                      \
        std::cerr << "Error with " << file << "\n"; \
        return 1;                                   \
    }                                               \
} while (0)
