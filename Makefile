BUILD = build

firmware: $(BUILD)/firmware.elf $(BUILD)/firmware.bin
.PHONY: clean firmware

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

LDSCRIPT = stm32f078.ld
STARTUP = Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f078xx.s

DEFINE += \
	-DSTM32F078xx \
	-DUSE_HAL_DRIVER \
	-DMSC_MEDIA_PACKET=8192 \

INCLUDE += \
	-I Inc \
	-I Drivers/CMSIS/Include \
	-I Drivers/CMSIS/Device/ST/STM32F0xx/Include \
	-I Drivers/STM32F0xx_HAL_Driver/Inc/ \
	-I Middlewares/ST/STM32_USB_Device_Library/Core/Inc/ \
	-I Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc/ \
	-I Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc/ \

SRC += \
	Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/system_stm32f0xx.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pcd_ex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_adc_ex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c_ex.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
	Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi_ex.c \
	Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
	Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
	Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
	Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
	Src/usbd_desc.c \
	Src/usbd_storage_if.c \
	Src/sd_tim.c \
	Src/usbd_conf.c \
	Src/stm32f0xx_hal_msp.c \
	Src/sd_usbd_cdc_if.c \
	Src/sd_buffer.c \
	Src/sd_uart.c \
	Src/sd_gpio.c \
	Src/sd_spi.c \
	Src/main.c \
	Src/sd_i2c.c \
	Src/sd_led_pattern.c \
	Src/sd_interrupt.c \
	Src/sd_led.c \
	Src/sd_pwm.c \
	Src/usb_device.c \
	Src/sd_dac.c \
	Src/sd_platform.c \
	Src/sd_button.c \
	Src/sd_spi_bridge.c \
	Src/sd_adc.c \

CFLAGS += -Wall --std=gnu99 -Os -g3 -flto
CFLAGS += -fdata-sections -ffunction-sections
CFLAGS += -funsigned-char -funsigned-bitfields
CFLAGS += -mcpu=cortex-m0 -mthumb

LDFLAGS += -mcpu=cortex-m0 -mthumb -flto
LDFLAGS += -Wl,--gc-sections --specs=nano.specs

OBJS := $(addprefix $(BUILD)/, $(subst .c,.o, $(SRC)))
-include $(OBJS:.o=.d)

$(OBJS): $(BUILD)/%.o: %.c
	@mkdir -p $(shell dirname $@)
	$(Q)$(CC) $(CFLAGS) $(INCLUDE) $(DEFINE) -c $< -o $@ -MMD -MP -MF $(patsubst %.o,%.d,$@)

$(BUILD)/firmware.elf: $(OBJS) $(STARTUP)
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(STARTUP) $(OBJS) -Wl,-T$(LDSCRIPT) -o $(BUILD)/firmware.elf

$(BUILD)/firmware.bin: $(BUILD)/firmware.elf
	$(Q)$(OBJCOPY) -O binary -R .eeprom $(BUILD)/firmware.elf $(BUILD)/firmware.bin

clean:
	@-rm -rf $(BUILD)
