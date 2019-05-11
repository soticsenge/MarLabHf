/**
*****************************************************************************
** Kommunikációs mérés - spi.h
** SPI kommunikáció
*****************************************************************************
*/
#pragma once
#ifndef _SPI_H__
#define _SPI_H__
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef SPI_Init();

void SPI_EnableChip(int enable);

HAL_StatusTypeDef SPI_Send(uint8_t* pData, uint16_t dataSize);

HAL_StatusTypeDef SPI_Receive(uint8_t* pData, uint16_t dataSize);

HAL_StatusTypeDef SPI_SendReceive(uint8_t* pDataIn, uint8_t *pDataOut, uint16_t dataSize);

#endif
