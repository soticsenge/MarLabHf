/**
*****************************************************************************
** Kommunikációs mérés - glcd.c
** A grafikus LCD függvényei
*****************************************************************************
*/
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_cortex.h"

#include "bsp_glcd.h"
#include "bsp_globalfunctions.h"
#include "font.h"

void GLCD_MSP_Init(void);

/* ----------------- Publikus függvények ----------------- */

void GLCD_Init(void)
{
	GLCD_MSP_Init();

	// Init GLCD functions
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);	// GLCD_E = 0
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);	// GLCD_RESET = 1

	GLCD_Write(3,0,0x3F); 	//GLCD bekapcsolása
	GLCD_Write(3,0,0xC0);	//GLCD Start Line
	GLCD_Clear();			//Kijelzõ törlése
}

void GLCDEN(int newState)
{
	if(newState)
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
	}
}

/** Beírja a g_data értéket a kiválasztott kijelzõvezérlõbe (cs_s->CS1, CS2) utasítás/adat paraméternek megfelelõen.
* Általánosan felhasználható 8bit (adat/utasítás) beírására a kijelzõ vezérlõjébe.
* 	cs_s, 1 = CS1, 2 = CS2, 3 = CS1&CS2
* 	d_i, 0 = instruction, 1 = data
*/
void GLCD_Write(char cs_s,char d_i,char g_data)
{
	uint16_t data = 0x0000;
	switch(cs_s)
	{
	case 1:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
		break;
	}
	switch(d_i)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);
		break;
	}

	data = GPIOE->IDR;
	data &= 0x00FF;
	data |= g_data << 8;
	GPIOE->ODR = data;

	Sys_DelayUs(1);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);
	Sys_DelayUs(2);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_RESET);
	Sys_DelayUs(4);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
}

/** Kijelző törlése. */
void GLCD_Clear(void)
{
	char x,y;
	for(x=0;x<8;x++)
	{
		GLCD_Write(3,0,0x40);
		GLCD_Write(3,0,(0xB8|x));
		for(y=0;y<64;y++)
		{
			GLCD_Write(3,1,0x00);
		}
	}
}

/** A kijelzõ adott sor-oszlop metszetének beállítása az m_data értéknek megfelelõen.
 * 		m_data: adott metszet rajzolata hexába kódolva
 * 		cX: sor (0-7)
 * 		cY: oszlop (0-127) */
void GLCD_Write_Block(char m_data,char cX,char cY)
{
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	GLCD_Write(chip,1,m_data);
}

/** String kiírása a kijelzőre. */
void GLCD_WriteString(const char* string,char Y, char X)
{
	char temp = 0;
	int i=0;
	while(string[i]!='\0')
	{
		temp = string[i];
		GLCD_Write_Char(temp-32,X,Y+6*i);
		i++;
	}
}

/** Karakter írása a kijelzőre. */
void GLCD_Write_Char(char cPlace,char cX,char cY)
{
	char i=0;
	char chip=1;
	if(cY>=64)
	{
		chip=2;
		cY-=64;
	}
	GLCD_Write(chip,0,(0x40|cY));
	GLCD_Write(chip,0,(0xB8|cX));
	for (i=0;i<5;i++)
	{
	  if (cY+i >= 64)
	  {
		  chip=2;
		  GLCD_Write(chip,0,(0x40|(cY+i-64)));
		  GLCD_Write(chip,0,(0xB8|cX));
	  }
	  GLCD_Write(chip,1,fontdata[cPlace*5+i]);
	}
}

/* ----------------- Belső függvények ----------------- */

/** Alacsony szintű inicializálás. */
void GLCD_MSP_Init(void)
{
	// Init ports
	__GPIOE_CLK_ENABLE();
	__GPIOD_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();
	__GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef portInit;
	/* data: PE8-15
	 * RW: PE7
	 * CS: PB4-5
	 * E: PD7
	 * DI: PD6
	 * RESET: PD3
	 * GLCDEN: PB7
	 * Backlight PWM: PC8
	 */
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;

	// Port E
	portInit.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	HAL_GPIO_Init(GPIOE, &portInit);
	portInit.Pin = GPIO_PIN_7;
	HAL_GPIO_Init(GPIOE, &portInit);
	// Port D
	portInit.Pin = GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOD, &portInit);
	// Port C
	portInit.Pin = GPIO_PIN_8;
	HAL_GPIO_Init(GPIOC, &portInit);
	// Port B
	portInit.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
	HAL_GPIO_Init(GPIOB, &portInit);

	// Minden alapállapotba
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_RESET);	// GLCD_RW = 0
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, GPIO_PIN_SET);		// GLCD_E = 1
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);	// GLCD_RESET = 0
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);	// #GLCDEN = 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);		// Backlight = 1 (PWM 100%)
}
