################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Embedded_Command_Processor/parser_src/process.c \
../Embedded_Command_Processor/parser_src/ring_buffer.c 

OBJS += \
./Embedded_Command_Processor/parser_src/process.o \
./Embedded_Command_Processor/parser_src/ring_buffer.o 

C_DEPS += \
./Embedded_Command_Processor/parser_src/process.d \
./Embedded_Command_Processor/parser_src/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Embedded_Command_Processor/parser_src/%.o Embedded_Command_Processor/parser_src/%.su Embedded_Command_Processor/parser_src/%.cyclo: ../Embedded_Command_Processor/parser_src/%.c Embedded_Command_Processor/parser_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F756ZGTx -DNUCLEO_F756ZG -DSTM32F7 -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Embedded_Command_Processor-2f-parser_src

clean-Embedded_Command_Processor-2f-parser_src:
	-$(RM) ./Embedded_Command_Processor/parser_src/process.cyclo ./Embedded_Command_Processor/parser_src/process.d ./Embedded_Command_Processor/parser_src/process.o ./Embedded_Command_Processor/parser_src/process.su ./Embedded_Command_Processor/parser_src/ring_buffer.cyclo ./Embedded_Command_Processor/parser_src/ring_buffer.d ./Embedded_Command_Processor/parser_src/ring_buffer.o ./Embedded_Command_Processor/parser_src/ring_buffer.su

.PHONY: clean-Embedded_Command_Processor-2f-parser_src

