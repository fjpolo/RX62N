################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../generate/dbsct.c \
../generate/hwsetup.c \
../generate/intprg.c \
../generate/resetprg.c \
../generate/sbrk.c \
../generate/vecttbl.c 

COMPILER_OBJS += \
generate/dbsct.obj \
generate/hwsetup.obj \
generate/intprg.obj \
generate/resetprg.obj \
generate/sbrk.obj \
generate/vecttbl.obj 

C_DEPS += \
generate/dbsct.d \
generate/hwsetup.d \
generate/intprg.d \
generate/resetprg.d \
generate/sbrk.d \
generate/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
generate/%.obj: ../generate/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest004_TCPIP\generate" -lang=c -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest004_TCPIP\generate" -lang=c -nomessage -output=obj -obj_path="generate" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

