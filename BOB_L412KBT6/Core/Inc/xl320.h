/*
 * xl320.h
 *
 *  Created on: Feb 7, 2023
 *      Author: clega
 */

#ifndef INC_XL320_H_
#define INC_XL320_H_

#include <stdint.h>

uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
uint8_t xl320_write8(uint8_t id, uint16_t address, uint8_t param);
uint8_t xl320_write16(uint8_t id, uint16_t address, uint16_t param);

void servomotor_com(int NumMot, int angle, int dir, int speed);


#endif /* INC_XL320_H_ */
