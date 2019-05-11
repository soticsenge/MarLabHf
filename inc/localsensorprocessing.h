/**
*****************************************************************************
** Kommunikációs mérés - localsensorprocessing.h
** Az érzékelõk kezelésének header file-ja
*****************************************************************************
*/
#pragma once
#ifndef _LOCALSENSORPROCESSING_H__
#define _LOCALSENSORPROCESSING_H__
#include "stm32f4xx.h"

// Legutolso mert homerseklet
extern signed int currentTemperature;
// Legutolso MEMS meres eredmenye (kuszobszint feletti doles van?)
// A 3 tengely kozul a legnagyobb ertek
extern signed int currentMaxTilt;

// Ennek a zonanak az azonositoja, indulaskor a kapcsolok allasa allitja be
extern uint8_t localZoneID;

#define MAX_ZONE_NUM 4
extern uint8_t zoneStatus[MAX_ZONE_NUM];	// Minden zona allapota
extern uint8_t hasSensorDataChanged;		// Valtozott valamelyik erzekelo erteke?
											//	Ha valtozott, az LCD-t frissiteni kell majd.
extern uint8_t hasAnyZoneStatusChanged;		// Valtozott a fenti tomb tartalma?
extern uint8_t hasLocalZoneStatusChanged;	// Valtozott a sajat zona allapota?

// Alarm status bitek
#define ALARM_FIRE 1
#define ALARM_STEALTH 2
#define ALARM_TEST 4

void LocalSensorProcessing_Init();
void LocalSensorProcessing_Go();

#endif
