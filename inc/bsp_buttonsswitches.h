/**
*****************************************************************************
** Kommunik�ci�s m�r�s - buttonsswitches.h
** A nyom�gombok �s kapcsol�k kezel�se
*****************************************************************************
*/
#pragma once
#ifndef _BUTTONSSWITCHES_H__
#define _BUTTONSSWITCHES_H__
#include "stm32f4xx.h"

void ButtonsSwitches_Init();

int ButtonsSwitches_GetTestSwitchState();

int ButtonsSwitches_GetTestButtonState();

int ButtonsSwitches_GetGraphicsModeSwitchState();

uint8_t ButtonsSwitches_GetLocalZoneID();

#endif
