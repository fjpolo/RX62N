################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/bsp/dbsct.c \
../src/bsp/hwsetup.c \
../src/bsp/intprg.c \
../src/bsp/lcd.c \
../src/bsp/resetprg.c \
../src/bsp/sbrk.c \
../src/bsp/vecttbl.c 

COMPILER_OBJS += \
src/bsp/dbsct.obj \
src/bsp/hwsetup.obj \
src/bsp/intprg.obj \
src/bsp/lcd.obj \
src/bsp/resetprg.obj \
src/bsp/sbrk.obj \
src/bsp/vecttbl.obj 

C_DEPS += \
src/bsp/dbsct.d \
src/bsp/hwsetup.d \
src/bsp/intprg.d \
src/bsp/lcd.d \
src/bsp/resetprg.d \
src/bsp/sbrk.d \
src/bsp/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
src/bsp/%.obj: ../src/bsp/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\driver","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\uip","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\src\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest006_zeroCopy\generate" -lang=c99 -nomessage -output=obj -obj_path="src/bsp" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

