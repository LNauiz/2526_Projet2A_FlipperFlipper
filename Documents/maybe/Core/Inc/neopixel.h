/*
 * neopixel.h
 *
 *  Created on: Feb 10, 2026
 *      Author: johann
 */

#ifndef INC_NEOPIXEL_H_
#define INC_NEOPIXEL_H_
#include <stdint.h>

void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void led_render();



#endif /* INC_NEOPIXEL_H_ */
