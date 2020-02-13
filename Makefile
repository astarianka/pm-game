
NAME = pm_game

CFILES = \
	lib/libopencm3-drivers/helper.c \
	lib/libopencm3-drivers/hd44780.c \
	src/main.c \
	src/pm.c \
	src/periph_init.c \
	src/pm_isr.c \
	src/eeprom_driver.c

include common.mk
