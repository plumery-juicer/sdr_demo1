#ifndef __REFEREE_CONFIG_H
#define __REFEREE_CONFIG_H

#include <cstdint>

const uint8_t REFEREE_SOF = 0xA5;
const uint8_t FRAME_HEADER_LEN = 5;
const uint8_t FRAME_DATA_LEN =4;

enum class cmd_id : uint16_t
{
    radar_id1 = 0x0a01,//机器人位置信息
    radar_id2 = 0x0a02,//机器人血量信息
    radar_id3 = 0x0a03,//机器人允许发弹量
    radar_id4 = 0x0a04,//经济和各种地形增益占领情况
    radar_id5 = 0x0a05,//各机器人本身增益情况
    radar_id6 = 0x0a06,//密钥
};

struct radar_id1_data
{
    uint16_t robot_1_x;//英雄
    uint16_t robot_1_y;
    uint16_t robot_2_x;//工程
    uint16_t robot_2_y;
    uint16_t robot_3_x;//步兵
    uint16_t robot_3_y;
    uint16_t robot_4_x;
    uint16_t robot_4_y;
    uint16_t robot_6_x;//无人机
    uint16_t robot_6_y;
    uint16_t robot_7_x;//哨兵
    uint16_t robot_7_y;
    
} __attribute__((packed));

struct radar_id2_data
{
    uint16_t robot_1_hp;//英雄
    uint16_t robot_2_hp;//工程
    uint16_t robot_3_hp;//步兵
    uint16_t robot_4_hp;
    uint16_t robot_5_hp;//保留
    uint16_t robot_7_hp;//哨兵
    
} __attribute__((packed));

struct radar_id3_data
{
    uint16_t robot_1_bullet;//英雄
    uint16_t robot_3_bullet;//步兵
    uint16_t robot_4_bullet;
    uint16_t robot_6_bullet;//无人机
    uint16_t robot_7_bullet;//哨兵
    
} __attribute__((packed));

struct radar_id4_data
{
    uint16_t enemy_gold_last;//对方剩余经济
    uint16_t enemy_gold_sum;//对方累计经济
    uint32_t enemy_terrain;//对方占领的地形情况
    
} __attribute__((packed));

struct radar_id5_data
{
    uint8_t robot_1_hp_buff;//英雄回血buff
    uint16_t robot_1_cooling_buff;//
    uint8_t robot_1_def_buff;//防御增益
    uint8_t robot_1_undef_buff;//防御减益
    uint16_t robot_1_ad_buff;//攻击增益

    uint8_t robot_2_hp_buff;//工程回血buff
    uint16_t robot_2_cooling_buff;//
    uint8_t robot_2_def_buff;//防御增益
    uint8_t robot_2_undef_buff;//防御减益
    uint16_t robot_2_ad_buff;//攻击增益

    uint8_t robot_3_hp_buff;//步兵回血buff
    uint16_t robot_3_cooling_buff;//
    uint8_t robot_3_def_buff;//防御增益
    uint8_t robot_3_undef_buff;//防御减益
    uint16_t robot_3_ad_buff;//攻击增益

    uint8_t robot_4_hp_buff;//步兵回血buff
    uint16_t robot_4_cooling_buff;//
    uint8_t robot_4_def_buff;//防御增益
    uint8_t robot_4_undef_buff;//防御减益
    uint16_t robot_4_ad_buff;//攻击增益

    uint8_t robot_7_hp_buff;//哨兵回血buff
    uint16_t robot_7_cooling_buff;//
    uint8_t robot_7_def_buff;//防御增益
    uint8_t robot_7_undef_buff;//防御减益
    uint16_t robot_7_ad_buff;//攻击增益

    uint8_t robot_7_statu_buff;//对方哨兵姿态

    uint8_t robot_1_state;//对方英雄主要状态
    uint8_t robot_2_state;
    uint8_t robot_3_state;
    uint8_t robot_4_state;
    uint8_t robot_7_state;
} __attribute__((packed));

struct radar_id6_data
{
    uint8_t key[6];
} __attribute__((packed));

struct radar_data
{
    radar_id1_data id1;
    radar_id2_data id2;
    radar_id3_data id3;
    radar_id4_data id4;
    radar_id5_data id5;
    radar_id6_data id6;
} __attribute__((packed));

#endif