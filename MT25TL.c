/*
 * MT25TL.c
 *
 *  Created on: Aug 13, 2021
 *      Author: Janardhan (JD)
 */
#include "MT25TL.h"
extern DEV_ID_DATA FM1_ID;

/*
 *@brief function sets the Flash memory in write enable mode
 *
 *@retval null
 *
 *@param null
 *
 */
static void Write_Enable() {
	uint8_t command = WRITE_ENABLE & 0xFF;
	RESET_FMCS();
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);
	SET_FMCS();
}

static void Write_Disable() {
	uint8_t command = WRITE_DISABLE;
	RESET_FMCS();
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);
	SET_FMCS();
}

void SET_FMCS() {
	HAL_GPIO_WritePin(FM_CS_GPIO_Port, FM_CS_Pin, GPIO_PIN_SET);
	HAL_Delay(10);
}

void RESET_FMCS() {
	HAL_GPIO_WritePin(FM_CS_GPIO_Port, FM_CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(10);
}
/*
 * @brief		erases the specified sector
 *
 * @param	Sector_address	address in the sector to be erased
 * @param	Sector_Size		size of sector to be erased
 * 					64: 64-KB sector
 * 					32:	32-KB Sub-Sector
 * 					4:  4-KB  Sub-Sector
 *
 * @retval	none
 */
void Erase_Sector_FM(uint32_t Sector_address, uint8_t Sector_Size) {
	uint8_t command;
	uint8_t ADDR[4];
	ADDR[0] = (uint8_t) (Sector_address >> 24 & 0xFF);	// 0x _ _ 00 00 00
	ADDR[1] = (uint8_t) (Sector_address >> 16 & 0xFF);	// 0x 00 _ _ 00 00
	ADDR[2] = (uint8_t) (Sector_address >> 8 & 0xFF); 	// 0x 00 00 _ _ 00
	ADDR[3] = (uint8_t) (Sector_address >> 0 & 0xFF);	// 0x 00 00 00 _ _

	switch (Sector_Size) {
	case 64: // 64kb sector
		command = ERASE_4_BYTE_SECTOR & 0xFF; // 0xDC (erase operation command)
		break;
	case 32: // 32kb sub-sector
		command = ERASE_4_BYTE_32KB_SUBSECTOR & 0xFF; // 0x5C (erase operation command)
		break;
	case 4: //4kb sub-sector
		command = ERASE_4_BYTE_4KB_SUBSECTOR & 0xFF; // 0x21 (erase operation command)
		break;
	}

	Write_Enable(); // enabling Write mode

	RESET_FMCS();	// Lowering the CS Pin

	///////////////////////////////////////////////////////////////////////////
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);// Sector Erase Command (0xDC)
	HAL_SPI_Transmit(&FM_SPI, ADDR, 4, 100);// Address to erase the sector of
	///////////////////////////////////////////////////////////////////////////
	SET_FMCS();		// Setting CS pin back high
	HAL_Delay(5);
	return;
}
/*
 * @brief	Erases the die of the Flash Memory
 * @param	none
 * @retval	none
 */
void Erase_Die() {

}

/*
 *@brief Function to write 1 byte data to specified memory address
 *
 *@retval null
 *
 *@param address 32-bit address to write data
 *@param data  8-bit data to be written
 */
void Write_Byte(uint32_t address, uint8_t data) {
	uint8_t command;
	command = FM_4_BYTE_PAGE_PROGRAM;
	uint8_t ADDR[4];
	ADDR[0] = (uint8_t) (address >> 24);		// 0x _ _ 00 00 00
	ADDR[1] = (uint8_t) (address >> 16);		// 0x 00 _ _ 00 00
	ADDR[2] = (uint8_t) (address >> 8);		// 0x 00 00 _ _ 00
	ADDR[3] = (uint8_t) (address >> 0);		// 0x 00 00 00 _ _

	Write_Enable();		// Enabling Write Mode of Flash Memory

	RESET_FMCS();		// Pulling CS pin low

	////////////////////////////////////////////////////////////////////
	HAL_Delay(1);
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100); // Write Operation command 1 byte (0x02)
	HAL_SPI_Transmit(&FM_SPI, ADDR, 4, 100);		// Address to write data on
	HAL_SPI_Transmit(&FM_SPI, &data, 1, 100);		// Data to write
	HAL_Delay(1);
	////////////////////////////////////////////////////////////////////

	SET_FMCS();			// Pulling CS pin back high

	Write_Disable(); 	// Disabling the Write Mode of Flash Memory
	return;
}

/*
 *@brief 	Provides the startidng address of specified sector
 *
 *@param 	sector_num 	sector number of whose address is needed (MAX value 1023)
 *
 *@retval	32-bit starting address of the specified sector
 *
 */
uint32_t GET_Sector_Address(uint16_t sector_num) {
	uint32_t ADDRESS = 0x00;
	if (sector_num > 1023) {
		ADDRESS = 0x03FFFFFF;
		return ADDRESS;
	}

	ADDRESS = ADDRESS + (FMSector_size + 1) * sector_num;
	return ADDRESS;
}

/*
 * @brief	Provides Starting Address of Specified 32KB sub-sector
 *
 * @param	sector_num	sub-sector number of 32KB sub-sector (max 2047)
 *
 * @retval	32-bit starting address of the specified 32KB Sub-Sector
 *
 */

uint32_t GET_SubSector_Address_32KB(uint16_t sector_num) {
	uint32_t ADDRESS = 0x00;
	if (sector_num > 2047) {
		ADDRESS = 0x03FFFFFF;
		return ADDRESS;
	}
	ADDRESS = ADDRESS + (FMSubSector_32KB_size + 1) * sector_num;
	return ADDRESS;
}

/*
 * @brief	Provides Starting Address of Specified 4KB sub-sector
 *
 * @param	sector_num	sub-sector number of 4KB sub-sector (max 16383)
 *
 * @retval	32-bit starting address of the specified 4KB Sub-Sector
 *
 */

uint32_t GET_SubSector_Address_4KB(uint16_t sector_num) {
	uint32_t ADDRESS = 0x00;
	if (sector_num > 16383) {
		ADDRESS = 0x03FFFFFF;
		return ADDRESS;
	}
	ADDRESS = ADDRESS + (FMSubSector_4KB_size + 1) * sector_num;
	return ADDRESS;
}

/*
 * @brief	Reads data from specified address of flash memory
 *
 * @param	address		Location of the data to be read from Flash
 *
 * @retval	data_buffer	8-bit data read from Flash Memory
 *
 */

uint8_t Read_Byte(uint32_t address) {

	uint8_t data_buffer;
	uint8_t command = FM_4_BYTE_READ & 0xFF;
	uint8_t ADDR[4];

	ADDR[0] = (uint8_t) ((address >> 24) & 0xFF);		// 0x _ _ 00 00 00
	ADDR[1] = (uint8_t) ((address >> 16) & 0xFF);	// 0x 00 _ _ 00 00
	ADDR[2] = (uint8_t) ((address >> 8) & 0xFF);		// 0x 00 00 _ _ 00
	ADDR[3] = (uint8_t) ((address >> 0) & 0xFF);		// 0x 00 00 00 _ _

	RESET_FMCS();	// Pulling the CS pin Low
	/////////////////////////////////////////////////////////////
	HAL_Delay(1);
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);	//read data command 0x03
	HAL_SPI_Transmit(&FM_SPI, ADDR, 4, 100);
	HAL_SPI_Receive(&FM_SPI, &data_buffer, 1, 100);
	HAL_Delay(1);
	/////////////////////////////////////////////////////////////
	SET_FMCS();		// Pulling the CS pin High

	return data_buffer;	// Returning data read from the Flash Memory
}

/*
 * @brief	Reads ID data of the Flash memory, static data programmed at factory
 *
 * @param	*pRxBuff	Pointer to the temporary data location
 * @param	pDEV_STR	pointer to structure containing the Flash memory device ID
 *
 * @retval	none
 *
 */

void Read_ID(volatile uint8_t *pRxBuff, DEV_ID_DATA *pDEV_STR) {
	DEV_ID_DATA BUFF;
	uint8_t command;
	command = READ_ID & 0xFF;

	RESET_FMCS();		// pulling the cs pin Low

	////////////////////////////////////////////////////////////////////////
	HAL_Delay(1);
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);	// read ID command (0x9E)
	HAL_Delay(1);
	HAL_SPI_Receive(&FM_SPI, (uint8_t*) pRxBuff, 20, 1000);
	HAL_Delay(1);
	/////////////////////////////////////////////////////////////////////////
	SET_FMCS();			// Pulling the CS Pin High

	BUFF.MFR_ID = *pRxBuff++;
	BUFF.MEM_TYP = *pRxBuff++;
	BUFF.MEM_CAPACITY = *pRxBuff++;
	BUFF.REMAINING_BYTES = *pRxBuff++;
	BUFF.EXTENDED_DEV_ID = *pRxBuff++;
	BUFF.DEV_CONF_INFO = *pRxBuff++;
	for (int i = 0; i < 14; i++) {
		BUFF.UID_CODE[i] = *pRxBuff++;
	}
	*pDEV_STR = BUFF;
	return;
}

/*
 * @brief	Reads and returns the status of flash memory in latched in register
 * @param	None
 * @retval	Status 	Flash memory status values in status register
 */
int8_t Status_Reg_Read() {
	uint8_t status;
	uint8_t command;
	command = READ_STATUS_REG & 0xFF;

	RESET_FMCS();		//pulling CS pin Low
	////////////////////////////////////////////////////////
	HAL_SPI_Transmit(&FM_SPI, &command, 1, 100); // Read Status register command (0x05)
	HAL_Delay(1);
	HAL_SPI_Receive(&FM_SPI, &status, 1, 100);
	HAL_Delay(1);
	/////////////////////////////////////////////////////////
	SET_FMCS();			// Pulling CS Pin Back High
	return status;
}

/*
 * @brief	Reads the data bits on the commanded register
 *
 * @param	command	command byte specifying which register to read
 *
 * @retval	uint16_t data in the register
 * 				only NON-VOLATILE CONFIGURATION REGISTER HAS 16-bit data
 * 				other register has 8-bit data
 */
uint16_t Read_Register(uint8_t command) {
	uint16_t reg_data;
	if (command == READ_NONVOLATILE_CONFIG_REG) {
		uint8_t data_buf[2];

		RESET_FMCS();
		HAL_Delay(1);
		///////////////////////////////////////////////////////////////////////////
		HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);// Sector Erase Command (0xDC)
		HAL_SPI_Receive(&FM_SPI, data_buf, 2, 100);	// Address to erase the sector of
		///////////////////////////////////////////////////////////////////////////
		HAL_Delay(1);
		SET_FMCS();
		reg_data = (data_buf[0] << 8 & 0xFF) | data_buf[1];
		return reg_data;
	} else {

		uint8_t data = 0;

		RESET_FMCS();
		HAL_Delay(1);
		///////////////////////////////////////////////////////////////////////////
		HAL_SPI_Transmit(&FM_SPI, &command, 1, 100);// Sector Erase Command (0xDC)
		HAL_SPI_Receive(&FM_SPI, &data, 1, 100);// Address to erase the sector of
		///////////////////////////////////////////////////////////////////////////
		HAL_Delay(1);
		SET_FMCS();
		reg_data = data;
		return reg_data;
	}

}
