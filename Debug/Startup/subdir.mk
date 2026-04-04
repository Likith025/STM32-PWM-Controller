################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f756zgtx.s 

OBJS += \
./Startup/startup_stm32f756zgtx.o 

S_DEPS += \
./Startup/startup_stm32f756zgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m7 -g3 -DDEBUG -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32f756zgtx.d ./Startup/startup_stm32f756zgtx.o

.PHONY: clean-Startup

