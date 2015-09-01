################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../common.c \
../entity.c \
../game.c \
../input.c \
../main.c \
../render.c \
../resources.c \
../sound.c 

OBJS += \
./common.o \
./entity.o \
./game.o \
./input.o \
./main.o \
./render.o \
./resources.o \
./sound.o 

C_DEPS += \
./common.d \
./entity.d \
./game.d \
./input.d \
./main.d \
./render.d \
./resources.d \
./sound.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


