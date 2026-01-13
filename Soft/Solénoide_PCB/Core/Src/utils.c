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
	HAL_GPIO_WritePin(GPIOA, LED_Status, GPIO_PIN_SET);
}

void loop(void){
	if (HAL_GPIO_ReadPin(Bouton_Actif)==1){
		HAL_GPIO_WritePin(GPIOA,Com_Sol_A,GPIO_PIN_SET);
		HAL_delay(250);
		HAL_GPIO_WritePin(GPIOA,Com_Sol_A,GPIO_PIN_RESET);
	}
}
