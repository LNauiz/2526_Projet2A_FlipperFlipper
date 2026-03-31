#include "stm32l4xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern DMA_HandleTypeDef hdma_tim1_ch1;

#define NUM_PIXELS 7
#define NUM_BPP    3
#define NUM_BYTES  (NUM_PIXELS * NUM_BPP)

// With ARR=19:
// Logic 0: ~30% (Pulse = 6)
// Logic 1: ~65% (Pulse = 13)
#define PWM_HI (13)
#define PWM_LO (6)

// Reset pulse needs >50us. 40 zeros at 800kHz = 50us exactly.
#define WR_BUF_LEN (NUM_BYTES * 8 + 40)
uint8_t wr_buf[WR_BUF_LEN];
uint8_t rgb_arr[NUM_BYTES];

void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
    if (index >= NUM_PIXELS) return;
    // WS2812B uses GRB order
    rgb_arr[3 * index]     = g;
    rgb_arr[3 * index + 1] = r;
    rgb_arr[3 * index + 2] = b;
}

void led_render() {
    // 1. Wait for DMA to be ready
    while (hdma_tim1_ch1.State != HAL_DMA_STATE_READY);

    // 2. Fill PWM data
    for(uint_fast8_t i = 0; i < NUM_BYTES; ++i) {
        for(uint_fast8_t bit = 0; bit < 8; ++bit) {
            if(rgb_arr[i] & (0x80 >> bit)) {
                wr_buf[i * 8 + bit] = PWM_HI;
            } else {
                wr_buf[i * 8 + bit] = PWM_LO;
            }
        }
    }

    // 3. Fill the rest of the buffer with 0 for the Reset pulse
    for(uint_fast8_t i = NUM_BYTES * 8; i < WR_BUF_LEN; ++i) {
        wr_buf[i] = 0;
    }

    // 4. Start DMA
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)wr_buf, WR_BUF_LEN);
}

// Simple Rainbow Test Function
void led_rainbow_test(uint8_t speed) {
    static uint8_t j = 0;
    for (int i = 0; i < NUM_PIXELS; i++) {
        uint8_t pos = ((i * 256 / NUM_PIXELS) + j) & 255;
        if(pos < 85) {
            led_set_RGB(i, 255 - pos * 3, 0, pos * 3);
        } else if(pos < 170) {
            pos -= 85;
            led_set_RGB(i, 0, pos * 3, 255 - pos * 3);
        } else {
            pos -= 170;
            led_set_RGB(i, pos * 3, 255 - pos * 3, 0);
        }
    }
    led_render();
    j++;
    HAL_Delay(speed);
}
