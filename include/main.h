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
 */

/**
 * Example description:
 *
 * Game which uses HD44780 as display and buttons to control movement of PM
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <stdbool.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "../../libopencm3-drivers/include/hd44780.h"

#include "../include/pm.h"
#include "../include/pm_isr.h"
#include "../include/periph_init.h"

#define SET 1
#define RESET 0

#define ONE_SEC 1000
#define SEC_SCALLER ONE_SEC
#define GAME_TIME ONE_SEC * 30

/* Initial coords of objects */
#define BOMB_X_INIT 5
#define BOMB_Y_INIT 0
#define TARGET_X_INIT 7
#define TARGET_Y_INIT 1
#define PM_X_INIT 0
#define PM_Y_INIT 0

/* Shows if EEPROM record should be cleared */
#define CLEAR_ON 1
#define CLEAR_OFF 0

static struct hd44780_bus lcd_bus = {
	.rs = { GPIOE, GPIO7 },
	.e = { GPIOE, GPIO11 },
	.rnw = { GPIOE, GPIO10 },
	.db7 = { GPIOE, GPIO15 },
	.db6 = { GPIOE, GPIO14 },
	.db5 = { GPIOE, GPIO13 },
	.db4 = { GPIOE, GPIO12 },
};

/* Global variables of objects position */
uint8_t PM_x;
uint8_t PM_y;
uint8_t bomb_x;
uint8_t bomb_y;
uint8_t target_x;
uint8_t target_y;

uint8_t score;
uint8_t game_status;
char way;
uint8_t clear_rec; /* Shows if record in EEPROM should be cleared */

volatile uint32_t timer_counter;
volatile uint32_t sleep_ms_counter;

#endif //MAIN_H