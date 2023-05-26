################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/ENCODER/ENCODER.c 

OBJS += \
./MyCode/ENCODER/ENCODER.o 

C_DEPS += \
./MyCode/ENCODER/ENCODER.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/ENCODER/%.o MyCode/ENCODER/%.su: ../MyCode/ENCODER/%.c MyCode/ENCODER/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/study/STM32G070RBT6/MyCode/inc" -I"D:/study/STM32G070RBT6/MyCode/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-ENCODER

clean-MyCode-2f-ENCODER:
	-$(RM) ./MyCode/ENCODER/ENCODER.d ./MyCode/ENCODER/ENCODER.o ./MyCode/ENCODER/ENCODER.su

.PHONY: clean-MyCode-2f-ENCODER

