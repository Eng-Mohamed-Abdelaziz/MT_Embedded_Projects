################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ADC.c \
../DC_Motor.c \
../FanController.c \
../GPIO.c \
../LCD.c \
../LM35.c \
../PWM.c 

OBJS += \
./ADC.o \
./DC_Motor.o \
./FanController.o \
./GPIO.o \
./LCD.o \
./LM35.o \
./PWM.o 

C_DEPS += \
./ADC.d \
./DC_Motor.d \
./FanController.d \
./GPIO.d \
./LCD.d \
./LM35.d \
./PWM.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega16 -DF_CPU=1000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


