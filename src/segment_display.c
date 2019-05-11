
#include "stm32f4xx.h"
//#include "stm32f4xx_hal.h"
//#include "stm32f4xx_hal_cortex.h"


const uint32_t digits [10] =
			{0x003F,	//0
			0x0006,		//1
			0x005B,		//2
			0x004F,		//3
			0x0066,		//4
			0x006D,		//5
			0x007D,		//6
			0x0007,		//7
			0x007F,		//8
			0x006F		//9
			};


int digitArray[4] = {0}; //initialize all elements to 0


void displayOneDigit(int number){

	if(0 <= number && number <= 9){
		GPIOC->ODR=digits[number];
	} else { // in case of invalid parameter display zero
		GPIOC->ODR=digits[0];
	}
}

void clear(){
	GPIOC->ODR=0x0;
}

void Init7SegDisplays(){



	__GPIOA_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	GPIO_InitTypeDef portInit;

	//7seg MUX
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_10 | GPIO_PIN_11 |GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA, &portInit);


	//Actual 7seg display
	portInit.Mode = GPIO_MODE_OUTPUT_PP;
	portInit.Pull = GPIO_NOPULL;
	portInit.Speed = GPIO_SPEED_FAST;
	portInit.Pin = GPIO_PIN_0 |GPIO_PIN_1 |GPIO_PIN_2 | GPIO_PIN_3 |GPIO_PIN_4 |GPIO_PIN_5 |GPIO_PIN_6 |GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
	HAL_GPIO_Init(GPIOC, &portInit);

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10, GPIO_PIN_RESET);



}

void convertNumberToArray(int number){
		int size = 0;
		while (number > 0)
		{
			int digit = number%10;
			number /= 10;
			digitArray[size] = digit;
			++size;
		}
}


void displayNumberOfSteps(int steps){

		convertNumberToArray(steps);

		displayOneDigit(digitArray[3]);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);
		clear();
		HAL_Delay(5);     //One second delay
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);

		displayOneDigit(digitArray[2]);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);
		clear();
		HAL_Delay(5);     //One second delay
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);

		displayOneDigit(digitArray[1]);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);
		clear();
		HAL_Delay(5);     //One second delay
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);

		displayOneDigit(digitArray[0]);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);
		clear();
		HAL_Delay(5);     //One second delay
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9, GPIO_PIN_SET);
}


