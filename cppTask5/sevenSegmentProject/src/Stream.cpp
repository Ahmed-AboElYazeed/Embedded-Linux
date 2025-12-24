#include "Stream.hpp"
#include <iostream>

stream::stream(const std::string& userPath) : path(userPath)
{
    std::cout << "stream constructor\n";
}

stream::~stream()
{
    std::cout << "stream destructor\n";
}
