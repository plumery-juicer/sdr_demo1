#include "referee.h"

void referee_t::init()
{
    rx_buf.clear();
    current_state = State::WAIT_FOR_SOF;
}

void referee_t::process_data(const uint8_t data, size_t length)
{
    switch (current_state)
    {
        case State::WAIT_FOR_SOF:
            printf("Waiting for SOF...当前data是%02X\n", data);
            if(data == REFEREE_SOF)
            {
                printf("SOF received.\n");
                rx_buf.push_back(data);
                current_state = State::WAIT_FOR_HEADER;
            }
            break;
        case State::WAIT_FOR_HEADER:
            printf("Waiting for header...\n");
            rx_buf.push_back(data);
            if(rx_buf.size() == FRAME_HEADER_LEN)
            {
                printf("Header received, verifying CRC8...\n");
                if(Get_CRC8_Check_Sum(rx_buf.data(),FRAME_HEADER_LEN-1,CRC8_INIT)==data)
                {
                    printf("CRC8 verification passed.\n");
                    data_length = (static_cast<uint16_t>(rx_buf[2]) << 8) | rx_buf[1];
                    printf("Data length: %d\n", data_length);
                    current_state=State::WAIT_FOR_DATA;
                }
                else
                {
                    current_state=State::WAIT_FOR_SOF;
                    rx_buf.clear();
                    data_length=0;
                }
            }
            break;

        case State::WAIT_FOR_DATA:
            printf("Waiting for data...\n");
            printf("当前数组长度是%d\n", rx_buf.size());
            printf(".当前data是%02X\n", data);
            rx_buf.push_back(data);
            if(rx_buf.size()==(FRAME_HEADER_LEN+FRAME_DATA_LEN+data_length))
            {                                                                   
                printf("Data received, verifying CRC16,数据长度是%d...\n", data_length);
                if(Verify_CRC16_Check_Sum(rx_buf.data(),FRAME_HEADER_LEN+FRAME_DATA_LEN+data_length))
                {
                    printf("CRC16 verification passed.\n");
                    uint16_t cmd_id = (static_cast<uint16_t>(rx_buf[FRAME_HEADER_LEN+1]) << 8) | rx_buf[FRAME_HEADER_LEN];
                    printf("Command ID: 0x%04X\n", cmd_id);
                    switch (cmd_id)
                    {
                        case static_cast<uint16_t>(cmd_id::radar_id1):
                            printf("Processing radar_id1 data...\n");
                            safe_copy(radar_info.id1, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id1));
                            break;
                        case static_cast<uint16_t>(cmd_id::radar_id2):
                            safe_copy(radar_info.id2, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id2));
                            break;
                        case static_cast<uint16_t>(cmd_id::radar_id3):

                            safe_copy(radar_info.id3, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id3));
                            break;
                        case static_cast<uint16_t>(cmd_id::radar_id4):

                            safe_copy(radar_info.id4, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id4));
                            break;
                        case static_cast<uint16_t>(cmd_id::radar_id5):

                            safe_copy(radar_info.id5, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id5));
                            break;
                        case static_cast<uint16_t>(cmd_id::radar_id6):
                            safe_copy(radar_info.id6, &rx_buf[FRAME_HEADER_LEN+2], sizeof(radar_info.id6));
                            break;
                        default:
                            printf("Unknown command ID: 0x%04X\n", cmd_id);
                            break;
                    }
                    rx_buf.clear();
                    current_state=State::WAIT_FOR_SOF;
                    data_length=0;
                    printf("当前位置是(%d,%d)\n", radar_info.id1.robot_1_x, radar_info.id1.robot_1_y);
                }
                else
                {
                    current_state=State::WAIT_FOR_SOF;
                    rx_buf.clear();
                    data_length=0;
                }
            }
            
            break;
        default:
            break;
    }
}