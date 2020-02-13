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

#ifndef PM_ISR
#define PM_ISR

#include <libopencm3/stm32/exti.h>

#include "../include/main.h"

void exti0_isr(void);
void exti9_5_isr(void);
void exti15_10_isr(void);
void sys_tick_handler(void);

#endif //PM_ISR