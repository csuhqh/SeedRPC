#ifndef SRC_NET_TCP_BUFFER_H
#define SRC_NET_TCP_BUFFER_H
#include <memory>
#include <vector>

namespace seedrpc {
/*
每次读/写都会重新调整指针位置
*/
class TcpBuffer {
public:
    typedef std::shared_ptr<TcpBuffer> s_ptr;
    TcpBuffer(int size);
    //返回可读字节数
    int readAble(); 
    //返回可写字节数
    int writeAble(); 
    int readIndex();
    int writeIndex();
    void writeToBuffer(const char* buf, int size);
    void readFromBuffer(std::vector<char>&re, int size);
    void expandBuffer(int new_size);
    //使得readIndex=0, writeIndex=dataSize
    void adjustBuffer();
    void moveReadIndex(int size);
    void moveWriteIndex(int size);
private:
    int m_read_index{0};
    int m_write_index{0};
    int m_size{0};
public:
    std::vector<char> m_buffer;
};

}  // namespace seedrpc

#endif