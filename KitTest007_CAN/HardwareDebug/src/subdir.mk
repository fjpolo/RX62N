################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/KitTest007_CAN.c 

COMPILER_OBJS += \
src/KitTest007_CAN.obj 

C_DEPS += \
src/KitTest007_CAN.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\RPDL","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\RPDL","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\generate" -lang=c99 -nomessage -output=obj -obj_path="src" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

