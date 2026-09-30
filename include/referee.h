#ifndef __REFEREE_H
#define __REFEREE_H

#include <cstdint>
#include <vector>

typedef struct __attribute__((packed)) {
    uint8_t  sof;
    uint16_t data_length;
    uint8_t  seq;
    uint8_t  crc8;
} FrameHeader_t;

union FrameHeader_Union
{
    FrameHeader_t _frame_header;
    uint8_t bytes[5];   // 直接按字节访问整个5字节帧头
};

typedef struct __attribute__((packed)) {
    uint16_t cmd_id;
    std::vector<uint8_t> data;
    uint16_t crc16;
} Framedata_t;

class referee_t
{
    public:
        referee_t(){};
        ~referee_t(){};

        void init();
        void process_data(const uint8_t data, size_t length);
    private:
        std::vector<uint8_t> big_rx_buf;
        
};



#endif