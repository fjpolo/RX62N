################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/uip/uip/main.c \
../src/uip/uip/psock.c \
../src/uip/uip/timer.c \
../src/uip/uip/uip.c \
../src/uip/uip/uip_arp.c 

COMPILER_OBJS += \
src/uip/uip/main.obj \
src/uip/uip/psock.obj \
src/uip/uip/timer.obj \
src/uip/uip/uip.obj \
src/uip/uip/uip_arp.obj 

C_DEPS += \
src/uip/uip/main.d \
src/uip/uip/psock.d \
src/uip/uip/timer.d \
src/uip/uip/uip.d \
src/uip/uip/uip_arp.d 

# Each subdirectory must supply rules for building sources it contributes
src/uip/uip/%.obj: ../src/uip/uip/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -output=obj -obj_path="src/uip/uip" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

