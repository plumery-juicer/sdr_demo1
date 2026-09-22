#include <iostream>
#include <atomic>
#include <thread>
#include <csignal>
#include <zmq.hpp>
#include <cstdint>
#include <vector>
#include <cstddef>

// 配置
const uint16_t SYNC_WORD = 0xD4D3;       // 你的同步字 8bit示例
const int SYNC_TOLERANCE = 1;         // 汉明距离容错，允许最多1bit错误
const size_t RING_BUF_SIZE = 4096;    // 环形缓存，至少大于同步字长度+最大帧长
const size_t ZMQ_TagOffest = 8;      // ZMQ消息中，数据比特的起始偏移量（字节数），根据实际情况调整

// 环形缓冲区，存储收到的raw bit：只存0/1，每个元素代表1bit
std::vector<uint8_t> ring_buf(RING_BUF_SIZE);
size_t ring_wr_ptr = 0;   // 写指针
size_t ring_rd_ptr = 0;   // 读指针

// 滑动移位寄存器：保存最近N比特，这里同步字8bit，用uint16足够
uint16_t shift_reg = 0;


// 原子退出标记，多线程安全
std::atomic<bool> g_running{true};

int hamming_dist(uint16_t a, uint16_t b);
void ring_write(const uint8_t* msg_data, size_t msg_len);

// Ctrl‑C 信号回调
void sigint_handler(int sig)
{
    (void)sig;
    std::cout << "\nGot Ctrl‑C, requesting exit..." << std::endl;
    g_running = false;
}

void zmq_worker(const std::string& endpoint)
{
    try
    {
        zmq::context_t context(1);
        zmq::socket_t subscriber(context, ZMQ_SUB);

        subscriber.connect(endpoint);
        subscriber.set(zmq::sockopt::subscribe, "");

        // ZMQ poller：等待消息，设置超时 100ms，避免永久阻塞
        zmq::pollitem_t items[] = {
            { subscriber.handle(), 0, ZMQ_POLLIN, 0 }
        };

        std::cout << "Worker: connected to " << endpoint << std::endl;

        while(g_running)
        {
            // poll 超时 100ms；g_running=false 时循环可以及时退出
            zmq::poll(items, 1, std::chrono::milliseconds(100));
            if (items[0].revents & ZMQ_POLLIN)
            {
                zmq::message_t msg;
                auto ret = subscriber.recv(msg, zmq::recv_flags::none);
                if (!ret.has_value())
                {
                    continue;
                }
                size_t bytes = msg.size();
                std::cout << "Recv: " << bytes << " bytes\n";


                const uint8_t* p = static_cast<const uint8_t*>(msg.data());
                size_t len = msg.size();

                // 1) 原始十六进制打印（最常用，看二进制包）
                std::cout << "recv len=" << len << " bytes: ";
                for(size_t i=0; i<len; i++)
                {
                    printf("%d ", p[i]);
                }
                std::cout << std::endl;

                // 裸数据指针，直接拿原始缓冲区，不做类型转换
                ring_write(static_cast<uint8_t*>(msg.data()), msg.size());

                while( ring_rd_ptr != ring_wr_ptr )
                {
                    uint8_t bit = ring_buf[ring_rd_ptr];
                    ring_rd_ptr = (ring_rd_ptr + 1) % RING_BUF_SIZE;

                    // 滑动移位寄存器：压入新bit，保存最近8bit
                    shift_reg = ((shift_reg << 1) | bit) & 0xFFFF;

                    // 同步字检测，带汉明容错
                    int dist = hamming_dist(shift_reg, SYNC_WORD);
                    if(dist <= SYNC_TOLERANCE)
                    {
                        // ========= 同步字命中！ =========
                        // shift_reg就是匹配到的同步字，接下来开始收payload比特
                        printf("Sync found! Hamming dist=%d\n", dist);

                        // 示例：接下来读取N个payload比特，你自己定义帧长度
                        const size_t PAYLOAD_BIT_CNT = 32;
                        std::vector<uint8_t> payload_bits;
                        payload_bits.reserve(PAYLOAD_BIT_CNT);

                        for(size_t b=0; b < PAYLOAD_BIT_CNT; b++)
                        {
                            // 环形缓冲区没有数据，需要等待下一条zmq消息到达
                            if(ring_rd_ptr == ring_wr_ptr)
                            {
                                break; // 暂时退出，等待下一轮zmq消息
                            }
                            uint8_t pb = ring_buf[ring_rd_ptr];
                            ring_rd_ptr = (ring_rd_ptr + 1) % RING_BUF_SIZE;
                            payload_bits.push_back(pb);
                        }

                        // payload_bits：每个元素0/1，一比特一个元素
                        // 【可选】把bit数组打包成真实uint8字节：8个bit拼成1字节
                        std::vector<uint8_t> payload_bytes;
                        for(size_t i=0; i < payload_bits.size(); i+=8)
                        {
                            uint8_t byte = 0;
                            for(int k=0;k<8;k++)
                            {
                                if(i+k >= payload_bits.size()) break;
                                byte = (byte <<1) | payload_bits[i+k];
                            }
                            payload_bytes.push_back(byte);
                        }

                        for(int i=0; i<payload_bytes.size(); i++)
                        {
                            printf("Payload byte[%d]: %d\n", i, payload_bytes[i]);
                        }
                        // payload_bytes 就是最终解析出来的原始帧字节
                    }
                }
                
                
            }
        }

        // 优雅关闭：socket 先关闭，context 自动析构
        std::cout << "Worker: shutting down socket..." << std::endl;
        subscriber.close();
    }
    catch(const zmq::error_t& e)
    {
        std::cerr << "ZMQ Exception: " << e.what() << std::endl;
        g_running = false;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        g_running = false;
    }
}

int main()
{
    // 注册 Ctrl‑C 信号
    std::signal(SIGINT, sigint_handler);

    std::thread worker_thread(zmq_worker, "tcp://127.0.0.1:5555");

    // 主线程等待工作线程结束
    if(worker_thread.joinable())
    {
        worker_thread.join();
    }

    std::cout << "Program exit normally.\n";
    return 0;
}

int hamming_dist(uint16_t a, uint16_t b)
{
    return __builtin_popcount(a ^ b);
}

// 环形buffer写入：把zmq收到的msg（全是0x00/0x01）压入环形缓存
void ring_write(const uint8_t* msg_data, size_t msg_len)
{
    for(size_t i=ZMQ_TagOffest; i<msg_len; i++)
    {
        uint8_t bit = msg_data[i] & 0x01; // msg只有0x00或0x01
        ring_buf[ring_wr_ptr] = bit;
        ring_wr_ptr = (ring_wr_ptr + 1) % RING_BUF_SIZE;
        // 写满覆盖读指针，工程上RING_BUF_SIZE设置足够大就不会触发
        if(ring_wr_ptr == ring_rd_ptr)
            ring_rd_ptr = (ring_rd_ptr +1) % RING_BUF_SIZE;
    }
}