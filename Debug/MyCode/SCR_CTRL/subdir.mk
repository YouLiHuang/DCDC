################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MyCode/SCR_CTRL/scr_ctrl.c 

OBJS += \
./MyCode/SCR_CTRL/scr_ctrl.o 

C_DEPS += \
./MyCode/SCR_CTRL/scr_ctrl.d 


# Each subdirectory must supply rules for building sources it contributes
MyCode/SCR_CTRL/%.o MyCode/SCR_CTRL/%.su: ../MyCode/SCR_CTRL/%.c MyCode/SCR_CTRL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G070xx -c -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I"D:/study/STM32G070RBT6/MyCode/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MyCode-2f-SCR_CTRL

clean-MyCode-2f-SCR_CTRL:
	-$(RM) ./MyCode/SCR_CTRL/scr_ctrl.d ./MyCode/SCR_CTRL/scr_ctrl.o ./MyCode/SCR_CTRL/scr_ctrl.su

.PHONY: clean-MyCode-2f-SCR_CTRL

