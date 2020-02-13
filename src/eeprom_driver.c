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
 * @brief Minimalistic driver for I2C EEPROM
 *
 */

#include "../include/eeprom_driver.h"

void eeprom_write(uint32_t i2c, uint8_t slave_addr, uint32_t mem_addr,
		  uint8_t *data, uint32_t size)
{
	uint32_t i;
	uint8_t buff[size + 1];
	/* Add memory address as first byte of data to be sent */
	buff[0] = mem_addr;

	for (i = 1; i <= size; i++)
		buff[i] = data[i - 1];

	i2c_transfer7(i2c, slave_addr, buff, size + 1, NULL, 0);
}

void eeprom_read(uint32_t i2c, uint8_t slave_addr, uint32_t mem_addr,
		 uint8_t *data_buff, uint32_t size)
{
	uint8_t buff[1] = { mem_addr };
	i2c_transfer7(i2c, slave_addr, buff, 1, data_buff, size);
}

void eeprom_init(uint32_t i2c, uint32_t gpio_port,
		 enum rcc_periph_clken rcc_i2c,
		 enum rcc_periph_clken rcc_gpio_port,
		 uint16_t gpio1, uint16_t gpio2)
{
	rcc_periph_clock_enable(rcc_i2c);
	rcc_periph_clock_enable(rcc_gpio_port);

	gpio_mode_setup(gpio_port, GPIO_MODE_AF, GPIO_PUPD_PULLUP,
			gpio1 | gpio2);
	gpio_set_output_options(gpio_port, GPIO_OTYPE_OD, GPIO_OSPEED_100MHZ,
				gpio1 | gpio2);
	gpio_set_af(gpio_port, GPIO_AF4, gpio1 | gpio2);

	i2c_reset(i2c);
	i2c_peripheral_disable(i2c);
	i2c_set_standard_mode(i2c);
	i2c_set_speed(i2c, i2c_speed_sm_100k, 8);
	i2c_enable_ack(i2c);
	i2c_set_dutycycle(i2c, I2C_CCR_DUTY_DIV2);
	i2c_peripheral_enable(i2c);
}