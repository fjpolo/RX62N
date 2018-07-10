################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/uip/unix/clock-arch.c 

COMPILER_OBJS += \
src/uip/unix/clock-arch.obj 

C_DEPS += \
src/uip/unix/clock-arch.d 

# Each subdirectory must supply rules for building sources it contributes
src/uip/unix/%.obj: ../src/uip/unix/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -output=obj -obj_path="src/uip/unix" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

