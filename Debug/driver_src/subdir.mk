################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/nvic.c \
../stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/stm32f7xx_gpio_driver.c \
../stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/stm32f7xx_uart_driver.c 

OBJS += \
./driver_src/nvic.o \
./driver_src/stm32f7xx_gpio_driver.o \
./driver_src/stm32f7xx_uart_driver.o 

C_DEPS += \
./driver_src/nvic.d \
./driver_src/stm32f7xx_gpio_driver.d \
./driver_src/stm32f7xx_uart_driver.d 


# Each subdirectory must supply rules for building sources it contributes
driver_src/nvic.o: D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/nvic.c driver_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F756ZGTx -DNUCLEO_F756ZG -DSTM32F7 -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
driver_src/stm32f7xx_gpio_driver.o: D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/stm32f7xx_gpio_driver.c driver_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F756ZGTx -DNUCLEO_F756ZG -DSTM32F7 -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
driver_src/stm32f7xx_uart_driver.o: D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src/stm32f7xx_uart_driver.c driver_src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F756ZGTx -DNUCLEO_F756ZG -DSTM32F7 -c -I"D:/embedded/Command_line_parser/stm32f7xx_driver/bsp" -I"D:/embedded/Command_line_parser/Src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_src" -I"D:/embedded/Command_line_parser/stm32f7xx_driver/stm32f7xx_driver/driver/driver_inc" -I"D:/embedded/Command_line_parser/Embedded_Command_Processor/parser_inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-driver_src

clean-driver_src:
	-$(RM) ./driver_src/nvic.cyclo ./driver_src/nvic.d ./driver_src/nvic.o ./driver_src/nvic.su ./driver_src/stm32f7xx_gpio_driver.cyclo ./driver_src/stm32f7xx_gpio_driver.d ./driver_src/stm32f7xx_gpio_driver.o ./driver_src/stm32f7xx_gpio_driver.su ./driver_src/stm32f7xx_uart_driver.cyclo ./driver_src/stm32f7xx_uart_driver.d ./driver_src/stm32f7xx_uart_driver.o ./driver_src/stm32f7xx_uart_driver.su

.PHONY: clean-driver_src

