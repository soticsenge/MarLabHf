/**
*****************************************************************************
** Kommunik�ci�s m�r�s - thermometer.h
** Az I2C-s h�m�r� header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _THERMOMETER_H__
#define _THERMOMETER_H__

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef Thermometer_Init();
uint8_t Thermometer_GetTemperature();

#endif
