/**
*****************************************************************************
** Kommunikációs mérés - mainloop.h
** UART kommunikáció
*****************************************************************************
*/
#pragma once
#ifndef _UART_H__
#define _UART_H__

/** A legutoljára fogadott byte. */
extern uint8_t lastReceivedUartCommand;

void UART_Init();

void UART_SendString(const char* str);

#endif
