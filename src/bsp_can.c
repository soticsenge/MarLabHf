/** 
 * A CAN kommunikációs feladat megoldását ebben a fájlban készítse el.
 *
 * A CAN periféria inicializálása a korábbiakhoz hasonlóan történik: alacsony szintű pin beállításokból
 * és magának a perifériának a beállításaiból áll. Ez utóbbi kiegészül egy filter beállítással, mellyel
 * meghatározzuk, hogy milyen üzeneteket szeretnénk fogadni (mivel a CAN busz nem eszközöket címez, hanem
 * mindenki eldöntheti, hogy miket akar fogadni).
 *
 * A küldéshez és fogadáshoz CanTxMsgTypeDef és CanRxMsgTypeDef típusú változókra lesz szükség (jelen esetben globális változók
 * txMessage és rxMessage néven), amiket az inicializálás során adunk meg a perifériának. Küldésnél csak ki kell töltenünk
 * az üzenetet és azt mondani, hogy mehet, fogadásnál pedig egy callback függvényben értesülünk arról, hogy új adatok jöttek.
 *
 * Mind a küldést, mind a fogadást megszakításos módszerrel valósítjuk meg, hogy ne blokkoljanak semmi más funkciót.
 * Ilyenkor figyelni kell, hogy a beérkező adatok fogadása után tájékoztassuk a HAL-t, hogy további adatokat is szeretnénk majd fogadni.
 * Ezen kívül küldésnél figyelnünk kell majd rá, hogy csak akkor küldjünk, ha nincsen folyamatban egy másik küldési folyamat.
 */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include <stdio.h>
#include "log.h"
#include "localsensorprocessing.h"
#include "bsp_globalfunctions.h"

/* ----------------- Belső, globális változók ------------------- */

// A handle, amin keresztül kapcsolatot tartunk a CAN1 perifériával.
CAN_HandleTypeDef hcan;

// Az aktuálisan küldött és fogadott üzeneteket itt tároljuk.
CanTxMsgTypeDef txMessage;
CanRxMsgTypeDef rxMessage;

/* ----------------- Publikus függvények ------------------------ */

/** CAN kommunikáció inicializálása. */
HAL_StatusTypeDef CAN_Init()
{
	// 
	// A CAN vezérlő inicializálásához először engedélyezze a CAN1 periféria órajelét a korábbi feladatokhoz hasonlóan,
	// majd állítsa be a hcan handler Instance, pTxMsg, pRxMsg és Init maradék mezőit: első bitszegmens 6 kvantum, második 8 kvantum.
	// Normál mód, Prescaler 2 (erre nincs konstans), a synchronisation jump width pedig 1 kvantum.
	// A HAL_CAN_Init() függvény segítségével inicalizálja a perifériát. Ellenőrizze a visszatérési értéket és ha sikertelen volt,
	//	jelezze a logban a Log_LogStringAndHalStatus() függvénnyel és a visszatérési értékben.
	//	A loglevel paraméternek adjon meg LOGLEVEL_NORMAL-t, hogy a hibaüzenet ne csak részletes
	//	logolás esetén jelenjen meg.
		__CAN1_CLK_ENABLE();

	hcan.Instance = CAN1;

	hcan.Init.BS1 = CAN_BS1_6TQ;
	hcan.Init.BS2 = CAN_BS2_8TQ;
	hcan.Init.Mode = CAN_MODE_NORMAL;
	hcan.Init.Prescaler = 2;
	hcan.Init.SJW = CAN_SJW_1TQ;


	hcan.Init.ABOM = DISABLE;
	hcan.Init.AWUM = DISABLE;
	hcan.Init.NART = DISABLE;
	hcan.Init.RFLM = DISABLE;
	hcan.Init.TTCM = DISABLE;
	hcan.Init.TXFP = DISABLE;

	
	hcan.pTxMsg = &txMessage;
	hcan.pRxMsg = &rxMessage;

	HAL_StatusTypeDef result = HAL_CAN_Init(&hcan);
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("CAN_Init: HAL_CAN_Init sikertelen.\n\r", LOGLEVEL_NORMAL, result);
		return result;
	}


	// Ezután be kell állítanunk a filtert, ami jelen esetben a FIFO0-ra engedélyezi
	// a bejövő üzenetek fogadását. Ezt egy CAN_FilterConfTypeDef típusú változó kitöltésével,
	// majd a HAL_CAN_ConfigFilter() meghívásával tehetjük meg. (Figyeljen a pointer paraméterekre!)
	// BankNumber 0 (nincs rá konstans), a filter legyen aktív (ENABLE), 0-ás FIFO (CAN_FILTER_FIFO0),
	// az ID-ra vonatkozóan szűrünk (CAN_FILTERMODE_IDMASK), CAN_FILTERSCALE_32BIT,
	// a FilterNumber, IdLow, IdHigh, MaskIdLow, MaskIdHigh pedig mind 0.
	// Ellenőrizze a beállítás sikerességét a visszatérési érték alapján. Ha sikertelen,
	// adja vissza a hibakódot és a logban jelezze a Log_LogStringAndHalStatus()
	// függvény segítségével.
	CAN_FilterConfTypeDef filter;
	filter.BankNumber = 0;
	filter.FilterActivation = ENABLE;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterIdHigh = 0x0000;
	filter.FilterIdLow = 0x0000;
	filter.FilterMaskIdHigh = 0x0000;
	filter.FilterMaskIdLow = 0x0000;
	filter.FilterMode = CAN_FILTERMODE_IDMASK;
	filter.FilterNumber = 0;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	result = HAL_CAN_ConfigFilter(&hcan, &filter);
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("CAN_Init: HAL_CAN_ConfigFilter sikertelen.\n\r", LOGLEVEL_NORMAL, result);
		return result;
	}


	// Végül engedélyezze a FIFO 0 pending message megszakítást (CAN_IT...), mely a beérkező üzenetet jelzi majd
	// a __HAL_CAN_ENABLE_IT makró segítségével. Ez a makró olyan, mint egy függvény:
	//	első paramétere a szokásos handle, a második az interruptot azonosítja.
	//	Ha nem egyértelmű, melyik interruptot kell használni, akkor érdemes kiválasztani egyet,
	//	majd annak definíciójára ugorva (F3) meg lehet nézni a leírásukat.
	// Ezután engedélyezze a CAN1_RX0_IRQn és CAN1_TX_IRQn megszakításokat (kezelőfüggvény ennek a fájlnak a végén)
	//	a HAL_NVIC_SetPriority() és HAL_NVIC_EnableIRQ() függvényekkel. A PreemptPriority legyen 0, a SubPriority
	// pedig 0x01 és 0x02.
	__HAL_CAN_ENABLE_IT(&hcan, CAN_IT_FMP0);

	HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0x00, 0x01);
	HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0x00, 0x02);
	HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);


	// Végül írja ki a logra, hogy sikerült a CAN_Init.
	Log_LogString("CAN_Init: sikeres\n\r",LOGLEVEL_NORMAL);

	return HAL_OK;
}

/** CAN üzenet küldése. */
HAL_StatusTypeDef CAN_SendMessage(uint8_t zoneID, uint8_t newStatus)
{
	// Először is ellenőrizze, hogy a CAN vezérlő készen áll-e a küldésre, vagyis
	// a hcan.State vagy HAL_CAN_STATE_READY, vagy csak adatfogadás van folyamatban.
	// Ha a CAN periféria nem áll készen, adjon vissza HAL_BUSY állapotot.

	if ((hcan.State != HAL_CAN_STATE_READY) && (hcan.State != HAL_CAN_STATE_BUSY_RX))
	{
		return HAL_BUSY;
	}

	// Ha készen áll a küldésre, akkor töltse ki a hcan.pTxMsg által mutatott, CanTxMsgTypeDef
	//	típusú struct-ot. (Ez egyébként pont a txMessage változónk, mivel mi állítottuk be a pointert
	//	az inicializálás során.) A standard cím legyen 0x321, Data[0] a zoneID, Data[1] a newStatus (összesen 2 adatbájt).
	//	Standard ID-kat küldünk (CAN_ID_STD) és adat típusú frame-et (CAN_RTR_DATA).
	//	Az ExtId-t állítsa be a biztonság kedvéért 0x01-re.
	// Figyeljen rá, hogy mindig, amikor egy struct típusú változónak ad értéket,
	//	ne hagyjon ki egyetlen mezőt sem (pl. most a DLC-t). A default érték sokszor nem jó!
	hcan.pTxMsg->DLC = 2;
	hcan.pTxMsg->Data[0] = zoneID;
	hcan.pTxMsg->Data[1] = newStatus;
	hcan.pTxMsg->ExtId = 0x01;
	hcan.pTxMsg->IDE = CAN_ID_STD;
	hcan.pTxMsg->RTR = CAN_RTR_DATA;
	hcan.pTxMsg->StdId = 0x321;

	// Küldje el az üzenetet a HAL_CAN_Transmit_IT() függvény meghívásával. Ennek azért van ilyen kevés paramétere,
	// mert a hcan már tartalmazza a pointert a küldendő üzenetre, így most már csak annyit kell mondani, hogy mehet.
	// (Blokkolás nélkül küldünk, ezért van az _IT végződés.)
	// A visszatérési értéket ellenőrizze és ha hiba volt, jelezze a logban és adja vissza a hibakódot.
	// Használja a Log_LogStringAndHalStatus() függvényt.

	HAL_StatusTypeDef result = HAL_CAN_Transmit_IT(&hcan);
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("CAN_SendMessage: sikertelen.\n\r", LOGLEVEL_NORMAL, result);
		return result;
	}

	// Siker esetén írja ki a logra, hogy sikeresen elment az üzenet.
	Log_LogString("CAN: Uj zóna státusz kiküldve.\n\r",LOGLEVEL_NORMAL);

	return HAL_OK;
}

/** A főciklus hívja periodikusan. Változás esetén az új státusz elküldése. */
void CAN_Go()
{
	if (hasLocalZoneStatusChanged)
	{
		HAL_StatusTypeDef result = CAN_SendMessage(localZoneID, zoneStatus[localZoneID]);
		if (result != HAL_OK)
		{
			Log_LogStringAndHalStatus("CAN_SendMessage sikertelen.\n\r", LOGLEVEL_NORMAL, result);
		}
	}
}

/* ----------------- Belső függvények ----------------- */

/** Alacsony szintű inicializálás, a HAL_CAN_Init() hívja. */
void HAL_CAN_MspInit(CAN_HandleTypeDef * hcan)
{
	UNUSED(hcan);

	// Először engedélyezze a D port órajelét, majd a PD0 és PD1 pineket
	// állítsa Alternate function push-pullra, NOPULL, FAST beállítással,
	// az Alternate function pedig GPIO_AF9_CAN1.
	// Ne felejtse el meghívni a HAL_GPIO_Init() függvényt!
	__GPIOD_CLK_ENABLE();

	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate = GPIO_AF9_CAN1;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
}

/* ----------------- Megszakításkezelő és callback függvények ----------------- */

/** Beérkezett CAN üzenet feldolgozása. */
void ProcessReceivedCanMessage(uint8_t zoneID, uint8_t newStatus)
{
	zoneStatus[zoneID] = newStatus;
	hasAnyZoneStatusChanged = 1;

	// Ha a saját zónára vonatkozó üzenet jött, akkor jelezzük, hogy az is megváltozott,
	//	bár elvileg a LocalSensorProcessing már beállította.
	if (localZoneID==zoneID)
	{
		hasLocalZoneStatusChanged = 1;
	}
	// Mivel ezt a függvényt megszakításkezelőből hívjuk, itt nem logolunk UART-ra.
}

/** A CAN megszakításkezelő hívja sikeresen fogadott üzenet esetén. */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef *handle)
{
	// Először ellenőrizze, hogy a kapott üzenet bufferére mutató pointer (handle->pRxMsg) biztosan nem NULL.
	// Ha nem az, akkor ellenőrizze, hogy az üzenet ID-ja (standard ID) 0x321,
	// az ID tényleg standard ID-t (IDE mező értéke CAN_ID_STD), valamint hogy a kapott adatbájtok száma (frame hossza) tényleg 2-e.
	// Ha ezek mind teljesülnek, akkor hívja meg a ProcessReceivedCanMessage() függvényt, mely gondoskodik az üzenet feldolgozásáról.
	// A két paramétere a kapott üzenet 0. (zoneID) és 1. bájtja (new status).
	if (handle->pRxMsg)
	{
		if ((handle->pRxMsg->StdId == 0x321) && (handle->pRxMsg->IDE == CAN_ID_STD) && (handle->pRxMsg->DLC == 2))
		{
			ProcessReceivedCanMessage( handle->pRxMsg->Data[0], handle->pRxMsg->Data[1] );
		}
	}

	// Ahhoz, hogy ez után is tudjunk adatokat fogadni, indítson egy újabb interruptos CAN adatfogadást
	// a CAN_FIFO0 FIFO-n kereszül a HAL_CAN_Receive_IT() meghívásával.
	// Ha ezt nem tennénk, a továbbiakban nem értesülnénk a fogadott adatokról.
	HAL_CAN_Receive_IT(handle, CAN_FIFO0);
}

/** A CAN hibajelző callback függvénye. */
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *handle)
{
	char buffer[255];
	snprintf(buffer, 255, "HAL_CAN_ErrorCallback: CAN hiba: %u\n\r", (unsigned int)handle->ErrorCode);
	Log_LogString(buffer, LOGLEVEL_NORMAL);
}

/** CAN adatfogadási megszakítás kezelője. */
void CAN1_RX0_IRQHandler(void)
{
	// A megszakítás kezelőben hívja meg a HAL beépített kezelőjét, mely a HAL_CAN_IRQHandler()
	// mind küldési, mind fogadási megszakítás esetén.
	HAL_CAN_IRQHandler(&hcan);
}

/** CAN adatküldési megszakítás kezelője. */
void CAN1_TX_IRQHandler(void)
{
	// A megszakítás kezelőben hívja meg a HAL beépített kezelőjét, mely a HAL_CAN_IRQHandler()
	// mind küldési, mind fogadási megszakítás esetén.
	HAL_CAN_IRQHandler(&hcan);
}
