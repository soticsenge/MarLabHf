/**
*****************************************************************************
** Kommunikációs mérés - bsp_accelerometer.c
** Itt találhatóak a LIS302DL szenzor függvényei.
** Inicializálás után a Accelerometer_GetTilt() függvény adja
** vissza a mérési eredményeket.
** A kommunikációhoz az spi.h szolgáltatásait használjuk.
*****************************************************************************
* TODO 1. SPI Feladat
* Az SPI feladat részeként ebben a fájlban kell elkészíteni az SPI alacsony szintű
* függvényeit felhasználó LIS302DL_Write() és LIS302DL_Read() függvényeket.
*
* Az itt található, magasabb szintű függvények (Accelerometer_Init() és Accelerometer_GetTilt()
* ezeket a függvényeket használják a szenzor inicializálására és használatára.
*/
#include "bsp_spi.h"
#include "log.h"
#include "bsp_lis302dl_defines.h"

void LIS302DL_Init(LIS302DL_InitTypeDef *LIS302DL_InitStruct);
void LIS302DL_FilterConfig(LIS302DL_FilterConfigTypeDef *LIS302DL_FilterConfigStruct);
void LIS302DL_Write(uint8_t* pData, uint8_t WriteAddr, uint16_t dataSize);
void LIS302DL_Read(uint8_t* pData, uint8_t ReadAddr, uint16_t dataSize);

/* ----------------- Globális, belső változók ------------------- */
// Jeleztük már a logban, hogy a MEMS nem válaszol? (Hiba esetén.)
//	Azért, hogy ne minden interációban írjunk a logba.
int memsErrorReported = 0;

/* ----------------- Publikus függvények ------------------------ */

/** A MEMS szenzor inicializálása. Magába foglalja az SPI kapcsolat inicializálását is. */
void Accelerometer_Init()
{
	HAL_StatusTypeDef result = SPI_Init();
	if (result != HAL_OK)
	{
		Log_LogStringAndHalStatus("Accelerometer_Init: SPI_Init() sikertelen.\n\r", LOGLEVEL_NORMAL, result);
		return;
	}

	LIS302DL_InitTypeDef LIS302DL_InitStructure;
	LIS302DL_InitStructure.Power_Mode = LIS302DL_LOWPOWERMODE_ACTIVE;
	LIS302DL_InitStructure.Output_DataRate = LIS302DL_DATARATE_100;
	LIS302DL_InitStructure.Axes_Enable = LIS302DL_XYZ_ENABLE;
	LIS302DL_InitStructure.Full_Scale = LIS302DL_FULLSCALE_2_3;
	LIS302DL_InitStructure.Self_Test = LIS302DL_SELFTEST_NORMAL;
	LIS302DL_Init(&LIS302DL_InitStructure);
	// Kell egy kis idő a szenzornak.
	HAL_Delay(30);
	// MEMS High Pass Filter beállítása
	LIS302DL_FilterConfigTypeDef LIS302DL_FilterStruct;
	LIS302DL_FilterStruct.HighPassFilter_Data_Selection = LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER;
	LIS302DL_FilterStruct.HighPassFilter_CutOff_Frequency = LIS302DL_HIGHPASSFILTER_LEVEL_1;
	LIS302DL_FilterStruct.HighPassFilter_Interrupt = LIS302DL_HIGHPASSFILTERINTERRUPT_1_2;
	LIS302DL_FilterConfig(&LIS302DL_FilterStruct);

	memsErrorReported = 0;
}

/** Visszaadja az aktális gyorsulás vektort. Siker esetén a visszatérési érték 0. */
int Accelerometer_GetTilt(int8_t *Xg, int8_t *Yg, int8_t *Zg)
{
	unsigned char uBuffer[6];
	uint8_t temp = 0x00;

	LIS302DL_Read(&temp, LIS302DL_WHO_AM_I_ADDR, 1);
	if (temp != 0x3B)
	{
		if (!memsErrorReported)
		{
			Log_LogString("Hiba: SPI-on nem a MEMS válaszol! A JP3 jumper nyitva van?\n\r",LOGLEVEL_NORMAL);
			memsErrorReported = 1;
		}
		*Xg = 0x00;
		*Yg = 0x00;
		*Zg = 0x00;
		return 1;
	}
	else
	{
		HAL_Delay(50);
		LIS302DL_Read(uBuffer, LIS302DL_OUT_X_ADDR, 6);
		*Xg = (signed char)(uBuffer[2])/2;
		*Yg = (signed char)(uBuffer[0])/2;
		*Zg = (signed char)(uBuffer[4])/2;
		return 0;
	}
}

/* ----------------- Belső függvények --------------------------- */

/** LIS302DL IC inicializálása. Az Accelerometer_Init() hívja. */
void LIS302DL_Init(LIS302DL_InitTypeDef *LIS302DL_InitStruct)
{
  uint8_t ctrl = 0x00;

  /* MEMS beállítások: data rate, power mode, full scale, self test and axes */
  ctrl = (uint8_t) (LIS302DL_InitStruct->Output_DataRate | LIS302DL_InitStruct->Power_Mode | \
                    LIS302DL_InitStruct->Full_Scale | LIS302DL_InitStruct->Self_Test | \
                    LIS302DL_InitStruct->Axes_Enable);

  /* A beállítások kiírása a CTRL_REG1 regiszterbe. */
  LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG1_ADDR, 1);
}

/** A LIS302DL filer beállítása, az Accelerometer_Init() hívja. */
void LIS302DL_FilterConfig(LIS302DL_FilterConfigTypeDef *LIS302DL_FilterConfigStruct)
{
  uint8_t ctrl = 0x00;

  /* CTRL_REG2 regiszter olvasása. */
  LIS302DL_Read(&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);

  /* A módosítandó beállításokat először töröljük. */
  ctrl &= (uint8_t)~(LIS302DL_FILTEREDDATASELECTION_OUTPUTREGISTER | \
                     LIS302DL_HIGHPASSFILTER_LEVEL_3 | \
                     LIS302DL_HIGHPASSFILTERINTERRUPT_1_2);
  /* Majd beállítjuk: MEMS high pass filter cut-off level, interrupt és data selection bitek. */
  ctrl |= (uint8_t)(LIS302DL_FilterConfigStruct->HighPassFilter_Data_Selection | \
                    LIS302DL_FilterConfigStruct->HighPassFilter_CutOff_Frequency | \
                    LIS302DL_FilterConfigStruct->HighPassFilter_Interrupt);

  /* Eredmény visszaírása a CTRL_REG2 regiszterbe. */
  LIS302DL_Write(&ctrl, LIS302DL_CTRL_REG2_ADDR, 1);
}

/** A LIS302DL adott (WriteAddr) regiszterének írása. */
void LIS302DL_Write(uint8_t* pData, uint8_t WriteAddr, uint16_t dataSize)
{
	if(dataSize > 0x01)
	{
		// Módosítani kell az első elküldött byte-ot a több byte-os parancs jelzéséhez.
		WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
	}

	// TODO 1. SPI Feladat
	// Az spi.h által rendelkezésre bocsátott függvények segítségével válassza ki a LIS302DL
	// IC-t (chip select), küldje el neki a WriteAddr értékét (írandó regiszter kiválasztása),
	// majd a paraméterül kapott adatokat, végül pedig kapcsolja ki a CS jelet.
}

/** A LIS302DL adott regiszterének olvasása. */
void LIS302DL_Read(uint8_t* pData, uint8_t ReadAddr, uint16_t dataSize)
{
	// Az első byte-ban jelöljük az olvasási szándékot és a több byte-os olvasást is.
	if(dataSize > 0x01)
	{
		ReadAddr |= (uint8_t)(READWRITE_CMD | MULTIPLEBYTE_CMD);
	}
	else
	{
		ReadAddr |= (uint8_t)READWRITE_CMD;
	}

	// TODO 1. SPI Feladat
	// Az spi.h által rendelkezésre bocsátott függvények segítségével válassza ki a LIS302DL
	// IC-t, küldje el neki a ReadAddr értékét (olvasandó regiszter kiválasztása),
	// majd fogadjon dataSize számú bájtot és mentse el őket a pData által mutatott bufferbe.
	// Végül kapcsolja ki a CS jelet.
	// Törölje a keretprogram csupa nullát visszaadó részét.
	// (Ne feledkezzen meg a bsp_spi.c fájlban lévő feladatokról sem!)

	for(int i=0; i<dataSize; i++)
	{
		pData[i]=0;
	}
}
