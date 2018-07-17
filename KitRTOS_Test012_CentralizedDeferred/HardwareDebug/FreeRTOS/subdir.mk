################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../FreeRTOS/croutine.c \
../FreeRTOS/event_groups.c \
../FreeRTOS/list.c \
../FreeRTOS/queue.c \
../FreeRTOS/stream_buffer.c \
../FreeRTOS/tasks.c \
../FreeRTOS/timers.c 

COMPILER_OBJS += \
FreeRTOS/croutine.obj \
FreeRTOS/event_groups.obj \
FreeRTOS/list.obj \
FreeRTOS/queue.obj \
FreeRTOS/stream_buffer.obj \
FreeRTOS/tasks.obj \
FreeRTOS/timers.obj 

C_DEPS += \
FreeRTOS/croutine.d \
FreeRTOS/event_groups.d \
FreeRTOS/list.d \
FreeRTOS/queue.d \
FreeRTOS/stream_buffer.d \
FreeRTOS/tasks.d \
FreeRTOS/timers.d 

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/%.obj: ../FreeRTOS/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test012_CentralizedDeferred","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -output=obj -obj_path="FreeRTOS" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

