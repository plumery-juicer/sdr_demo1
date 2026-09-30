#ifndef __FIFO_H
#define __FIFO_H
#include <cstdint>
#include <cstring>
#include <algorithm>

// 环形FIFO，字节粒度，适合串口/射频分片接收
class RingBuffer
{
public:
    RingBuffer(size_t buf_size)
        : m_buf_size(buf_size), m_wr(0), m_rd(0)
    {
        m_buf = new uint8_t[m_buf_size];
    }
    ~RingBuffer()
    {
        delete[] m_buf;
    }

    // 返回当前FIFO里面有效字节数量
    size_t available() const
    {
        if(m_wr >= m_rd)
            return m_wr - m_rd;
        else
            return m_buf_size - (m_rd - m_wr);
    }

    // FIFO是否满
    bool isFull() const
    {
        return available() == (m_buf_size - 1);
    }

    // FIFO是否空
    bool isEmpty() const
    {
        return m_wr == m_rd;
    }

    // 批量写入：硬件收到15字节调用这个，n=15
    bool put(uint8_t* in, size_t n)
    {
        if(available() + n >= m_buf_size)
        {
            // 溢出，数据丢失，业务层可以加计数告警
            return false;
        }
        for(size_t i = 0; i < n; i++)
        {
            m_buf[m_wr] = in[i];
            m_wr = (m_wr + 1) % m_buf_size;
        }
        return true;
    }

    // 取出单个字节，解析状态机调用，返回false为空
    bool get(uint8_t& out_byte)
    {
        if(isEmpty())
            return false;
        out_byte = m_buf[m_rd];
        m_rd = (m_rd + 1) % m_buf_size;
        return true;
    }

    // 丢弃n个字节，同步丢失、错误帧的时候用
    void discard(size_t n)
    {
        size_t real_drop = std::min(n, available());
        m_rd = (m_rd + real_drop) % m_buf_size;
    }

    void clear()
    {
        m_wr = m_rd = 0;
    }

private:
    uint8_t* m_buf;
    size_t m_buf_size;
    size_t m_wr;    // 写指针
    size_t m_rd;    // 读指针
};


#endif