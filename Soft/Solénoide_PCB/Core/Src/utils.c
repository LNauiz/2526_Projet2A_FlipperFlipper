/*
 * utils.c
 *
 *  Created on: Nov 18, 2025
 *      Author: leona
 */

#include <stdio.h>
/*#include "gpio.h"*/
#include "stm32g4xx_hal.h"

void setup(void){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	printf("Hello World! \r\n");
}

void loop(void){
	if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8)==1){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
		printf("Tir \n\r");
		}
	else{
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	}
}
