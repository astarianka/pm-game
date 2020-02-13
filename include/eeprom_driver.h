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

#ifndef EEPROM_DRIVER_H
#define EEPROM_DRIVER_H

#include <stdint.h>

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>

/**
 * @brief Init I2C peripheral to use I2C EEPROM
 * @param i2c I2C peripheral
 * @param gpio_port GPIO port of I2C pins
 * @param rcc_i2c RCC of I2C
 * @param rcc_gpio_port RCC of GPIO
 * @param gpio1 GPIO pin of I2C interface (SDA | SCL)
 * @param gpio2 GPIO pin of I2C interface (SDA | SCL)
 * @return None
 */
void eeprom_init(uint32_t i2c, uint32_t gpio_port,
		 enum rcc_periph_clken rcc_i2c,
		 enum rcc_periph_clken rcc_gpio_port, uint16_t gpio1,
		 uint16_t gpio2);
/**
 * @brief Read data from I2C EEPROM
 * @param i2c I2C peripheral
 * @param slave_addr Address of I2C peripheral, defined by I2C manufacturer
 *	  (mentioned in I2C datasheet)
 * @param mem_addr Address of memory in I2C (EEPROM)
 * @param data Pointer to buffer in which data will be saved to
 * @param size Size of data in bytes
 * @return None
 */
void eeprom_read(uint32_t i2c, uint8_t slave_addr, uint32_t mem_addr,
		 uint8_t *data_buff, uint32_t size);
/**
 * @brief Write data to I2C EEPROM
 * @param i2c I2C peripheral
 * @param slave_addr Address of I2C peripheral, defined by I2C manufacturer
 *	  (mentioned in I2C datasheet)
 * @param mem_addr Address of memory in I2C (EEPROM)
 * @param data Pointer to data buffer to be saved in EEPROM
 * @param size Size of data in bytes
 * @return None
 */
void eeprom_write(uint32_t i2c, uint8_t slave_addr, uint32_t mem_addr,
		  uint8_t *data, uint32_t size);

#endif // EEPROM_DRIVER_H