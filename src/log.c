/**
*****************************************************************************
** Kommunikációs mérés - log.c
** Az UART-ra logolás
*****************************************************************************
*/
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include "log.h"
#include "bsp_uart.h"

/* ----------------- Belső, globális változók ----------------- */

unsigned int currentLogLevel = LOGLEVEL_NORMAL;

/* ----------------- Publikus függvények ----------------- */

/** Inicializálás. */
void Log_Init()
{
	UART_Init();
}

/** String kiírása a log-ba. */
void Log_LogString(char *text, unsigned int logLevel)
{
	// Ha eleg magas a logLevel, akkor kiirjuk USART-on a terminalra
	//if (logLevel>=currentLogLevel)
	//{
		// TODO 4.2. Kiegészítő feladat: USB-VCP naplózás
		// Jelenítse meg a log üzeneteket az USB kapcsolaton keresztül is.
		UART_SendString(text);
	//}
}

/** String és HAL status kiírása a log-ba. A logLevel a LOGLEVEL_ kezdetű konstansok egyike. */
void Log_LogStringAndHalStatus(char *text, unsigned int logLevel, HAL_StatusTypeDef status)
{
	char *statusString =
			(status==HAL_OK) ? "OK" :
			(status==HAL_ERROR ? "ERROR" :
			(status==HAL_BUSY ? "BUSY" :
			(status==HAL_TIMEOUT ? "TIMEOUT" : "UNKNOWN")));

}
