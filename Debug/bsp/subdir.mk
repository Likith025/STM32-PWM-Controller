################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stm32f7xx_driver/bsp/peripheral_config.c 

OBJS += \
./bsp/peripheral_config.o 

C_DEPS += \
./bsp/peripheral_config.d 


# Each subdirectory must supply rules for building sources it contributes
bsp/peripheral_config.o: D:/embedded/Command_line_parser/stm32f7xx_driver/bsp/peripheral_config.c bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F756ZGTx -DNUCLEO_F756ZG -DSTM32F7 -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-bsp

clean-bsp:
	-$(RM) ./bsp/peripheral_config.cyclo ./bsp/peripheral_config.d ./bsp/peripheral_config.o ./bsp/peripheral_config.su

.PHONY: clean-bsp

