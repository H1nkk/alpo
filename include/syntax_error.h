#pragma once
#include <string>

struct syntax_error {
    size_t pos;
    std::string message;
};