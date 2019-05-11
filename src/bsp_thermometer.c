/**
*****************************************************************************
** Kommunikációs mérés - thermometer.c
** Az I2C-s hõmérõ függvényei. A kommunikációhoz az i2c.h
** 	szolgáltatásait használja.
*****************************************************************************
*/
#include "bsp_i2c.h"
#include "log.h"

/* ----------------- Belső, globális változók ----------------- */

const uint8_t TempAddr = (uint8_t)0x30;
const uint8_t TempConfReg = (uint8_t)0x08;
const uint8_t TempReg = (uint8_t)0x05;

/* ----------------- Publikus függvények ----------------- */

/** Inicializálás. */
HAL_StatusTypeDef Thermometer_Init()
{
	HAL_StatusTypeDef result = I2C_Init(0xA0);
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("Thermometer_Init(): I2C_Init() sikertelen.", LOGLEVEL_NORMAL, result);
		return result;
	}

	// Init: 8-as regiszterbe 00-t írunk.
	uint8_t data = 0;
	result = I2C_WriteRegister(TempAddr, TempConfReg, &data, 1);
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("Thermometer_Init(): hőmérő inicializálás sikertelen.", LOGLEVEL_NORMAL, result);
		return result;
	}
	return HAL_OK;
}

/** I2C-s hõmérõ értékének lekérdezése */
uint8_t Thermometer_GetTemperature()
{
	uint8_t temperature = 0x00;

	uint8_t data[2];
	if (I2C_ReadRegister(TempAddr, TempReg, data, 2) != HAL_OK)
	{
		return 0xFF;
	}

	uint8_t UpperByte = data[0];
	uint8_t LowerByte = data[1];


	// A továbbiakban össze kell rakni a hõrmérséklet értéket a hõmérõ
	//	ábrázolásának megfelelõen.
	UpperByte = UpperByte & 0x1F;
	if ((UpperByte & 0x10) == 0x10){	// < 0°C
		UpperByte = UpperByte & 0x0F;
		temperature = 256 - (UpperByte*16+LowerByte/16);
	}else{	// > 0°C
		temperature = UpperByte*16+LowerByte/16;
	}
	return temperature;
}
