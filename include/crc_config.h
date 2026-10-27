#ifndef __CRC_CONFIG_H
#define __CRC_CONFIG_H

#include <cstdint>
#include <vector>
//crc8 generator polynomial:G(x=x8+x5+x4+1 
extern const unsigned char CRC8_INIT; 
extern const unsigned char CRC8_TAB[256]; 
extern uint16_t CRC_INIT; 
extern const uint16_t wCRC_Table[256];

unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage,unsigned int dwLength,unsigned char ucCRC8) ;
unsigned char Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength) ;
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
unsigned char Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);


#endif