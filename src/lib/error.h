#pragma once
#include <string>

void errif(bool condition, const std::string err_msg);

enum RETCODE{
    SUCCESS = 0,
    INTERNAL = 1, 
};