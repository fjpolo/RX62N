################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../FreeRTOS/portable/MemMang/heap_4.c 

COMPILER_OBJS += \
FreeRTOS/portable/MemMang/heap_4.obj 

C_DEPS += \
FreeRTOS/portable/MemMang/heap_4.d 

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/MemMang/%.obj: ../FreeRTOS/portable/MemMang/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitTest016_RTOS_Blinky_IdleHook\FreeRTOS","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -output=obj -obj_path="FreeRTOS/portable/MemMang" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

