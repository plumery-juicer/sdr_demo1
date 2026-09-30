#ifndef __MY_THREAD_H
#define __MY_THREAD_H

#include "fifo.h"
#include <atomic>

void writer_thread(RingBuffer& fifo, std::atomic<bool>& running_flag);
void reader_thread(RingBuffer& fifo, std::atomic<bool>& running_flag);

#endif