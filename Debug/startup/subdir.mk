################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f407xx.s 

OBJS += \
./startup/startup_stm32f407xx.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/CMSIS/core" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/CMSIS/device" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/HAL_Driver/Inc/Legacy" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/HAL_Driver/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/DFU/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Class/Template/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Middlewares/ST/STM32_USB_Device_Library/Core/Inc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ampire480272" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ampire640480" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/Common" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/cs43l22" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/exc7200" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ft6x06" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ili9325" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ili9341" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/l3gd20" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lis302dl" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lis3dsh" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ls016b8uy" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/lsm303dlhc" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/mfxstm32l152" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q128a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q256a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/n25q512a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/otm8009a" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ov2640" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/s25fl512s" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/s5k5cag" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/st7735" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/st7789h2" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/stmpe1600" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/stmpe811" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/ts3510" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/Components/wm8994" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities" -I"C:/Users/Csenge/Desktop/MARLAB/MARCOM2/MARCOM/Utilities/STM32F4-Discovery" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


