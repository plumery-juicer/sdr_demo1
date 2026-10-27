#ifndef __REFEREE_H
#define __REFEREE_H

#include <cstdint>
#include <vector>
#include "crc_config.h"
#include "referee_config.h"
#include <cstdio>
#include <cstring>

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

        enum class State
        {
            WAIT_FOR_SOF,
            WAIT_FOR_HEADER,
            WAIT_FOR_DATA
        };

        void init();
        void process_data(const uint8_t data, size_t length);

        radar_data radar_info;
    private:
        std::vector<uint8_t> rx_buf;
        State current_state = State::WAIT_FOR_SOF;
        uint16_t data_length = 0;

        template <typename T>
        static void safe_copy(T &target, const uint8_t *src,
                          const uint16_t packet_data_len)
        {
            // Safety copy to prevent buffer overflows
            const size_t copy_len =
            packet_data_len < sizeof(T) ? packet_data_len : sizeof(T);
            memcpy(&target, src, copy_len);
        }
        
};



#endif