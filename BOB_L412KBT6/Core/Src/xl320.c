/**
 * @file xl320.c
 *
 * @brief Fonctions pour communiquer avec les servomoteurs Dynamixel XL-320
 *
 * Ce fichier contient les définitions des fonctions pour communiquer avec les
 * servomoteurs Dynamixel XL-320 via une interface UART.
 *
 * @date 7 Février 2023
 * @author Timothée Dessagne
 */

#include "xl320.h"
#include "usart.h"

/**
 * @brief Calcule le CRC pour la communication avec les servomoteurs Dynamixel
 *
 * Cette fonction calcule le CRC utilisé pour la communication avec les
 * servomoteurs Dynamixel XL-320.
 *
 * @param crc_accum Accumulateur de CRC
 * @param data_blk_ptr Pointeur vers les données pour le calcul du CRC
 * @param data_blk_size Taille des données pour le calcul du CRC
 *
 * @return Valeur du CRC calculé
 */

uint16_t update_crc(uint16_t crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
	uint16_t i, j;
	uint16_t crc_table[256] = {
			0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
			0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
			0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
			0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
			0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
			0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
			0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
			0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
			0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
			0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
			0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
			0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
			0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
			0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
			0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
			0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
			0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
			0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
			0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
			0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
			0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
			0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
			0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
			0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
			0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
			0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
			0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
			0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
			0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
			0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
			0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
			0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
	};
	for(j = 0; j < data_blk_size; j++)
	{
		i = ((uint16_t)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}

#define TX_BUFFER_LEN 14

uint8_t tx_buffer[TX_BUFFER_LEN];
// 4 octets pour le header
// 1 octet pour l'ID
// 2 octets pour la longueur
// 1 octet pour l'instruction
// 3 ou 4 octets pour les parametres
// 2 octets pour le CRC

/**
 * @brief Ecrit une valeur de 8 bits dans le registre d'un servomoteur
 *
 * Cette fonction écrit une valeur de 8 bits dans le registre d'un servomoteur
 * Dynamixel XL-320 spécifié par son ID et son adresse.
 *
 * @param id ID du servomoteur
 * @param address Adresse du registre
 * @param param Valeur à écrire dans le registre
 *
 * @return Code d'erreur (0 pour succès, autre valeur pour échec)
 */

uint8_t xl320_write8(uint8_t id, uint16_t address, uint8_t param)
{
	// P1 = Address LSB
	// P2 = Address MSB
	// P3 = Param

	tx_buffer[0] = 0xFF;
	tx_buffer[1] = 0xFF;
	tx_buffer[2] = 0xFD;
	tx_buffer[3] = 0x00;

	tx_buffer[4] = id;
	tx_buffer[5] = 0x06;	// Length LSB
	tx_buffer[6] = 0x00;	// Length MSB
	tx_buffer[7] = 0x03;	// Instruction "write"
	tx_buffer[8] = address & 0xFF; //adresse du registre ?
	tx_buffer[9] = (address>>8) & 0xFF; //decalage puis masque
	tx_buffer[10] = param;

	uint16_t crc = update_crc(0, (unsigned char *)tx_buffer, 11);

	tx_buffer[11] = crc & 0xFF;
	tx_buffer[12] = (crc >> 8) & 0xFF;

	//Envoie de la trame
	HAL_UART_Transmit(&huart2,tx_buffer,13,HAL_MAX_DELAY);


	return 0;

}

/**

 * @brief Écrit une valeur de 16 bits dans le registre d'un servomoteur Dynamixel XL-320.
 * Cette fonction écrit une valeur de 16 bits dans le registre d'un servomoteur Dynamixel XL-320 spécifié par son ID et son adresse.
 * @param id ID du servomoteur
 * @param address Adresse du registre
 * @param param Valeur à écrire dans le registre
 * @return Code d'erreur (0 pour succès, autre valeur pour échec)
 */

uint8_t xl320_write16(uint8_t id, uint16_t address, uint16_t param)
{
	// P1 = Address LSB
	// P2 = Address MSB
	// P3 = Param LSB
	// P4 = Param MSB

	tx_buffer[0] = 0xFF;
	tx_buffer[1] = 0xFF;
	tx_buffer[2] = 0xFD;
	tx_buffer[3] = 0x00;

	tx_buffer[4] = id;
	tx_buffer[5] = 0x07;	// Length LSB
	tx_buffer[6] = 0x00;	// Length MSB
	tx_buffer[7] = 0x03;	// Instruction "write"
	tx_buffer[8] = address & 0xFF;
	tx_buffer[9] = (address>>8) & 0xFF;
	tx_buffer[10] = param & 0xFF;
	tx_buffer[11] = (param >> 8) & 0xFF;

	uint16_t crc = update_crc(0, (unsigned char *)tx_buffer, 12);

	tx_buffer[12] = crc & 0xFF;
	tx_buffer[13] = (crc >> 8) & 0xFF;

	HAL_UART_Transmit(&huart2,tx_buffer,14,HAL_MAX_DELAY);

	return 0;
}

/**

@brief Communication avec un servomoteur XL320.

*	Cette fonction permet de communiquer avec un servomoteur XL320 en spécifiant l'angle,

la direction et la vitesse de rotation du moteur.

*	@param NumMot Numéro du servomoteur.

*	@param angle Angle à atteindre en degrés.

*	@param dir Direction de rotation (0 pour anti-horaire, 1 pour horaire).

*	@param speed Vitesse de rotation du moteur (1 pour basse, 2 pour moyenne, 3 pour haute).

*	@return Aucune valeur de retour.
*/

void servomotor_com(int NumMot, int angle, int dir, int speed){
int location = 0;
	switch (speed){
	case 1:
		xl320_write16(NumMot,32,300); // Speed
		break;
	case 2:
		xl320_write16(NumMot,32,512);
		break;

	case 3:
		xl320_write16(NumMot,32,700);
		break;


	default:
		xl320_write16(NumMot,32,300);
	}

	if (dir == 0){
		location = 512 - (3.4133*angle);
	}
	else if(dir == 1){
		location = 512 + (3.4133*angle);
	}

	if (location >= 0 || location <= 300){

		xl320_write16(NumMot,30,location); // Goal Position
	}
	else{
		xl320_write8(NumMot,25,1);
	}
}
