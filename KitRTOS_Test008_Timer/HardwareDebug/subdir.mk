################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../ParTest.c \
../dbsct.c \
../hwsetup.c \
../intprg.c \
../led.c \
../main.c \
../printf-stdarg.c \
../resetprg.c \
../sbrk.c \
../vecttbl.c 

COMPILER_OBJS += \
ParTest.obj \
dbsct.obj \
hwsetup.obj \
intprg.obj \
led.obj \
main.obj \
printf-stdarg.obj \
resetprg.obj \
sbrk.obj \
vecttbl.obj 

C_DEPS += \
ParTest.d \
dbsct.d \
hwsetup.d \
intprg.d \
led.d \
main.d \
printf-stdarg.d \
resetprg.d \
sbrk.d \
vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test008_Timer","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -output=obj -obj_path="" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

