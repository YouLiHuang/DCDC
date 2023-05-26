################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/oled/spi_oled.c \
../MyCode/oled/time_oled.c 

OBJS += \
./MyCode/oled/spi_oled.o \
./MyCode/oled/time_oled.o 

C_DEPS += \
./MyCode/oled/spi_oled.d \
./MyCode/oled/time_oled.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/oled/%.o MyCode/oled/%.su: ../MyCode/oled/%.c MyCode/oled/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/study/STM32G070RBT6/MyCode/inc" -I"D:/study/STM32G070RBT6/MyCode/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-oled

clean-MyCode-2f-oled:
	-$(RM) ./MyCode/oled/spi_oled.d ./MyCode/oled/spi_oled.o ./MyCode/oled/spi_oled.su ./MyCode/oled/time_oled.d ./MyCode/oled/time_oled.o ./MyCode/oled/time_oled.su

.PHONY: clean-MyCode-2f-oled

