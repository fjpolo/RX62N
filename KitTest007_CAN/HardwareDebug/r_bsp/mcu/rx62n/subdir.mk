################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../r_bsp/mcu/rx62n/cpu.c \
../r_bsp/mcu/rx62n/locking.c \
../r_bsp/mcu/rx62n/mcu_clocks.c \
../r_bsp/mcu/rx62n/mcu_init.c \
../r_bsp/mcu/rx62n/mcu_interrupts.c \
../r_bsp/mcu/rx62n/mcu_locks.c 

COMPILER_OBJS += \
r_bsp/mcu/rx62n/cpu.obj \
r_bsp/mcu/rx62n/locking.obj \
r_bsp/mcu/rx62n/mcu_clocks.obj \
r_bsp/mcu/rx62n/mcu_init.obj \
r_bsp/mcu/rx62n/mcu_interrupts.obj \
r_bsp/mcu/rx62n/mcu_locks.obj 

C_DEPS += \
r_bsp/mcu/rx62n/cpu.d \
r_bsp/mcu/rx62n/locking.d \
r_bsp/mcu/rx62n/mcu_clocks.d \
r_bsp/mcu/rx62n/mcu_init.d \
r_bsp/mcu/rx62n/mcu_interrupts.d \
r_bsp/mcu/rx62n/mcu_locks.d 

# Each subdirectory must supply rules for building sources it contributes
r_bsp/mcu/rx62n/%.obj: ../r_bsp/mcu/rx62n/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\RPDL","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\RPDL","C:\Users\fpolo\e2_studio\workspace\KitTest007_CAN\generate" -lang=c99 -nomessage -output=obj -obj_path="r_bsp/mcu/rx62n" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

