################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/RS232/RS232.c 

OBJS += \
./MyCode/RS232/RS232.o 

C_DEPS += \
./MyCode/RS232/RS232.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/RS232/%.o MyCode/RS232/%.su: ../MyCode/RS232/%.c MyCode/RS232/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/study/STM32G070RBT6/MyCode/inc" -I"D:/study/STM32G070RBT6/MyCode/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-RS232

clean-MyCode-2f-RS232:
	-$(RM) ./MyCode/RS232/RS232.d ./MyCode/RS232/RS232.o ./MyCode/RS232/RS232.su

.PHONY: clean-MyCode-2f-RS232

