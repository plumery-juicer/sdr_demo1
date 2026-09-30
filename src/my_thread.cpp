#include "my_thread.h"
#include <iostream>
#include <thread>
#include <atomic>

void writer_thread(RingBuffer& fifo , std::atomic<bool>& running_flag)
{
    uint8_t cnt = 0;
    while(running_flag.load())
    {
        bool ok = fifo.put(&cnt, 1);
        if(ok)
        {
            std::cout << "[WRITER] write byte: " << (int)cnt << "\n";
            cnt++;
        }
        else
        {
            std::cout << "[WRITER] fifo full\n";
        }
        // 1单位速度：100ms
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[WRITER] thread exit\n";
}


void reader_thread(RingBuffer& fifo , std::atomic<bool>& running_flag)
{
    while(running_flag.load())
    {
        uint8_t val;
        if(fifo.get(val))
        {
            std::cout << "[READER] read byte: " << (int)val << "\n";
        }
        else
        {
            std::cout << "[READER] fifo empty\n";
        }
        // 2单位速度：50ms
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "[READER] thread exit\n";
}