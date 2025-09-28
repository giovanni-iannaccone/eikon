#pragma once

#define ATOI_DEC(str) static_cast<uint>(std::stoul(str, nullptr, 10))
#define ATOI_HEX(str) static_cast<uint>(std::stoul(str, nullptr, 16))