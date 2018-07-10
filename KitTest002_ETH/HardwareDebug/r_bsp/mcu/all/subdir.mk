################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../r_bsp/mcu/all/r_bsp_common.c 

COMPILER_OBJS += \
r_bsp/mcu/all/r_bsp_common.obj 

C_DEPS += \
r_bsp/mcu/all/r_bsp_common.d 

# Each subdirectory must supply rules for building sources it contributes
r_bsp/mcu/all/%.obj: ../r_bsp/mcu/all/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\driver","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\src","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\driver","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\src","C:\Users\fpolo\e2_studio\workspace\KitTest002_ETH\generate" -lang=c99 -nomessage -output=obj -obj_path="r_bsp/mcu/all" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

