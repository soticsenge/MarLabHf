/**
*****************************************************************************
** Kommunik�ci�s m�r�s - mainloop.h
** UART kommunik�ci�
*****************************************************************************
*/
#pragma once
#ifndef _UART_H__
#define _UART_H__

/** A legutolj�ra fogadott byte. */
extern uint8_t lastReceivedUartCommand;

void UART_Init();

void UART_SendString(const char* str);

#endif
