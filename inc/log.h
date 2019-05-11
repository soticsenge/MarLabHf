/**
*****************************************************************************
** Kommunikációs mérés - log.h
** Az UART-ra loggolás header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _LOG_H__
#define _LOG_H__
#include "stm32f4xx.h"

#define LOGLEVEL_NORMAL		2
#define LOGLEVEL_DETAILED	1

extern unsigned int currentLogLevel;

void Log_Init();
void Log_LogString(char *text, unsigned int logLevel);
void Log_LogStringAndHalStatus(char *text, unsigned int logLevel, HAL_StatusTypeDef status);

#endif
