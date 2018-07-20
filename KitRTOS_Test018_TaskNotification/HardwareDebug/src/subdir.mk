################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/ParTest.c \
../src/dbsct.c \
../src/hwsetup.c \
../src/intprg.c \
../src/led.c \
../src/main.c \
../src/maintasks.c \
../src/printf-stdarg.c \
../src/resetprg.c \
../src/sbrk.c \
../src/vecttbl.c 

COMPILER_OBJS += \
src/ParTest.obj \
src/dbsct.obj \
src/hwsetup.obj \
src/intprg.obj \
src/led.obj \
src/main.obj \
src/maintasks.obj \
src/printf-stdarg.obj \
src/resetprg.obj \
src/sbrk.obj \
src/vecttbl.obj 

C_DEPS += \
src/ParTest.d \
src/dbsct.d \
src/hwsetup.d \
src/intprg.d \
src/led.d \
src/main.d \
src/maintasks.d \
src/printf-stdarg.d \
src/resetprg.d \
src/sbrk.d \
src/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test018_TaskNotification","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -output=obj -obj_path="src" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

