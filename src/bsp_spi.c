 /** TODO 1. SPI Feladat
 * Az SPI kommunikációs feladatot ebben és a bsp_accelerometer.c fájlban oldja meg.
 * Ebben a fájlban a feladat az SPI kommunikáció inicializálása
 *  (SPI_Init() és HAL_SPI_MspInit() függvények), valamint az alapvető
 * kommunikációs függvények elkészítése (SPI_EnableChip(), SPI_Send(),
 * SPI_Receive() és SPI_SendReceive() függvények).
 *
 * Ha ezek elkészültek, az bsp_accelerometer.c ezek segítségével fog kommunikálni
 * a LIS302DL MEMS szenzorral.
 */
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "bsp_globalfunctions.h"

/* ----------------- Belső, globális változók ------------------- */

SPI_HandleTypeDef hspi;
const uint32_t spi_timeout = 5000;

/* ----------------- Publikus függvények ------------------------ */

HAL_StatusTypeDef SPI_Init()
{
	// TODO 1. SPI Feladat
	// A korábbiakhoz hasonlóan inicializálja az SPI1 perifériát
	// a hspi változó Instance és Init mezőinek kitöltésével, majd
	// a HAL_SPI_Init() függvény meghívásával.
	// BaudRate prescaler 16, órajel fázisa 1 EDGE, az órajel polaritás alacsony,
	//	CRC számítás kikapcsolva, CRC polinom 7 (csak erre nincsen előre definiált konstans és
	// az értéke fontos annak ellenére, hogy a CRC számítás ki van kapcsolva),
	// 8 bites adatokat küldünk, MSB first, a mikrovezérlő a master, a slave select (NSS) szoftveres,
	// SPI_TIMODE_DISABLED. A direction pedig 2LINES (ez a szokásos, 2 irányú SPI).
	// Ne felejtse el meghívni a HAL_SPI_Init() függvényt.


	return HAL_OK;
}

/** Az IC engedélyezése vagy tiltása. */
void SPI_EnableChip(int enable)
{
	// TODO 1. SPI Feladat
	// A PE3 (CS jel) beállításához használja a HAL_GPIO_WritePin() függvényt.
	// Figyeljen rá, hogy "igaz" enable érték esetén 0-ra kell állítani az active low CS jelet.

}

/** SPI adatküldés. */
HAL_StatusTypeDef SPI_Send(uint8_t* pData, uint16_t dataSize)
{
	// TODO 1. SPI Feladat
	// A művelethez használja a HAL_SPI_Transmit() függvényt, a kapott paramétereket
	// és az spi_timeout konstans változót.

	return HAL_OK;
}

/** SPI adat fogadás. */
HAL_StatusTypeDef SPI_Receive(uint8_t* pData, uint16_t dataSize)
{
	// TODO 1. SPI Feladat
	// A művelethez használja a HAL_SPI_Receive() függvényt, a kapott paramétereket
	// és az spi_timeout konstans változót.
	// Törölje a keretprogram csupa nullát visszaadó részét.

	for(int i=0; i<dataSize; i++)
	{
		pData[i]=0;
	}
	return HAL_OK;
}

/** SPI küldés és fogadás egyszerre. */
HAL_StatusTypeDef SPI_SendReceive(uint8_t* pDataIn, uint8_t *pDataOut, uint16_t dataSize)
{
	// TODO 1. SPI Feladat
	// A művelethez használja a HAL_SPI_TransmitReceive() függvényt, a kapott paramétereket
	// és az spi_timeout konstans változót.
	// Törölje a keretprogram csupa nullát visszaadó részét.
	
	for(int i=0; i<dataSize; i++)
	{
		pDataIn[i]=0;
	}
	return HAL_OK;
}

/* ----------------- Belső függvények --------------------------- */

/** Alacsony szintű inicializálás, az SPI_Init()-ben hívott HAL_SPI_Init() hívja meg. */
void HAL_SPI_MspInit(SPI_HandleTypeDef *handle)
{
	UNUSED(handle);

	// TODO 1. SPI Feladat
	// Az alacsony szintű inicializáláshoz engedélyezze az A és E GPIO-k, valamint az SPI1
	// periféria órajelét.


	// Az SPI SCK (PA5), MISO (PA6) és MOSI (PA7) pinjeit
	// a korábbiakhoz hasonlóan állítsa Alternate mode push-pull, nopull, fast módra,
	// az "Alternate" function pedig GPIO_AF5_SPI1.
	// Ezen kívül a CS jelhez a PE3 pint állítsa push-pull kimenetre, nopull, fast üzemmódban.
	// (Ne feledkezzen meg a bsp_accelerometer.c fájlban lévő feladatokról sem!)

}
