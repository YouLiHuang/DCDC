################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/PROTECT/protect.c \
../MyCode/PROTECT/time_delay_cycle.c 

OBJS += \
./MyCode/PROTECT/protect.o \
./MyCode/PROTECT/time_delay_cycle.o 

C_DEPS += \
./MyCode/PROTECT/protect.d \
./MyCode/PROTECT/time_delay_cycle.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/PROTECT/%.o MyCode/PROTECT/%.su: ../MyCode/PROTECT/%.c MyCode/PROTECT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/study/STM32G070RBT6/MyCode/inc" -I"D:/study/STM32G070RBT6/MyCode/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-PROTECT

clean-MyCode-2f-PROTECT:
	-$(RM) ./MyCode/PROTECT/protect.d ./MyCode/PROTECT/protect.o ./MyCode/PROTECT/protect.su ./MyCode/PROTECT/time_delay_cycle.d ./MyCode/PROTECT/time_delay_cycle.o ./MyCode/PROTECT/time_delay_cycle.su

.PHONY: clean-MyCode-2f-PROTECT

