/**
*****************************************************************************
** Kommunikációs mérés - LocalSensorProcessing.c
** Az érzékelõk kezelése
*****************************************************************************
*/
#include <stdio.h>
#include "localsensorprocessing.h"
#include "bsp_thermometer.h"
#include "log.h"
#include "bsp_accelerometer.h"
#include "bsp_buttonsswitches.h"

void collectSensorData();
void SetLocalZoneStatus(unsigned int alarmType, unsigned char status);

/* ----------------- Publikus, globális változók ----------------- */

// Referencia (reset-kori) és legutolsó mért hőmérséklet
signed int referenceTemperature = 0;
signed int currentTemperature = 0;
// Legutolsó MEMS mérés eredménye (küszöbszint feletti dőles van?)
// A 3 tengely közül a legnagyobb dőlésszöge
signed int currentMaxTilt = 0;
// a TestSwitch utolsó állapota
uint8_t TestSwitchState = 0;

// Ennek a zónának az azonosítója, induláskor a kapcsolók állása állítja be
uint8_t localZoneID = 0;

// Minden zóna állapota
uint8_t zoneStatus[MAX_ZONE_NUM];
// Valtozott valamelyik érzékelő értéke?
//	Ha változott, az LCD-t frissíteni kell majd.
uint8_t hasSensorDataChanged = 0;
// Valtozott a zoneStatus tömb tartalma?
uint8_t hasAnyZoneStatusChanged = 0;
// Valtozott a saját zóna állapota?
uint8_t hasLocalZoneStatusChanged = 0;

/* ----------------- Belső, globális változók ----------------- */

// Elõzõ iteráció szenzor értékei
signed int prevTemp = 0;
signed int prevMaxTilt = 0;
uint8_t prevTestSwitchState = 0;

/* ----------------- Publikus függvények ----------------- */

/** Szenzorok inicializálása. */
void LocalSensorProcessing_Init()
{
	Thermometer_Init();
	// Accelerometer_Init();

	//char buffer[100];
	// Referencia hőmérséklet lekérdezése. A riasztás eldöntésekor ehhez viszonyítunk majd.
	// Get temperature data, set referenceTemperature
	//referenceTemperature = Thermometer_GetTemperature();
	//snprintf(buffer,100,"Referencia hőmérséklet: %d\n\r",referenceTemperature);
	//Log_LogString(buffer,LOGLEVEL_NORMAL);

	// Saját zóna azonosító lekérdezése
	//localZoneID = ButtonsSwitches_GetLocalZoneID();

}

/** Helyi érzékelõk kezelését összefogó függvény. A főciklus hívja. */
void LocalSensorProcessing_Go()
{
	// Szenzor adatok frissítése
	collectSensorData();

	// Változtak a szenzor értékek? Ha igen, majd az LCD-t frissíteni kell...
	if ((currentTemperature != prevTemp) || (currentMaxTilt != prevMaxTilt) || (TestSwitchState != prevTestSwitchState))
	{
		hasSensorDataChanged=1;
	}
	prevTemp = currentTemperature;
	prevMaxTilt = currentMaxTilt;
	prevTestSwitchState = TestSwitchState;

	// Eddigi zóna státusz mentése
	uint8_t prevZoneStatus = zoneStatus[localZoneID];

	// Ha valami küszöbszint felett van, riasztás beállítása és küldése
	SetLocalZoneStatus(ALARM_FIRE, currentTemperature >= referenceTemperature+2);
	SetLocalZoneStatus(ALARM_STEALTH, currentMaxTilt > 5);
	SetLocalZoneStatus(ALARM_TEST, TestSwitchState);

	// Ha változott a zone státusz, az új értéket el kell küldeni a CAN buszon
	if (prevZoneStatus != zoneStatus[localZoneID])
	{
		hasLocalZoneStatusChanged=1;
		hasAnyZoneStatusChanged=1;
		// logolas
		char buffer[100];
		sprintf(buffer,"Helyi zóna (%u) állapota: %u\n\r",localZoneID, zoneStatus[localZoneID]);
		Log_LogString(buffer,LOGLEVEL_NORMAL);
	}
}

/* ----------------- Belső függvények ----------------- */

/** Szenzor adatok összegyűjtése és mentése globális változókba. */
void collectSensorData()
{
	currentTemperature = Thermometer_GetTemperature();

	char buffer[100];
	snprintf(buffer,100,"Aktuális hőmérséklet: %u\n\r",currentTemperature);
	Log_LogString(buffer,LOGLEVEL_DETAILED);

	// MEMS adatok lekérdezése, CurrentMaxTilt frissítése
	int8_t x,y,z;
	Accelerometer_GetTilt(&x, &y, &z);
	x = x>=0 ? x : -x;
	y = y>=0 ? y : -y;
	currentMaxTilt = x>y ? x : y;

	sprintf(buffer,"Aktuális dőlés: %u\n\r",currentMaxTilt);
	Log_LogString(buffer,LOGLEVEL_DETAILED);

	TestSwitchState = ButtonsSwitches_GetTestSwitchState();
	sprintf(buffer,"Aktuális teszt kapcsoló állás: %u\n\r", TestSwitchState);
	Log_LogString(buffer,LOGLEVEL_DETAILED);
}

/** Helyi riasztási állapotok beállítása. */
void SetLocalZoneStatus(unsigned int alarmType, unsigned char status)
{
	if (status)
	{
		zoneStatus[localZoneID] |= alarmType;
	}
	else
	{
		zoneStatus[localZoneID] &= ~alarmType;
	}
}

