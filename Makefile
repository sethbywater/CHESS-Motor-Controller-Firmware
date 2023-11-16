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

vpath %.c STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates \
	STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Src \
	src

vpath %.h STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Include \
	STM32CubeG4/Drivers/STM32G4xx_HAL_Driver/Inc \
	src

vpath %.s STM32CubeG4/Drivers/CMSIS/Device/ST/STM32G4xx/Source/Templates/gcc

vpath %.o obj

all: main.elf

build:
	mkdir -p $(BUILD)

main.elf: $(BUILD)/main.o $(BUILD)/startup.o $(BUILD)/system_stm32g4xx.o $(HAL_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) -o $(BUILD)/main.elf $^

# generic rules
$(BUILD)/%.o: %.c %.h build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(BUILD)/%.o: %.s build
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

flash: main.elf
	$(PROGRAMMER) $(PROGRAMMER_FLAGS) -c "program main.elf verify reset exit"
	
.PHONY: clean
clean:
	rm -rf $(BUILD)
