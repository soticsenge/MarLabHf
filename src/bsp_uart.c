/**
 * Az UART kommunikációs feladat megoldása.
 * Az itt található függvények elsődleges feladata a log.c logolási funkcióinak kiszolgálása, valamint
 * a  mainloop.c számára lehetővé tenni, hogy soros porton egyetlen bájtos parancsokat
 * fogadjon.
 *
 * Az inicializálás kereteiben belül egyrészt konfigurálni kell a mikrovezérlő megfelelő lábait
 * (alternate function, push-pull stb., HAL_UART_MspInit() függvény),
 * valamint az UART perifériát (baud rate, stop bitek száma stb., UART_Init() függvény).
 *
 * Ezután a UART_SendString() függvényben már meg lehet írni egy string küldését. Ezúttal
 * nem blokkoló, interrupt alapú küldést használunk, hogy küldés közben a rendszer többi
 * funkciója ne akadjon meg.
 *
 * A HAL koncepciója szerint az interruptokról callback függvényeken keresztül kapunk
 * értesítést, melyeket a beépített interrupt kezelő hív meg. Ezért néz ki az igazi interrupt kezelő
 * (USART3_IRQHandler()) függvény úgy, ahogy: csak egy már a HAL-lal kapott kezelő függvény meghívását kell
 * biztosítani.
 *
 * Fogadott adatokról értesítést a HAL_UART_RxCpltCallback() függvény segítségével kapunk, így a kapott adatok
 * feldolgozását itt kell megoldani. Ezen kívül a fogadást engedélyezni kell a UART_Init()-ben.
 * Jelen esetben a fogadott bájtot csak elmentjük a lastReceivedUartCommand globális változóba, melyet
 * a főciklus figyel a mainloop.c-ben, és szükség esetén reagál rá.
 */
 
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"
#include "string.h"
#include "bsp_globalfunctions.h"

/* ----------------- Publikus globális változók ----------------- */

/** Last received command. 0 if none. */
uint8_t lastReceivedUartCommand = 0;

/* ----------------- Belső, globális változók ----------------- */

// Leíró a UART perifériához.
UART_HandleTypeDef huart;
// Egyetlen bájtos adatfogadási buffer
char rxBuffer;
// Ide másoljuk a küldés alatt álló stringet. Az UART_SendString() használja.
#define TXBUFFERSIZE 255
char txBuffer[TXBUFFERSIZE];

/* ----------------- Publikus függvények ----------------- */

/** Inicializálás */
HAL_StatusTypeDef UART_Init()
{
	// UART inicializálása: huart.Instance és huart.Init beállítása, majd HAL_UART_Init hívása.
	// huart.Instance: ez a ténylegesen használt UART perifériára kell,
	// hogy mutasson. Jelen esetben USART3.
	// UART beállítások: 115200 baud, 8N1 (vagyis 8 adatbit van, nincs paritás és 1 stop bit van),
	// nincs flow control, és a küldést és fogadást is
	//	engedélyezni kell (UART_MODE_TX_RX). Az oversampling 16.
	// A baud rate kivételével mindenre van előre definiált konstans UART_ kezdettel.
	// Figyelem: A HAL_UART_Init() paramétere pointer típusú kell, hogy legyen!
	huart.Instance = USART2;
	huart.Init.BaudRate = 115200;
	huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart.Init.Mode = UART_MODE_TX_RX;
	huart.Init.StopBits = UART_STOPBITS_1;
	huart.Init.Parity = UART_PARITY_NONE;
	huart.Init.WordLength = UART_WORDLENGTH_8B;
	huart.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart);


	// Kiegészítő feladat a mérés végére: itt kell engedélyezni az adatok fogadását.
	// Ehhez kérje 1 byte fogadását az rxBuffer-be, nem blokkoló módon a
	//	HAL_UART_Receive_IT() meghívásával.
	return HAL_UART_Receive_IT(&huart, (uint8_t*)&rxBuffer, 1);
}

/** String küldése, nem blokkolva. */
HAL_StatusTypeDef UART_SendString(char *str)
{
	// Várjon addig, amig az UART periféria készen nem áll az adatküldésre.
	// Akkor tudunk küldeni, ha a huart.State vagy HAL_UART_STATE_READY, vagy _BUSY_RX
	//	(vagyis csak fogadás van folyamatban).
	// (Ez a várakozás részben blokkolóvá teszi az amúgy nem blokkolú küldést, de
	//	az egyszerűség kedvéért most ezt bevállaljuk, mert különben egy várakozó sort kellene
	//	készíteni a küldéseknek, ami meghaladja a mérés célkitűzését.)
	while ((huart.gState != HAL_UART_STATE_BUSY_RX) && (huart.gState != HAL_UART_STATE_READY))
	{
	}

	// Ha ide eljutottunk, akkor az UART periféria készen áll az adatküldésre.
	// A paraméterként kapott stringet nem blokkolva küldje el a HAL_UART_Transmit_IT()
	// függvény segítségével. Figyeljen rá, hogy ha visszatér ebből a függvényből, az str által
	// mutatott hely már megváltozhat, pedig még nem küldtük el. Így itt másolja át a tartalmát
	// a txBuffer-be (de csak maximum TXBUFFERSIZE bájtot)! (strncpy()).
	// A HAL_UART_Transmit_IT 2. paraméterének típusa (uint8_t*) kell legyen, így a txBuffer-t
	//	castolni kell.

	//strncpy(txBuffer, str, TXBUFFERSIZE); // --> Bugos, ezt használva néha HardFault-ra fut a program...
	//uint32_t length = strlen(txBuffer); // --> Bugos, ezt használva néha HardFault-ra fut a program...
	uint16_t length = 0;
    while(*(str+length) && length<TXBUFFERSIZE)
    {
    	*(txBuffer+length) = *(str+length);
    	length++;
    }
	//return HAL_UART_Transmit_IT(&huart, (uint8_t*)txBuffer, length);
    HAL_UART_Transmit(&huart, (uint8_t*)txBuffer, length,1000);
	return HAL_OK;
}


/* ----------------- Belső függvények ----------------- */

/** Alacsony szintű inicializálás, a HAL_UART_Init() hívja. */
void HAL_UART_MspInit(UART_HandleTypeDef* handle)
{
	UNUSED(handle);

	// Egyrészt engedélyezni kell a szükséges órajeleket (USART3 és GPIO D port)
	//	az __<periférianév>_CLK_ENABLE() makrók segítségével.
	__USART2_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();


	// Majd létre kell hozni egy GPIO_InitTypeDef típusú változót és annak kitöltésével, majd a HAL_GPIO_Init()
	//	meghívásával beállítani a lábakat. Mindenre van előre definiált konstans GPIO_ kezdettel.
	//  A beállítások: alternate function push-pull, NOPULL, gyors (FAST), az Alternate function GPIO_AF7_USART3,
	//  és a D port 8-as és 9-es pin-jeiről van szó.
	// Figyelem! A Mode beállításra az alternate function-re utaló, "GPIO_MODE_AF" kezdetű
	//	konstansokat használja! A GPIO_MODE_OUTPUT_PP nem kapcsolja össze a kimenetet az USART
	//	perifériával, mert sima GPIO portot definiál.
	GPIO_InitTypeDef portInit;
	portInit.Mode = GPIO_MODE_AF_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_2 | GPIO_PIN_3;
	portInit.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOA, &portInit);

	// Engedélyezze az USART3_IRQn megszakítást 0 Preempt és subpriority-val a
	//	HAL_NVIC_SetPriority() és HAL_NVIC_EnableIRQ() függvények segítségével.

	HAL_NVIC_SetPriority(USART2_IRQn,0,0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
;
}

/* ----------------- Megszakításkezelő és callback függvények ----------------- */

/** Callback függvény, mely sikeres adatfogadás végét jelzi. */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle)
{
	// Ennek a callback függvénynek a meghívásakor a fogadott adatok az rxBufferben vannak,
	// mivel az Init végén oda kértük a mentésüket.
	// Megjegyzés: a handle->pRxBuffPtr a következő szabad bájtra mutat, nem a buffer elejére!
	// Mentse el azt az egy bájtot a lastReceivedUartCommand globális változóba, majd
	//  az HAL_UART_Receive_IT() meghívásával kérje a következő bájt fogadását.
	//	Ha ez utóbbi elmarad, akkor a további bejövő adatokról nem szerzünk majd tudomást,
	//	mert nem kértük fogadásukat.
	lastReceivedUartCommand = rxBuffer;
	HAL_UART_Receive_IT(handle, (uint8_t*)&rxBuffer, 1);
}

/** Interrupt kezelő. */
void USART2_IRQHandler(void)
{
	HAL_UART_IRQHandler(&huart);
}
