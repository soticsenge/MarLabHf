#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "bsp_buttonsswitches.h"

/* ----------------- Belső globális deklarációk ----------------- */

#define 	BUTTON_BTN1	(uint16_t)1<<12
#define 	BUTTON_BTN2	(uint16_t)1<<13
#define 	BUTTON_BTN3	(uint16_t)1<<14
#define 	BUTTON_BTN4	(uint16_t)1<<15

#define 	BUTTON_ALL	(uint16_t)0xf000

/* ----------------- Publikus függvények ------------------------ */

void ButtonsSwitches_Init()
{
	// __GPIOE_CLK_ENABLE();
	//__GPIOC_CLK_ENABLE();

	__GPIOB_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	// Configure switch inputs
	GPIO_InitTypeDef portInit;


	//7seg button
	portInit.Mode = GPIO_MODE_INPUT;
	portInit.Pull = GPIO_PULLUP;;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_7;

	HAL_GPIO_Init(GPIOB, &portInit);

	//7seg digit
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_9 |GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7 |GPIO_PIN_0 |GPIO_PIN_1;
	HAL_GPIO_Init(GPIOC, &portInit);

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_1, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7, GPIO_PIN_SET);



		GPIOC->ODR=0x0006;


	//7seg en
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_10 | GPIO_PIN_11 |GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA, &portInit);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);



//	portInit.Mode = GPIO_MODE_INPUT;
//	portInit.Pull = GPIO_NOPULL;
//	portInit.Speed = GPIO_SPEED_FAST;
//	portInit.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;
//	HAL_GPIO_Init(GPIOE, &portInit);

	// SWEN output
//	portInit.Mode = GPIO_MODE_OUTPUT_PP;
//	portInit.Pull = GPIO_NOPULL;
//	portInit.Speed = GPIO_SPEED_FAST;
//	portInit.Pin = GPIO_PIN_6;
//	HAL_GPIO_Init(GPIOC, &portInit);

	// Enable switch inputs
	//HAL_GPIO_WritePin(GPIOB,GPIO_PIN_7, GPIO_PIN_SET);
}

/** SW3 beolvasása */
int ButtonsSwitches_GetTestSwitchState()
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_5);
}

int ButtonsSwitches_GetTestButtonState()
{
	return HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
}

/** SW4 beolvasása */
int ButtonsSwitches_GetGraphicsModeSwitchState()
{
	return HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_6);
}

/** SW1 és SW2 beolvasása*/
uint8_t ButtonsSwitches_GetLocalZoneID()
{
	// Kapcsolók beolvasása: SW1: PE2, SW2: PE4
	uint8_t localZoneIDpin0 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2);
	uint8_t localZoneIDpin1 = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4);

	uint8_t localZoneID = localZoneIDpin0 + (localZoneIDpin1<<1);

	return localZoneID;
}
