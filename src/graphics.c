/**
*****************************************************************************
** Kommunikációs mérés - graphics.h
** A grafikai megjelenítés
*****************************************************************************
*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include <stdio.h>
#include "bsp_buttonsswitches.h"
#include "Log.h"
#include "bsp_glcd.h"
#include "zonemap.h"
#include "localsensorprocessing.h"

void ShowDrawingGraphics();
void ShowDetailedGraphics();

/* ----------------- Belső, globális változók ----------------- */

// Szöveges mûveletekhez....
char buffer[] = "                        ";

// Változott a grafikus üzemmód (részletes vagy alaprajzos) az elõzõ
//	iteráció óta?
unsigned char hasGraphicsModeChanged = 1;

// Elozo iterációban a grafikus üzemmód. Figyelni kell, hogy változik-e, mert ha
//	igen, akkor állapotváltozás nélkül is újra kell rajzolni.
uint8_t prevGraphicsMode = 0xFF;	// Első alkalommal biztosan más lesz...

/* ----------------- Publikus függvények ----------------- */

/** Grafikus megjelenítés inicializálása. */
void Graphics_Init()
{
	GLCD_Init();
}

/** A főciklus grafikai teendőit összefoglaló függvény. */
void Graphics_Go()
{
	// Aktuális megjelenítési mód lekérdezése (részletek vagy alaprajzos)
	uint8_t graphicsMode = ButtonsSwitches_GetGraphicsModeSwitchState();

	if (graphicsMode != prevGraphicsMode)
	{
		// Mindenkeppen rajzolja újra...
		hasGraphicsModeChanged = 1;
		prevGraphicsMode = graphicsMode;
		// Log
		Log_LogString("GRH: Változott az üzemmód.\n\r",LOGLEVEL_NORMAL);
	}

	if (graphicsMode)
	{
		ShowDetailedGraphics();
	}
	else
	{
		ShowDrawingGraphics();
	}

	hasGraphicsModeChanged = 0;
}

/* ----------------- Belso függvények ----------------- */

/** Háttérkép (alaprajz) megjelenítése. */
void Send_BackgroundImage()
{
	int i,j,k=0;
	for (i=0;i<8;i++){
		for(j=0;j<128;j++){
			GLCD_Write_Block(zoneMap[k],i,j);
			k++;
		}
	}
}

/** Részletek képernyõ megjelenítése. */
void ShowDetailedGraphics()
{
	// Csak ha indokolt a frissítés...
	if (hasAnyZoneStatusChanged || hasSensorDataChanged || hasGraphicsModeChanged)
	{
		if (hasGraphicsModeChanged)
		{
			GLCD_Clear();
			GLCD_WriteString("Komm. Meres - BME AUT",0,0);

			sprintf(buffer,"LocalZoneID:%u",localZoneID);
			GLCD_WriteString(buffer,3,2);

			sprintf(buffer,"Temp:%d",currentTemperature);
			GLCD_WriteString(buffer,3,3);

			sprintf(buffer,"Tilt:%d",currentMaxTilt);
			GLCD_WriteString(buffer,3,4);

			sprintf(buffer,"Zones:%u %u %u %u",
					zoneStatus[0],zoneStatus[1],zoneStatus[2],zoneStatus[3]);
			GLCD_WriteString(buffer,3,5);

			// Log
			Log_LogString("GRH: Újrarajzolva (részletes kép)\n\r",LOGLEVEL_DETAILED);
		}
		else
		{
			sprintf(buffer,"%u   ",localZoneID);
			GLCD_WriteString(buffer,3+12*6,2);

			sprintf(buffer,"%d   ",currentTemperature);
			GLCD_WriteString(buffer,3+5*6,3);

			sprintf(buffer,"%d   ",currentMaxTilt);
			GLCD_WriteString(buffer,3+5*6,4);

			sprintf(buffer,"%u %u %u %u     ",
					zoneStatus[0],zoneStatus[1],zoneStatus[2],zoneStatus[3]);
			GLCD_WriteString(buffer,3+6*6,5);
		}
	}
}

/** Alaprajzos képernyõ megjelenítése, ha szükséges. */
void ShowDrawingGraphics()
{
	unsigned char zoneBaseCol[] = {12, 39, 70, 85};
	unsigned char zoneBaseRow[] = {4, 2, 2, 5};
	int zoneID = 0;
	char zoneString[4];	// 3 karakter + lezáró 0-nak hely
	if (hasGraphicsModeChanged)
	{
		GLCD_Clear();
		Send_BackgroundImage();
		Log_LogString("GRH: Zónatérkép újrarajzolva\n\r",LOGLEVEL_DETAILED);
	}
	if (hasAnyZoneStatusChanged || hasGraphicsModeChanged)
	{
		for(zoneID=0; zoneID<4; zoneID++)
		{
			zoneString[0] = (zoneStatus[zoneID] & ALARM_FIRE) ? 98+32 : ' ';	// 98: tűz karakter
			zoneString[1] = (zoneStatus[zoneID] & ALARM_STEALTH) ? 99+32 : ' ';	// 99: ember karakter
			zoneString[2] = (zoneStatus[zoneID] & ALARM_TEST) ? 'T' : ' ';	// 99: T karakter
			zoneString[3] = 0;
			GLCD_WriteString(zoneString,zoneBaseCol[zoneID],zoneBaseRow[zoneID]);
		}
		Log_LogString("GRH: Újrarajzolva (rajz)\n\r",LOGLEVEL_DETAILED);
	}
}
