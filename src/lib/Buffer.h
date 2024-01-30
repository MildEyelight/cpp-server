#pragma once
#include <string>
class Buffer{
private:
    std::string buf;
    // std::string 和std::vector<char> 的性能比较。
public:
    Buffer() = default;
    int append(const char* _str ,const int _size);
    int clear();
    const char* c_str() const;
    ssize_t size() const;
};