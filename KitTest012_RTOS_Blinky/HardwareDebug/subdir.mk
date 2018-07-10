################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../ParTest.c \
../dbsct.c \
../hwsetup.c \
../idcode.c \
../interrupt_handlers.c \
../main.c \
../printf-stdarg.c \
../reset_program.c \
../vector_table.c 

COMPILER_OBJS += \
ParTest.obj \
dbsct.obj \
hwsetup.obj \
idcode.obj \
interrupt_handlers.obj \
main.obj \
printf-stdarg.obj \
reset_program.obj \
vector_table.obj 

C_DEPS += \
ParTest.d \
dbsct.d \
hwsetup.d \
idcode.d \
interrupt_handlers.d \
main.d \
printf-stdarg.d \
reset_program.d \
vector_table.d 

# Each subdirectory must supply rules for building sources it contributes
%.obj: ../%.c Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitTest012_RTOS_Blinky\FreeRTOS","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -output=obj -obj_path="" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

