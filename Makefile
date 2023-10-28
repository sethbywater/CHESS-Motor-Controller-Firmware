CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAGS=-D STM32G473xx \
	 -I STM32CubeG4/Drivers/CMSIS/Core/Include \
	 -I STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Include \
	 -I STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Inc \
	 -I src

BUILD=build

LINKER_FILE=linker.ld
LDFLAGS=-T $(LINKER_FILE)

PROGRAMMER=openocd
PROGRAMMER_FLAGS=-f interface/stlink.cfg -f target/stm32g4x.cfg

HAL_SOURCES := $(wildcard STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/*.c)
HAL_OBJS := $(patsubst STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/%.c, $(BUILD)/%.o, $(HAL_SOURCES))

SOURCES := $(wildcard *.c)
OBJS := $($(SOURCES):.c=.o)


all: main.elf

build:
	mkdir -p $(BUILD)

.PHONY: clean
clean:
	rm -rf $(BUILD)

main.elf: $(BUILD)/main.o $(BUILD)/startup.o $(BUILD)/system_stm32g4xx.o $(HAL_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(BUILD)/main.elf $^



# main.o: Source/main.c
# 	$(CC) $(CFLAGS) $(CPPFLAGS) Source/main.c -c

# startup.o: Source/startup.c
# 	$(CC) $(CFLAGS) $(CPPFLAGS) Source/startup.c -c

# system_stm32g4xx.o: STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c
# 	$(CC) $(CFLAGS) $(CPPFLAGS) STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/system_stm32g4xx.c -c

# stm32g4xx_hal.o: STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c
# 	$(CC) $(CFLAGS) $(CPPFLAGS) STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c -c

vpath %.c STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates \
	STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src \
	src

vpath %.h STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Include \
	STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Inc \
	src

vpath %.s STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/gcc

# vpath %.o obj

# the stm32 hardware abstraction library
# STM_HAL_OBJS = $(shell ls STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src | sed  's/\.c/\.o/g')
# STM_HAL_OBJS = $(patsubst STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/%.c, %.o, $(wildcard STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src/*.c))


# generic rules
$(BUILD)/%.o: %.c %.h build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

flash: main.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program main.elf verify reset exit"


# HAL_OBJS = stm32g4xx_hal.o \
# 	stm32g4xx_hal_adc.o \
# 	stm32g4xx_hal_adc_ex.o \
# 	stm32g4xx_hal_comp.o \
# 	stm32g4xx_hal_cordic.o \
# 	stm32g4xx_hal_cortex.o \
# 	stm32g4xx_hal_crc.o \
# 	stm32g4xx_hal_crc_ex.o \
# 	stm32g4xx_hal_cryp.o \
# 	stm32g4xx_hal_cryp_ex.o \
# 	stm32g4xx_hal_dac.o \
# 	stm32g4xx_hal_dac_ex.o \
# 	stm32g4xx_hal_dma.o \
# 	stm32g4xx_hal_dma_ex.o \
# 	stm32g4xx_hal_exti.o \
# 	stm32g4xx_hal_fdcan.o \
# 	stm32g4xx_hal_flash.o \
# 	stm32g4xx_hal_flash_ex.o \
# 	stm32g4xx_hal_flash_ramfunc.o \
# 	stm32g4xx_hal_fmac.o \
# 	stm32g4xx_hal_gpio.o \
# 	stm32g4xx_hal_hrtim.o \
# 	stm32g4xx_hal_i2c.o \
# 	stm32g4xx_hal_i2c_ex.o \
# 	stm32g4xx_hal_i2s.o \
# 	stm32g4xx_hal_irda.o \
# 	stm32g4xx_hal_iwdg.o \
# 	stm32g4xx_hal_lptim.o \
# 	stm32g4xx_hal_msp_template.o \
# 	stm32g4xx_hal_nand.o \
# 	stm32g4xx_hal_nor.o \
# 	stm32g4xx_hal_opamp.o \
# 	stm32g4xx_hal_opamp_ex.o \
# 	stm32g4xx_hal_pcd.o \
# 	stm32g4xx_hal_pcd_ex.o \
# 	stm32g4xx_hal_pwr.o \
# 	stm32g4xx_hal_pwr_ex.o \
# 	stm32g4xx_hal_qspi.o \
# 	stm32g4xx_hal_rcc.o \
# 	stm32g4xx_hal_rcc_ex.o \
# 	stm32g4xx_hal_rng.o \
# 	stm32g4xx_hal_rtc.o \
# 	stm32g4xx_hal_rtc_ex.o \
# 	stm32g4xx_hal_sai.o \
# 	stm32g4xx_hal_sai_ex.o \
# 	stm32g4xx_hal_smartcard.o \
# 	stm32g4xx_hal_smartcard_ex.o \
# 	stm32g4xx_hal_smbus.o \
# 	stm32g4xx_hal_smbus_ex.o \
# 	stm32g4xx_hal_spi.o \
# 	stm32g4xx_hal_spi_ex.o \
# 	stm32g4xx_hal_sram.o \
# 	stm32g4xx_hal_tim.o \
# 	stm32g4xx_hal_timebase_tim_template.o \
# 	stm32g4xx_hal_tim_ex.o \
# 	stm32g4xx_hal_uart.o \
# 	stm32g4xx_hal_uart_ex.o \
# 	stm32g4xx_hal_usart.o \
# 	stm32g4xx_hal_usart_ex.o \
# 	stm32g4xx_hal_wwdg.o \
# 	stm32g4xx_ll_adc.o \
# 	stm32g4xx_ll_comp.o \
# 	stm32g4xx_ll_cordic.o \
# 	stm32g4xx_ll_crc.o \
# 	stm32g4xx_ll_crs.o \
# 	stm32g4xx_ll_dac.o \
# 	stm32g4xx_ll_dma.o \
# 	stm32g4xx_ll_exti.o \
# 	stm32g4xx_ll_fmac.o \
# 	stm32g4xx_ll_fmc.o \
# 	stm32g4xx_ll_gpio.o \
# 	stm32g4xx_ll_hrtim.o \
# 	stm32g4xx_ll_i2c.o \
# 	stm32g4xx_ll_lptim.o \
# 	stm32g4xx_ll_lpuart.o \
# 	stm32g4xx_ll_opamp.o \
# 	stm32g4xx_ll_pwr.o \
# 	stm32g4xx_ll_rcc.o \
# 	stm32g4xx_ll_rng.o \
# 	stm32g4xx_ll_rtc.o \
# 	stm32g4xx_ll_spi.o \
# 	stm32g4xx_ll_tim.o \
# 	stm32g4xx_ll_ucpd.o \
# 	stm32g4xx_ll_usart.o \
# 	stm32g4xx_ll_usb.o \
# 	stm32g4xx_ll_utils.o