/**
*****************************************************************************
** Kommunikációs mérés - mainloop.c
** A fõciklus
*****************************************************************************
* TODO 5.2. kiegészítő feladat: USB-VCP adatfogadás kezelése
* A kiegészítő feladatban a beérkező parancsokra reagálást itt, a MainLoop_Go()
* függvényben valósítsa meg. (A feladat másik része a generálandó usbd_cdc_if.c-ben
* lesz megtalálható, mely a parancsok fogadásáért felelős.)
*/

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include <stdio.h>
#include "bsp_globalfunctions.h"
#include "log.h"
#include "localsensorprocessing.h"
#include "graphics.h"
#include "bsp_buttonsswitches.h"
#include "bsp_uart.h"
#include "bsp_can.h"
#include "segment_display.h"


/** Inicializálás. Minden alrendszert inicializál. */
void MainLoop_Init()
{
	GlobalFunctions_Init();
	Log_Init();
	LocalSensorProcessing_Init();
//	ButtonsSwitches_Init();
	Init7SegDisplays();
//	Graphics_Init();
//	CAN_Init();

	Log_LogString("***********************\n\r",LOGLEVEL_NORMAL);
	Log_LogString("* Kommunikációs mérés *\n\r",LOGLEVEL_NORMAL);
	Log_LogString("*       BME AUT       *\n\r",LOGLEVEL_NORMAL);
	Log_LogString("***********************\n\r\n\r",LOGLEVEL_NORMAL);

	localZoneID = ButtonsSwitches_GetLocalZoneID();

	char buffer[100];
	snprintf(buffer,100,"Saját zóna azonosítója: %u\n\r",localZoneID);
	Log_LogString(buffer,LOGLEVEL_NORMAL);

	// Elso alkalommal jelenjen meg minden...
	hasAnyZoneStatusChanged = 1;
	hasLocalZoneStatusChanged = 1;
	hasSensorDataChanged = 1;
}

/** A főciklus egy iterációját összefogó függvény. */
void MainLoop_Go()
{

	int tmp = 0;

	Log_LogString("MAINLOOP: Főciklus - új iteráció\n\r",LOGLEVEL_DETAILED);

	tmp = (ButtonsSwitches_GetTestButtonState());

//	// Érzékelõk kezelése
//	LocalSensorProcessing_Go();
//	// CAN kommunikáció kezelése
//	CAN_Go();
//	// Megjelenítés a grafikus LCD-n (változás esetén)
//	Graphics_Go();





	// UART adatfogadás kezelése
	// Amennyiben az uart.c-ben definiált lastReceivedUartCommand változó
	// értéke nem 0, vagyis kaptunk parancsot az UART-on, akkor azt itt hajtsa végre
	// egy switch szerkezettel, majd lastReceivedUartCommand-ot állítsa vissza 0-ra.
	// Az egyes értelmezett parancsok:
	// - 'g': Greeting. A Log_LogString függvény segítségével küldjön üdvözlő szöveget.
	// - 'n': Normal log level: a currentLogLevel globális valtozót állítsa LOGLEVEL_NORMAL-ra
	// - 'd': Detailed log level: A log levelt állítsa DETAILED-re
	// - 'q': Query log level: Küldje el a logra hogy most éppen milyen log level van kiválasztva.
	// - Ismeretlen parancs karakter esetén küldjön hibaüzenetet.

//	IF (LASTRECEIVEDUARTCOMMAND!=0)
//	{
//		SWITCH (LASTRECEIVEDUARTCOMMAND)
//		{
//		CASE 'G':	// GREETING
//			LOG_LOGSTRING("HELLO!\N\R",LOGLEVEL_NORMAL);
//			BREAK;
//		CASE 'N':	// NORMAL LOG LEVEL
//			CURRENTLOGLEVEL = LOGLEVEL_NORMAL;
//			LOG_LOGSTRING("LOG LEVEL: NORMAL\N\R",LOGLEVEL_NORMAL);
//			BREAK;
//		CASE 'D':	// DETAILED LOG LEVEL
//			CURRENTLOGLEVEL = LOGLEVEL_DETAILED;
//			LOG_LOGSTRING("LOG LEVEL: DETAILED\N\R",LOGLEVEL_NORMAL);
//			BREAK;
//		CASE 'Q':	// QUERY LOG LEVEL
//			IF (CURRENTLOGLEVEL == LOGLEVEL_DETAILED)
//			{
//				LOG_LOGSTRING("LOG LEVEL: DETAILED\N\R",LOGLEVEL_NORMAL);
//			}
//			ELSE
//			{
//				LOG_LOGSTRING("LOG LEVEL: NORMAL\N\R",LOGLEVEL_NORMAL);
//			}
//			BREAK;
//		DEFAULT:
//			LOG_LOGSTRING("ISMERETLEN PARANCS!\N\R",LOGLEVEL_NORMAL);
//			BREAK;
//		}
//		LASTRECEIVEDUARTCOMMAND=0;
	}

	
	// TODO 5.2. kiegészítő feladat: USB-VCP adatfogadás kezelése
	// A fenti UART-os példával megegyező módon valósítsa meg az adatfogadást az USB-VCP-ről is.
	// Használja az előző feladatban megvalósított CDC_READ_FS() függvényt (usbd_cdc_if.c)
	
	
	
	// TODO 6. kiegészítő feladat: USB-VCP csatlakozási események naplózása
	// Naplózza az USB eszköz csatlakozását és leválasztását. Egészítse ki és használja
	// az usbd_cdc_if.c-ben található CDC_Init_FS() és CDC_DeInit_FS() függvényeket.
	
	
	
	
	// A változást jelzõ flag-ek törlése
//	hasAnyZoneStatusChanged = 0;
//	hasLocalZoneStatusChanged = 0;
//	hasSensorDataChanged = 0;
//}
