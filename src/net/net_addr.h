#ifndef SRC_NET_NET_ADDR_H
#define SRC_NET_NET_ADDR_H

#include <arpa/inet.h>
#include "src/common/log.h"
#include <string.h>

#include <memory>
#include <string>

namespace seedrpc {

// 地址基类
class NetAddr {
public:
    typedef std::shared_ptr<NetAddr> s_ptr;
    virtual sockaddr* getSockAddr() = 0;
    virtual socklen_t getSocketLen() = 0;
    virtual int getFamily() = 0;
    virtual std::string toString() = 0;
    virtual bool checkValid() = 0;  // 检查ip地址是否有效
};

// ip4地址
class IPNetAddr : public NetAddr {
public:
    static bool checkValid(const std::string& addr) {
        size_t i = addr.find_first_of(":");
        if (i == std::string::npos)
            return false;
        std::string ip = addr.substr(0, i);
        std::string port = addr.substr(i + 1, addr.size() - i - 1);
        if (ip.empty() || port.empty())
            return false;
        int iport = std::atoi(port.c_str());
        if (iport <= 0 || iport > 65536)
            return false;
        return true;
    }
    IPNetAddr(const std::string& ip, uint16_t port) : m_ip(ip), m_port(port) {
        memset(&m_addr, 0, sizeof(m_addr));
        m_addr.sin_addr.s_addr = inet_addr(ip.c_str());
        m_addr.sin_family = AF_INET;
        m_addr.sin_port = htons(port);  // host to network short
    }
    IPNetAddr(const std::string& addr_ip_port) {
        size_t i = addr_ip_port.find_first_of(':');
        if (i == std::string::npos) {
            ERRORLOG("invalid ip4 addr : %s", addr_ip_port.c_str());
            return;
        }
        m_ip = addr_ip_port.substr(0, i);
        std::string port = addr_ip_port.substr(i + 1, addr_ip_port.size() - i - 1);
        printf("inputs: ip = %s, port = %s\n", m_ip.c_str(), port.c_str());
        m_port = htons(atoi(port.c_str()));
        m_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());
        m_addr.sin_port = m_port;
        m_addr.sin_family = AF_INET;
    }

    sockaddr* getSockAddr() {
        return (sockaddr*)&m_addr;
    }

    socklen_t getSocketLen() {
        return sizeof(m_addr);
    }

    int getFamily() {
        return AF_INET;
    }
    IPNetAddr(sockaddr_in addr) : m_addr(addr) {
        m_ip = std::string(inet_ntoa(m_addr.sin_addr));
        m_port = ntohs(m_addr.sin_port);
    }

    std::string toString() {
        return m_ip + ":" + std::to_string(m_port);
    }

    bool checkValid() {
        if (m_ip.empty())
            return false;
        if (m_port < 0 || m_port > 65535)
            return false;
        if (inet_addr(m_ip.c_str()) == INADDR_NONE) {
            return false;
        }
        return true;
    }

private:
    std::string m_ip;
    uint16_t m_port;
    sockaddr_in m_addr;
};

}  // namespace seedrpc

#endif