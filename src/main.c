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

#include "../include/main.h"

static void coords_init(void);

int main(void)
{
	/* Initialization */
	game_status = GAME_OFF;
	way = DEF_WAY_VALUE;
	clear_rec = CLEAR_OFF;
	score = 0;
	timer_counter = 0;

	/* Peripherals initialization */
	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);
	led_init_all();
	button_init();
	button_isr_init();
	backlight_init();
	systick_setup();
	hd44780_init(&lcd_bus, 16, false, 2, false);

	/* Game objects initialization */
	create_PM();
	create_bomb();
	create_target();
	record_init();

	hd44780_printf_xy(1, 0, "START THE GAME");

	/* Main loop of the game */
	while (1) {
		/*Clear record in EEPROM if needed*/
		if (clear_rec == CLEAR_ON) {
			clear_record();
			sleep_ms(100);
			clear_rec = CLEAR_OFF;
		}

		/*Check if game should be started*/
		if (game_status) {
			/* LEDs reset */
			gpio_clear(GPIOD, GPIO14);
			gpio_clear(GPIOD, GPIO12);

			/* Initializate start game screen */
			hd44780_clear();
			hd44780_home();
			coords_init();

			draw_PM(PM_x, PM_y);
			draw_bomb(bomb_x, bomb_y);
			draw_target(target_x, target_y);

			/* Print score */
			hd44780_printf_xy(14, 0, "%d", score);
			hd44780_printf_xy(15, 0, "%d", score);
			/* Print timer */
			hd44780_printf_xy(14, 1, "%d",
					  (GAME_TIME / SEC_SCALLER) / 10);
			hd44780_printf_xy(15, 1, "%d",
					  (GAME_TIME / SEC_SCALLER) % 10);

			/* Setting the game timer.One second overload for delay
			 * before the whole image will be displayed.
			 */
			timer_counter = GAME_TIME + ONE_SEC;

			while (game_status && timer_counter) {
				/* Output time left */
				hd44780_printf_xy(
					14, 1, "%d",
					(timer_counter / SEC_SCALLER) / 10);
				hd44780_printf_xy(
					15, 1, "%d",
					(timer_counter / SEC_SCALLER) % 10);

				/* Check for intersections */
				update_state(&PM_x, &PM_y, &bomb_x, &bomb_y,
				      &target_x, &target_y, &score,
				      &game_status, &way, &timer_counter);

				 /* Turn on green LED*/
				if (score > 0)
					gpio_set(GPIOD, GPIO12);
				else
					gpio_clear(GPIOD, GPIO12);
			}
			/* Turn on red LED */
			gpio_set(GPIOD, GPIO14);

			if (timer_counter == 0 && game_status == GAME_ON) {
				hd44780_clear();
				hd44780_home();
				hd44780_printf_xy(2, 0, "TIME IS OVER!");
				sleep_ms(1000);
			}

			game_status = GAME_OFF;

			/* Check value of record score in EEPROM */
			uint8_t curr_rec = get_record(REC_SCORE);
			if (curr_rec < score) {
				save_record(score);
				curr_rec = score;
			}

			/* Finall screen */
			hd44780_clear();
			hd44780_home();
			hd44780_printf_xy(1, 0, "GAME");
			hd44780_printf_xy(1, 1, "OVER");

			hd44780_home();
			hd44780_printf_xy(6, 0, "SCORE");
			hd44780_printf_xy(8, 1, "%d", (score / 10));
			hd44780_printf_xy(9, 1, "%d", score % 10);

			hd44780_home();
			hd44780_printf_xy(12, 0, "BEST");
			hd44780_printf_xy(14, 1, "%u", (curr_rec / 10));
			hd44780_printf_xy(15, 1, "%u", curr_rec % 10);
			draw_PM(6, 1);

			score = 0;
		}
	}
	return 0;
}

/* Set default position of objects */
static void coords_init(void)
{
	coords_rand_init();
	bomb_x = BOMB_X_INIT;
	bomb_y = BOMB_Y_INIT;
	target_x = TARGET_X_INIT;
	target_y = TARGET_Y_INIT;
	PM_x = PM_X_INIT;
	PM_y = PM_X_INIT;
}