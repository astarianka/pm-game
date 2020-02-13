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

#include "../include/periph_init.h"

void button_init(void)
{
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOA);

	gpio_mode_setup(GPIOC, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP,
			GPIO6 | GPIO8 | GPIO9 | GPIO11);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO15);
	gpio_mode_setup(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
}
void button_isr_init(void)
{
	rcc_periph_clock_enable(RCC_SYSCFG);

	nvic_enable_irq(NVIC_EXTI0_IRQ);
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);

	exti_select_source(EXTI6 | EXTI8 | EXTI9 | EXTI11, GPIOC);
	exti_select_source(EXTI0 | EXTI15, GPIOA);

	exti_set_trigger(EXTI6 | EXTI8 | EXTI9 | EXTI11 | EXTI15,
			 EXTI_TRIGGER_FALLING);
	exti_set_trigger(EXTI0, EXTI_TRIGGER_RISING);
	exti_enable_request(EXTI6 | EXTI8 | EXTI9 | EXTI0 | EXTI11 | EXTI15);
}
void led_init(uint16_t gpio)
{
	rcc_periph_clock_enable(RCC_GPIOD);

	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLDOWN, gpio);
	gpio_set_output_options(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, gpio);
}
void led_init_all(void)
{
	led_init(GPIO15);
	led_init(GPIO12);
	led_init(GPIO13);
	led_init(GPIO14);
}
void systick_setup(void)
{
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_set_reload(168000); /* Interrupt every 1MS */
	systick_interrupt_enable();
	systick_counter_enable();
}
