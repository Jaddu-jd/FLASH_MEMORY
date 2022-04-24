/*
 * MT25TL.h
 *
 *  Created on: Aug 12, 2021
 *      Author: User
 */
#pragma once

#ifndef MT25TL_H_
#define MT25TL_H_

#include "main.h"

//command codes  definition for MT25TL Flash memory

/*COMMAND CODES OF MT25TL START */
//Software Reset operations
#define RESET_ENABLE						0x66
#define RESET_MEMORY						0x99

//READ ID Operations
#define READ_ID								0x9E
#define MULTIPLE_IO_READ_ID					0xAF
#define READ_SERIAL_FLASH_DISO_PARAM		0x5A

//READ memory operation command
#define READ								0x03
#define FAST_READ							0x0B
#define DUAL_OUTPUT_FAST_READ				0x3B
#define DUAL_IO_FAST_READ					0xBB
#define QUAD_OUTPUT_FAST_READ				0x6B
#define QUAD_IO_FAST_READ					0xEB
#define DTR_FAST_READ						0x0D
#define DTR_DUAL_OUTPUT_FAST_READ			0x3D
#define DTR_DUAL_IO_FAST_READ				0xBD
#define DTR_QUAD_OUTPUT_FAST_READ			0x6D
#define DTR_QUAD_IO_FAST_READ				0xED
#define QUAD_IO_WORD_READ					0xE7

//READ memory Operations with 4-Byte Address

#define FM_4_BYTE_READ						0x13
#define FM_4_BYTE_FAST_READ					0x0C
#define FM_4_BYTE_DUAL_OUTPUT_FAST_READ		0x3C
#define FM_4_BYTE_DUAL_IO_FAST_READ			0xBC
#define FM_4_BYTE_QUAD_OUTPUT_FAST_READ		0x6C
#define FM_4_BYTE_QUAD_IO_FAST_READ			0xEC
#define FM_4_BYTE_DTR_FAST_READ				0x0E
#define FM_4_BYTE_DTR_DUAL_IO_FAST_READ		0xBE
#define FM_4_BYTE_DTR_QUAD_IO_FAST_READ		0xEE

//WRITE Operations

#define WRITE_ENABLE						0x06
#define WRITE_DISABLE						0x04

//READ REGISTERS Operations

#define READ_STATUS_REG						0x05
#define READ_FLAG_STATUS_REG				0x70
#define READ_NONVOLATILE_CONFIG_REG			0xB5
#define READ_VOLATILE_CONFIG_REG			0x85
#define READ_ENHANCED_VOLATILE_CONFIG_REG	0x65
#define READ_EXTENDED_ADDRESS_REG			0xC8
#define READ_GENERAL_PURPOSE_READ_REG		0x96

//WRITE REGISTER Operations
#define WRITE_STATUS_REG					0x01
#define WRITE_NONVOLATILE_CONFIG_REG		0xB1
#define WRITE_VOLATILE_CONFIG_REG			0x81
#define WRITE_ENHANCED_VOLATILE_CONFIG_REG	0x61
#define WRITE_EXTENDED_ADDRESS_REG			0xC5

//CLEAR FLAG STATES REGISTER Operations
#define CLEAR_FLAG_STATUS_REG				0X50

//PROGRAM OPERATIONS
#define PAGE_PROGRAM						0x02
#define DUAL_INPUT_FAST_PROGRAM				0xA2
#define EXTENDED_DUAL_INPUT_FAST_PROGRAM	0xD2
#define QUAD_INPUT_FAST_PROGRAM				0x32
#define EXTENDED_QUAD_INPUT_FAST_PROGRAM	0x38

//PROGRAM Operation with 4-BYTE address
#define FM_4_BYTE_PAGE_PROGRAM 						0x12
#define FM_4_BYTE_QUAD_INPUT_FAST_PROGRAM			0x34
#define FM_4_BYTE_QUAD_INPUT_EXTENDED_FAST_PROGRAM	0x3E

//ERASE OPERATIONS
#define ERASE_32KB_SUBSECTOR				0x52
#define ERASE_4KB_SUBSECTOR					0x20
#define ERASE_SECTOR						0xD8
#define ERASE_512Mb_BULK					0xC7
#define ERASE_512MBBLUK						0x60

//used only for MT25QL01GBB Flash
#define DIE_ERASE							0xC4

//ERASE Operations with 4-byte address
#define ERASE_4_BYTE_SECTOR					0xDC
#define ERASE_4_BYTE_4KB_SUBSECTOR			0x21
#define ERASE_4_BYTE_32KB_SUBSECTOR			0x5C

//SUSPEND/RESUME Operations
#define PROGRAM_ERASE_SUSPEND				0x75
#define PROGRAM_ERASE_RESUME				0x7A

//ONE-TIME PROGRAMMABLE (OTP) operations
#define READ_OTP_ARRAY						0x4B
#define PROGRAM_OTP_ARRAY					0x42

//4-BYTE ADDRESS MODE Operations
#define ENTER_4_BYTE_ADDRESS_MODE			0xB7
#define EXIT_4_BYTE_ADDRESS_MODE			0xE9

//QUAD PROTOCOL OPERATIONS
#define ENTER_QUAD_IO_MODE					0x35
#define RESET_QUAD_IO_MODE					0xF5

//DEEP POWER-DOWN Operations
#define ENTER_DEEP_POWER_DOWN				0xB9
#define RELEASE_FROM_DEEP_POWER_DOWN		0xAB

//ADVANCED SECTOR PROTECTION OPERATIONS
#define READ_SECTOR_PROTECTION				0x2D
#define PROGRAM_SECTOR_PROTECTION			0x2C
#define	READ_VOLATILE_LOCK_BITS				0xE8
#define WRITE_VOLATILE_LOCK_BITS			0xE5
#define READ_NONVOLATILE_LOCK_BITS			0xE2
#define WRITE_NONVOLATILE_LOCK_BITS			0xE3
#define ERASE_NONVOLATILE_LOCK_BITS			0xE4
#define READ_GLOBAL_FREEZE_BIT				0xA7
#define WRITE_GLOBAL_FREEZE_BIT				0xA6
#define READ_PASSWORD						0x27
#define WRITE_PASSWORD						0x28
#define UNLOCK_PASSWORD						0x29

//ADVANCED SECTOR PROTECTION Operations with 4-BYTE Address
#define READ_4_BYTE_VOLATILE_LOCK_BITS		0xE0
#define WRITE_4_BYTE_VOLATILE_LOCK_BITS		0xE1

//ADVANCED FUNCTION INTERFACE Operations

#define INTERFACE_ACTIVATION				0x9B
#define CYCLIC_REDUNDANCY_CHECK				0x9B
#define CHECK_REDUNDANCY_CYCLIC				0x27

/*COMMANDS OF MT25TL END */

//Sectors ADDRESSES for FLASH MEMORY of DIFF SECTOR
#define FMSector0_start 0x00
#define FMSector_size 0xFFFF

#define FMSubSector0_32KB_start 0x00
#define FMSubSector_32KB_size 0x7FFF

#define FMSubSector0_4KB_start 0x00
#define FMSubSector_4KB_size 0x0FFF

typedef struct {
	uint8_t MFR_ID;
	uint8_t MEM_TYP;
	uint8_t MEM_CAPACITY;
	uint8_t REMAINING_BYTES;
	uint8_t EXTENDED_DEV_ID;
	uint8_t DEV_CONF_INFO;
	uint8_t UID_CODE[14];
} DEV_ID_DATA;

/* Functions for FLASH OPERATIONS */
void SET_FMCS();
void RESET_FMCS();

void Erase_Sector_FM(uint32_t Sector_address, uint8_t Sector_Size);
void Erase_Die();

void Write_Byte(uint32_t address, uint8_t data);
uint32_t GET_Sector_Address(uint16_t sector_num);
uint32_t GET_SubSector_Address_32KB(uint16_t sector_num);
uint32_t GET_SubSector_Address_4KB(uint16_t sector_num);

uint8_t Read_Byte(uint32_t address);
void Read_ID(volatile uint8_t *pRxBuff, DEV_ID_DATA *pDEV_STR);

int8_t Status_Reg_Read();

uint16_t Read_Register(uint8_t command);

#endif /* MT25TL_H_ */
