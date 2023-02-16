################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../gpio_access.c 

C_DEPS += \
./gpio_access.d 

OBJS += \
./gpio_access.o 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler 7.5.0 [arm-linux-gnueabihf]'
	arm-linux-gnueabihf-gcc.exe -I"C:\intelFPGA_lite\22.1std\embedded\ip\altera\hps\altera_hps" -I"C:\intelFPGA_lite\22.1std\embedded\ip\altera\hps\altera_hps\hwlib\include" -I"C:\intelFPGA_lite\22.1std\embedded\ip\altera\hps\" -I"C:\intelFPGA_lite\22.1std\embedded\ip\altera\hps\altera_hps\hwlib\include\soc_cv_av" -O0 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean--2e-

clean--2e-:
	-$(RM) ./gpio_access.d ./gpio_access.o

.PHONY: clean--2e-

