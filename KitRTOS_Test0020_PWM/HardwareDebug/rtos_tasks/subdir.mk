################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../rtos_tasks/maintasks.c 

COMPILER_OBJS += \
rtos_tasks/maintasks.obj 

C_DEPS += \
rtos_tasks/maintasks.d 

# Each subdirectory must supply rules for building sources it contributes
rtos_tasks/%.obj: ../rtos_tasks/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_config","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\pwm","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\RPDL","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\rtos_tasks","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_config","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\pwm","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\RPDL","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\rtos_tasks","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\src","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\include","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\MemMang","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM\FreeRTOS\portable\Renesas\RX600","C:\Users\fpolo\e2_studio\workspace\KitRTOS_Test0020_PWM","C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include" -lang=c99 -nomessage -output=obj -obj_path="rtos_tasks" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

