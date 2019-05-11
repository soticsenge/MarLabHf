/**
*****************************************************************************
** Kommunikációs mérés - can.h
** A CAN kommunikáció header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _CAN_H__
#define _CAN_H__

HAL_StatusTypeDef CAN_Init();
HAL_StatusTypeDef CAN_SendMessage(uint8_t zoneID, uint8_t newStatus);

void CAN_Go();

#endif
