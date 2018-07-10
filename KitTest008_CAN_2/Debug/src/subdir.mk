################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/api_demo.c \
../src/dbsct.c \
../src/hwsetup.c \
../src/intprg.c \
../src/lcd.c \
../src/r_can_api.c \
../src/resetprg.c \
../src/sbrk.c \
../src/switches.c \
../src/vecttbl.c 

COMPILER_OBJS += \
src/api_demo.obj \
src/dbsct.obj \
src/hwsetup.obj \
src/intprg.obj \
src/lcd.obj \
src/r_can_api.obj \
src/resetprg.obj \
src/sbrk.obj \
src/switches.obj \
src/vecttbl.obj 

C_DEPS += \
src/api_demo.d \
src/dbsct.d \
src/hwsetup.d \
src/intprg.d \
src/lcd.d \
src/r_can_api.d \
src/resetprg.d \
src/sbrk.d \
src/switches.d \
src/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -cpu=rx600 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest008_CAN_2","C:\Users\fpolo\e2_studio\workspace\KitTest008_CAN_2\src" -define=NOT_DEBUG=1 -lang=c99 -nomessage -change_message=information -debug -listfile="$(@:%.obj=%.lst)" -show=source -optimize=0 -speed -nologo  "$<"
	ccrx -cpu=rx600 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest008_CAN_2","C:\Users\fpolo\e2_studio\workspace\KitTest008_CAN_2\src" -define=NOT_DEBUG=1 -lang=c99 -nomessage -change_message=information -output=obj -obj_path="src" -debug -listfile="$(@:%.obj=%.lst)" -show=source -optimize=0 -speed -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

