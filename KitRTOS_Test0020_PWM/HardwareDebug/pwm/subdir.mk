################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../pwm/pwm.c 

COMPILER_OBJS += \
pwm/pwm.obj 

C_DEPS += \
pwm/pwm.d 

# Each subdirectory must supply rules for building sources it contributes
pwm/%.obj: ../pwm/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_config","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\pwm","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\RPDL","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\rtos_tasks","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_config","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\pwm","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\RPDL","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\rtos_tasks","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -output=obj -obj_path="pwm" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

