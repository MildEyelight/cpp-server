#pragma once
#include <string>

class InetAddress{
private:
    std::string _ip;
    int _port;
public:
    InetAddress(const std::string& ip, const int port):
        _ip(ip), _port(port){}
    
    const std::string& get_ip() const{
        return _ip;
    }

    const int get_port() const{
        return _port;
    }
};