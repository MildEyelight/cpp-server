#include "Buffer.h"

int Buffer::append(const char* _str, const int _size){
    buf.append(_str, _size);
    return _size;
}

int Buffer::clear(){
    buf.clear();
    return 0;
}

const char* Buffer::c_str() const{
    return buf.c_str();
}

ssize_t Buffer::size() const{
    return buf.size();
}