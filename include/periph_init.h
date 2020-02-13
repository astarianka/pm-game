/**
 * Copyright (C) 2019, Anna Bezverkhnia
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 *
 * @brief Functions for peripheral initialization
 *
 */

#ifndef PERIPH_INIT
#define PERIPH_INIT

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

/**
 * @brief  Initialize button peripherals
 * @return None
 */
void button_init(void);
/**
 * @brief  Initialize button peripherals interrupt
 * @return None
 */
void button_isr_init(void);
/**
 * @brief  Initialize particular led
 * @param  gpio GPIO pin of LED
 * @return None
 */
void led_init(uint16_t gpio);
/**
 * @brief  Initialize all LEDs
 * @return None
 */
void led_init_all(void);
/**
 * @brief  Set up systick to generate interrupts
 * @return None
 */
void systick_setup(void);

#endif //PERIPH_INIT