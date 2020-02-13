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
 * @brief Functions for creation and control of game objects and score record.
 *
 */

#ifndef PM_H
#define PM_H

#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>

#include "../../libopencm3-drivers/include/hd44780.h"

#include "../include/eeprom_driver.h"

/* Ways */
#define DEF_WAY_VALUE 'n'
#define RIGHT 'r'
#define LEFT 'l'
#define UP 'u'
#define DOWN 'd'
#define START 's'

/* Game state */
#define GAME_ON 1
#define GAME_OFF 0

/* Addresses of custom symbols */
#define PM_PART_1 0x00
#define PM_PART_2 0x01
#define OBST_TARGET 0x02
#define OBST_BOMB 0x03

#define SYMBOL_SIZE 8 /* Size of custom symbol */

/* Display size 14x2 */
#define FIELD_BOUND_X 14
#define FIELD_BOUND_Y 2

#define PM_SIZE 2 /* PM symbol consists of 2 separate symbols */
#define RESET_VALUE 0x44 /* Indicate that EEPROM was cleared
			  *and score is not valid
			  */
#define CHECK_VALUE 0x55 /* Indicate that EEPROM score is valid*/
#define REC_SCORE 0 /* Value of score in EEPROM */
#define REC_STATE 1 /* State of score record in EEPROM */

#define I2C_SLAVE 0x50
#define EEPROM_ADDRESS 0x00

#define STEPS_RELOAD 5

extern volatile uint32_t sleep_ms_counter;
uint32_t steps; /* Used to count steps done and change position of bomb
		 * after certain amount of them
		 */

/**
 * @brief Delay function for hd44780 driver
 * @param ms amount of milliseconds for delay
 * @return None
 */
void sleep_ms(uint32_t ms);
/**
 * @brief Initialize randomization of coordinates for objects on LCD
 * @return None
 */
void coords_rand_init();
/**
 * @brief Create and put PM symbol into display memory
 * @return None
 */
void create_PM();
/*
 * @brief Create and put bomb symbol into display memory
 * @return None
 */
void create_bomb();
/**
 * @brief Create and put target symbol into display memory
 * @return None
 */
void create_target();
/**
 * @brief Move PM symbol to some position
 * @return None
 */
void draw_PM(uint8_t x, uint8_t y);
/**
 * @brief Move bomb symbol to some position
 * @return None
 */
void draw_bomb(uint8_t x, uint8_t y);
/**
 * @brief Move target symbol to some position
 * @return None
 */
void draw_target(uint8_t x, uint8_t y);
/**
 * @brief Detects intersection of objects
 * @param x_pm 			x coords of PM
 * @param y_pm 			y coords of PM
 * @param x_b 			x coords of bomb
 * @param y_b 			y coords of bomb
 * @param x_t			x coords of target
 * @param y_t			y coords of target
 * @param count  		score counter
 * @param game_status		shows if play on or off
 * @param way 			way to move PM symbol
 * @param timer_counter timer to limit game time, shows time left
 * @return None
 */
void update_state(uint8_t *x_pm, uint8_t *y_pm, uint8_t *x_b, uint8_t *y_b,
	   uint8_t *x_t, uint8_t *y_t, uint8_t *count, uint8_t *game_status,
	   char *way, volatile uint32_t *timer_counter);
/**
 * @brief Save record score
 * @param score Score
 * @return None
 */
void save_record(uint8_t score);
/**
 * @brief Get record from EEPROM
 * @param target Target value (score or state)
 * @return value retrived from EEPROM
 */
uint8_t get_record(uint8_t target);
/**
 * @brief Initialize EEPROM record saving
 * @return None
 */
void record_init();
/**
 * @brief Reset value of score in EEPROM to 0
 * @return None
 */
void clear_record();
/**
 * @brief Initialize blacklight
 * @return None
 */
void backlight_init();
/**
 * @brief Blacklight blink
 * @param alive time in ms to stay on
 * @param sleep time in ms to stay off
 * @return None
 */
void backlight_blink(uint32_t alive, uint32_t sleep);
#endif //PM_H
