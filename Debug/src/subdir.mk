################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bsp_accelerometer.c \
../src/bsp_buttonsswitches.c \
../src/bsp_can.c \
../src/bsp_glcd.c \
../src/bsp_globalfunctions.c \
../src/bsp_i2c.c \
../src/bsp_spi.c \
../src/bsp_thermometer.c \
../src/bsp_uart.c \
../src/graphics.c \
../src/localsensorprocessing.c \
../src/log.c \
../src/main.c \
../src/mainloop.c \
../src/segment_display.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/system_stm32f4xx.c 

OBJS += \
./src/bsp_accelerometer.o \
./src/bsp_buttonsswitches.o \
./src/bsp_can.o \
./src/bsp_glcd.o \
./src/bsp_globalfunctions.o \
./src/bsp_i2c.o \
./src/bsp_spi.o \
./src/bsp_thermometer.o \
./src/bsp_uart.o \
./src/graphics.o \
./src/localsensorprocessing.o \
./src/log.o \
./src/main.o \
./src/mainloop.o \
./src/segment_display.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/system_stm32f4xx.o 

C_DEPS += \
./src/bsp_accelerometer.d \
./src/bsp_buttonsswitches.d \
./src/bsp_can.d \
./src/bsp_glcd.d \
./src/bsp_globalfunctions.d \
./src/bsp_i2c.d \
./src/bsp_spi.d \
./src/bsp_thermometer.d \
./src/bsp_uart.d \
./src/graphics.d \
./src/localsensorprocessing.d \
./src/log.d \
./src/main.d \
./src/mainloop.d \
./src/segment_display.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F407G_DISC1 -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/CMSIS/core" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/CMSIS/device" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/HAL_Driver/Inc/Legacy" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/HAL_Driver/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/Template/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ampire480272" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ampire640480" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/Common" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/cs43l22" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/exc7200" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ft6x06" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ili9325" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ili9341" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/l3gd20" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lis302dl" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lis3dsh" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ls016b8uy" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lsm303dlhc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/mfxstm32l152" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q128a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q256a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q512a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/otm8009a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ov2640" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/s25fl512s" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/s5k5cag" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/st7735" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/st7789h2" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/stmpe1600" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/stmpe811" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ts3510" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/wm8994" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/STM32F4-Discovery" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


