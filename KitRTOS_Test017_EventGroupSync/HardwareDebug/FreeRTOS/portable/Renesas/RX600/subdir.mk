################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../FreeRTOS/portable/Renesas/RX600/port.c 

SRC_SRCS += \
../FreeRTOS/portable/Renesas/RX600/port_asm.src 

SRC_DEPS += \
FreeRTOS/portable/Renesas/RX600/port_asm.d 

ASSEMBLER_OBJS += \
FreeRTOS/portable/Renesas/RX600/port_asm.obj 

COMPILER_OBJS += \
FreeRTOS/portable/Renesas/RX600/port.obj 

C_DEPS += \
FreeRTOS/portable/Renesas/RX600/port.d 

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/portable/Renesas/RX600/%.obj: ../FreeRTOS/portable/Renesas/RX600/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test017_EventGroupSync","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c -nomessage -output=obj -obj_path="FreeRTOS/portable/Renesas/RX600" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

FreeRTOS/portable/Renesas/RX600/%.obj: ../FreeRTOS/portable/Renesas/RX600/%.src 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Assembler'
	asrx -MM -MP -MF="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -debug -nologo  "$<"
	asrx -isa=rxv1 -fpu -output="$(@:%.d=%.obj)" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

