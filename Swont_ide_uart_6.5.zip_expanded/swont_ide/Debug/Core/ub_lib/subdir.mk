################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/ub_lib/stm32_ub_vga_screen.c 

OBJS += \
./Core/ub_lib/stm32_ub_vga_screen.o 

C_DEPS += \
./Core/ub_lib/stm32_ub_vga_screen.d 


# Each subdirectory must supply rules for building sources it contributes
Core/ub_lib/stm32_ub_vga_screen.o: ../Core/ub_lib/stm32_ub_vga_screen.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I"E:/Jaar 3/Software ontwikkeling/Software-ontwikkeling/Swont_ide_uart_6.5.zip_expanded/swont_ide/Core/ub_lib" -I../Drivers/CMSIS/Include -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Core/ub_lib/stm32_ub_vga_screen.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

