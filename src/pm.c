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

#include "../include/pm.h"

void sleep_ms(uint32_t time)
{
	sleep_ms_counter = time;
	while (sleep_ms_counter > 0);
}

void coords_rand_init()
{
	srand(time(NULL));
	steps = 0;
}

static void clear_char(uint8_t x, uint8_t y)
{
	hd44780_putchar_xy(x, y, ' ');
}

void create_PM()
{
	uint8_t PM1_symbol[SYMBOL_SIZE] = {0x03, 0x07, 0x0E, 0x0F,
					   0x0F, 0x0F, 0x07, 0x03};
	uint8_t PM2_symbol[SYMBOL_SIZE] = {0x18, 0x1C, 0x1C, 0x18,
					   0x10, 0x1E, 0x1C, 0x18};

	hd44780_define_char(0x40, PM1_symbol, SYMBOL_SIZE);
	hd44780_define_char(0x48, PM2_symbol, SYMBOL_SIZE);
}

void create_target()
{
	uint8_t target_symbol[SYMBOL_SIZE] = {0x0, 0x0, 0x1C, 0x1C,
					      0x1C, 0x0, 0x0, 0x0};

	hd44780_define_char(0x50, target_symbol, SYMBOL_SIZE);
}

void create_bomb()
{
	uint8_t bomb_symbol[SYMBOL_SIZE] = {0x0, 0x0, 0x14, 0x8,
					    0x14, 0x0, 0x0, 0x0};

	hd44780_define_char(0x58, bomb_symbol, SYMBOL_SIZE);
}

void draw_PM(uint8_t x, uint8_t y)
{
	hd44780_putchar_xy(x, y, PM_PART_1);
	hd44780_putchar_xy(x + 1, y, PM_PART_2);
}

void draw_bomb(uint8_t x, uint8_t y)
{
	hd44780_putchar_xy(x, y, OBST_BOMB);

}

void draw_target(uint8_t x, uint8_t y)
{
	hd44780_putchar_xy(x, y, OBST_TARGET);
}

void backlight_init()
{
	rcc_periph_clock_enable(RCC_GPIOE);

	gpio_clear(GPIOE, GPIO9);
	gpio_mode_setup(GPIOE, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);

	gpio_set(GPIOE, GPIO9);
}

void backlight_blink(uint32_t alive, uint32_t sleep)
{
	gpio_clear(GPIOE, GPIO9);
	sleep_ms(sleep);

	gpio_set(GPIOE, GPIO9);
	sleep_ms(alive);

	gpio_clear(GPIOE, GPIO9);
	sleep_ms(sleep);

	gpio_set(GPIOE, GPIO9);
	sleep_ms(alive);

	gpio_clear(GPIOE, GPIO9);
	sleep_ms(sleep);

	gpio_set(GPIOE, GPIO9);
}

static void move_PM(char way, uint8_t *curr_x, uint8_t *curr_y)
{
	switch (way) {
	case RIGHT:
		/* Check if PM on the edge of display */
		if (*curr_x == FIELD_BOUND_X - PM_SIZE)
			break;

		/* Clear previous position of PM */
		if (*curr_x >= 0)
			clear_char(*curr_x, *curr_y);

		*curr_x += 1;
		break;

	case LEFT:
		if (*curr_x == 0)
			break;

		clear_char(*curr_x + 1, *curr_y);

		*curr_x -= 1;
		break;

	case UP:
		if (*curr_y == 0)
			break;

		clear_char(*curr_x, *curr_y);
		clear_char(*curr_x + 1, *curr_y);

		*curr_y -= 1;
		break;

	case DOWN:
		if (*curr_y == 1)
			break;

		clear_char(*curr_x, *curr_y);
		clear_char(*curr_x + 1, *curr_y);

		*curr_y += 1;
		break;

	case START:
		clear_char(*curr_x, *curr_y);
		clear_char(*curr_x + 1, *curr_y);

		*curr_x = 0;
		*curr_y = 0;

		break;
	}

	draw_PM(*curr_x, *curr_y);

	steps++;
	return;
}

void update_state(uint8_t *x_pm, uint8_t *y_pm, uint8_t *x_b, uint8_t *y_b,
	   uint8_t *x_t, uint8_t *y_t, uint8_t *count, uint8_t *game_status,
	   char *way, volatile uint32_t *timer_counter)
{
	uint8_t x_rand;

	/* Change position of PM */
	if (*way != DEF_WAY_VALUE) {
		move_PM(*way, x_pm, y_pm);
		*way = DEF_WAY_VALUE;
	}

	/* Check if PM on the same line with any obstacle */
	if (*y_pm != *y_b && *y_pm != *y_t)
		goto exit;

	/* Check if there any intersection between PM and bomb occured.
	*  If that is true: finish the game.
	*/
	if ((*x_pm == *x_b || *x_pm + 1 == *x_b) && *y_pm == *y_b) {
		*game_status = GAME_OFF;
		*timer_counter = 0;

		backlight_blink(100, 100);
		goto exit;
	}

	/* Check if there any intersection between PM and target occured.
	*  If that is true: change position of target, increment score.
	*/
	if ((*x_pm == *x_t || *x_pm + 1 == *x_t) && *y_pm == *y_t) {
		backlight_blink(100, 100);
		steps += 1;

		x_rand = rand() % FIELD_BOUND_X;

		/* Searching for new coords */
		while (x_rand == *x_pm || x_rand == *x_pm + 1 || x_rand == *x_b)
			x_rand = rand() % FIELD_BOUND_X;

		*x_t = x_rand;
		*y_t = rand() % FIELD_BOUND_Y;

		draw_target(*x_t, *y_t);

		*count += 1;
		goto print;
	}

	/* Check if amount of steps to change bomb position reached.
	*  If that is true: change position of bomb.
	*  Position changes every STEPS_RELOAD steps.
	*/
	if (steps != 0 && steps % STEPS_RELOAD == 0) {
		clear_char(*x_b, *y_b);

		x_rand = rand() % FIELD_BOUND_X;
		while (x_rand - 1 == *x_pm || x_rand == *x_pm + 2 ||
		       x_rand == *x_pm || x_rand == *x_pm + 1 ||
		       x_rand == *x_t || (x_rand == 12 && *x_t == 13) ||
		       (x_rand == 1 && *x_t == 0)){
			x_rand = rand() % FIELD_BOUND_X;
		}

		*x_b = x_rand;
		*y_b = rand() % FIELD_BOUND_Y;

		draw_bomb(*x_b, *y_b);
		steps += 1;
	}
	goto exit;

	print:
		hd44780_printf_xy(14, 0, "%d", (*count / 10));
		hd44780_printf_xy(15, 0, "%d", *count % 10);
	exit:
		return;
}
void record_init()
{
	eeprom_init(I2C1, GPIOB, RCC_I2C1, RCC_GPIOB, GPIO6, GPIO9);

	uint8_t rec_state = get_record(REC_STATE);
	/*Check if EEPROM was already written to*/
	if (rec_state != CHECK_VALUE)
		save_record(0);
}

void clear_record()
{
	uint8_t size = 2;
	uint8_t buff[size];

	buff[0] = 0;
	buff[1] = RESET_VALUE;

	eeprom_write(I2C1, I2C_SLAVE, EEPROM_ADDRESS, buff, size);
}

uint8_t get_record(uint8_t target)
{
	uint8_t size = 2;
	uint8_t buff[size];

	eeprom_read(I2C1, I2C_SLAVE, EEPROM_ADDRESS, buff, size);
	return buff[target];
}

void save_record(uint8_t score)
{
	uint32_t size = 2;
	uint8_t buff[size];

	buff[0] = score;
	buff[1] = CHECK_VALUE;

	eeprom_write(I2C1, I2C_SLAVE, EEPROM_ADDRESS, buff, size);
}
