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


int stepCount=0;
int stepTimeStamps[9999];
char buffer[8];


void  addStep()
{
	stepTimeStamps[stepCount] = HAL_GetTick();
	stepCount++;
}

/** Inicializálás. Minden alrendszert inicializál. */
void MainLoop_Init()
{
	GlobalFunctions_Init();
	Log_Init();
	LocalSensorProcessing_Init();
	ButtonsSwitches_Init();
//	Graphics_Init();
//	CAN_Init();


	// Elso alkalommal jelenjen meg minden...
	hasAnyZoneStatusChanged = 1;
	hasLocalZoneStatusChanged = 1;
	hasSensorDataChanged = 1;
}





/** A főciklus egy iterációját összefogó függvény. */
void MainLoop_Go()
{

	int tmp = 0;

	//Log_LogString("MAINLOOP: Főciklus - új iteráció\n\r",LOGLEVEL_DETAILED);

	tmp = (ButtonsSwitches_GetTestButtonState());


	if(!tmp) {
		addStep();
	//	Log_LogString("AAA" ,0);
	}
	displayNumberOfSteps(stepCount);
//	// Érzékelõk kezelése
//	LocalSensorProcessing_Go();
//	// CAN kommunikáció kezelése
//	CAN_Go();
//	// Megjelenítés a grafikus LCD-n (változás esetén)
//	Graphics_Go();



	if(lastReceivedUartCommand=='L'){
		int i=0;

		itoa(HAL_GetTick(), buffer, 10);
		Log_LogString(buffer,0);
		Log_LogString("\n",0);

		while (i<stepCount){
			itoa(stepTimeStamps[i], buffer, 10);
			Log_LogString(buffer,0);
			Log_LogString("\n",0);

			i++;
		}
		Log_LogString("EOM\n",0);
			lastReceivedUartCommand=0;
	}


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
}
