#include <stdio.h>
/*#include "gpio.h"*/
#include "stm32g4xx_hal.h"
#include "main.h"


void setup(){
	println("Hello Worlds \n\r");
	HAL_GPIO_WritePin(LED_Status_GIO_Port,LED_Status_Pin,1);
}
//LED_STATUS_GPIO_Port

void loop(){
	if (HAL_GPIO_ReadPin(Bouton_Droit_GPIO_Port,Bouton_Droit_Pin)){
		HAL_GPIO_WritePin(Com_Sol_A_GPIO_Port,Com_Sol_A_Pin,1);
	}
	else{
		HAL_GPIO_WritePin(Com_Sol_A_GPIO_Port,Com_Sol_A_Pin,0);
	};
	if (HAL_GPIO_ReadPin(Bouton_Gauche_GPIO_Port,Bouton_Gauche_Pin)){
		HAL_GPIO_WritePin(Com_Sol_B_GPIO_Port,Com_Sol_B_Pin,1);
	}
	else{
		HAL_GPIO_WritePin(Com_Sol_B_GPIO_Port,Com_Sol_B_Pin,0);
	}
};


