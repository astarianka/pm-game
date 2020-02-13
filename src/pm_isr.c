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
 * @brief Interrupt handlers for buttons and systick.
 *
 */

#include "../include/pm_isr.h"

/* Interrupt of button A0 (User button).
 *  Used for reseting record score on EEPROM
 */
void exti0_isr(void)
{
	exti_reset_request(EXTI0);

	if (clear_rec == CLEAR_OFF)
		clear_rec = CLEAR_ON;

	if (game_status)
		clear_rec = CLEAR_OFF;
}

/* Interrupts for direction buttons */
void exti9_5_isr(void)
{
	/* SWT4 */
	if (exti_get_flag_status(EXTI6) != RESET) {
		way = UP;
		exti_reset_request(EXTI6);
	}
	/* SWT5 */
	if (exti_get_flag_status(EXTI8) != RESET) {
		way = DOWN;
		exti_reset_request(EXTI8);
	}
	/* SWT3 */
	if (exti_get_flag_status(EXTI9) != RESET) {
		way = LEFT;
		exti_reset_request(EXTI9);
	}
	/* If ganme is off - way shouldn't be changed */
	if (!game_status)
		way = DEF_WAY_VALUE;
}

void exti15_10_isr(void)
{
	/* SWT1 */
	if (exti_get_flag_status(EXTI11) != RESET) {
		if (game_status)
			way = RIGHT;

		exti_reset_request(EXTI11);
	}
	/* SWT2 */
	if (exti_get_flag_status(EXTI15) != RESET) {
		game_status = GAME_ON;
		way = START;
		exti_reset_request(EXTI15);
	}
}

/* Interrupt used for timer and delay function */
void sys_tick_handler(void)
{
	if (sleep_ms_counter > 0)
		sleep_ms_counter--;

	if (timer_counter > 0)
		timer_counter--;
}