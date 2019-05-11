/** 
 * Az I2C kommunikációs feladat megoldást ebben a fájlban készítse el.
 * Az itt elkészült függvények elsődleges feladata a thermometer.c kommunikációs funkcióinak kiszolgálása.
 *
 * Ehhez egyrészt inicializálni kell az I2C kommunikációt mind a processzor lábainak megfelelő beállításával,
 * mind az I2C1 periféria megfelelő beállításával. (HAL_I2C_MspInit() és I2C_Init() függvények.)
 *
 * Ezután a I2C_ReadRegister() és I2C_WriteRegister() felelősek egy I2C periféria egy adott regiszterének írásáért vagy
 * olvasásáért. Ebben a HAL nagyon sokat segít nekünk. A háttérben lévő kommunikáció tartalmaz egy start conditiont, a
 * periféria megcímzését, a kért regiszter címének elküldését, majd a tényleges adatok küldését vagy fogadását.
 * Olvasáskor külön komplikációt jelent, hogy először írni kell (a kért regiszter címét), utána pedig olvasni, amit az
 * I2C protokoll keretein belül egy restart condition és egy újracímzés segítségével lehet megoldni.
 * Ezt a HAL mind megoldja helyettünk.
 * */

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "bsp_globalfunctions.h"

/* ----------------- Belső, globális változók ------------------- */

// A tényleges I2C periféria beállítására és hivatkozására használt struct.
I2C_HandleTypeDef hi2c;

const uint32_t i2c_timeout = 5000;

/* ----------------- Publikus függvények ------------------------ */

/** I2C kommunikáció inicializálása. */
HAL_StatusTypeDef I2C_Init(uint32_t ownAddress)
{
	// A hi2c handler struct Instance és Init mezőit kitöltve, majd a HAL_I2C_Init() függvényt
	// meghívva inicializálja az I2C1 perifériát (ez lesz az Instance):
	// (A saját címen és a clock speeden kívül mindenre van konstans I2C_ kezdettel.)
	// 7 bites címzés, clock speed 100000, nincs kettős címzés, duty cycle 2,
	//	general call és nostretch letiltva.
	// Ne felejtse el meghívni a HAL_I2C_Init() függvényt, melynek paramétere pointer, visszatérési
	//	értékét pedig tovább is adhatja returnnel.
	// (Az OwnAddress2 beállítása nem szükséges. Annak csak dual addressing mode esetén van értelme.)
	hi2c.Instance = I2C1;
	hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c.Init.ClockSpeed = 100000;
	hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	hi2c.Init.OwnAddress1 = ownAddress;
	return HAL_I2C_Init(&hi2c);
	return HAL_OK;
}

/** Regiszter olvasása I2C-n keresztül. */
HAL_StatusTypeDef I2C_ReadRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{
	// A kapott paraméterek és a HAL_I2C_Mem_Read() függvény segítségével olvasson be a "deviceAddress" eszköz
	//	"registerAddress" regiszteréből (cím hossza 1 byte) "dataSize" bájtot a "pData" által mutatott bufferbe.
	//	A timeout paraméternek a fentebb definiált "i2c_timeout" globális konstanst használja.
	// Törölje a keretprogram csupa nullát visszaadó részét.
	return HAL_I2C_Mem_Read(&hi2c, deviceAddress, (uint16_t)registerAddress, 1, pData, dataSize, i2c_timeout);
	// Megjegyzés: Az registerAddress castolása nem kötelező.
	for(int i=0; i<dataSize; i++)
	{
		pData[i]=0;
	}
	return HAL_OK;
}

/** Regiszter írása I2C-n keresztül. */
HAL_StatusTypeDef I2C_WriteRegister(uint16_t deviceAddress, uint8_t registerAddress, uint8_t *pData, uint16_t dataSize)
{
	// A paraméterek és a HAL_I2C_Mem_Write() függvény segítségével a beolvasáshoz hasonlóan küldje el
	//	a küldendő adatokat. A memória cím méretét byteban kell megadni (1 byte).
	return HAL_I2C_Mem_Write(&hi2c, deviceAddress, (uint16_t)registerAddress, 1, pData, dataSize, i2c_timeout);
	// Megjegyzés: Az registerAddress castolása nem kötelező.
	return HAL_OK;
}

/* ----------------- Belső függvények --------------------------- */

/** Alacsony szintű inicializálás, a HAL_I2C_Init() hívja. */
void HAL_I2C_MspInit(I2C_HandleTypeDef *handle)
{
	UNUSED(handle);

	// Engedélyezze a B port és az I2C1 periféria órajelét a __<periférianeve>_CLK_ENABLE() makrókkal.
	__GPIOB_CLK_ENABLE();
	__I2C1_CLK_ENABLE();

	// Az UART esethez hasonlóan egy GPIO_InitTypeDef típusú változón keresztül állítsa be a PB6 és PB9 lábakat:
	// Alternate function open-drain, pullup, fast. Az "Alternate" function az I2C1 periféria: GPIO_AF4_I2C1.
	// Ne felejtse el meghívni a HAL_GPIO_Init() függvényt!
	GPIO_InitTypeDef portInit;
	portInit.Mode = GPIO_MODE_AF_OD;
	portInit.Pull = GPIO_PULLUP;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_6 | GPIO_PIN_9;
	portInit.Alternate = GPIO_AF4_I2C1;
	HAL_GPIO_Init(GPIOB, &portInit);
}
