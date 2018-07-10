################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../r_bsp/board/rdkrx62n/dbsct.c \
../r_bsp/board/rdkrx62n/hwsetup.c \
../r_bsp/board/rdkrx62n/lowlvl.c \
../r_bsp/board/rdkrx62n/lowsrc.c \
../r_bsp/board/rdkrx62n/resetprg.c \
../r_bsp/board/rdkrx62n/sbrk.c \
../r_bsp/board/rdkrx62n/vecttbl.c 

COMPILER_OBJS += \
r_bsp/board/rdkrx62n/dbsct.obj \
r_bsp/board/rdkrx62n/hwsetup.obj \
r_bsp/board/rdkrx62n/lowlvl.obj \
r_bsp/board/rdkrx62n/lowsrc.obj \
r_bsp/board/rdkrx62n/resetprg.obj \
r_bsp/board/rdkrx62n/sbrk.obj \
r_bsp/board/rdkrx62n/vecttbl.obj 

C_DEPS += \
r_bsp/board/rdkrx62n/dbsct.d \
r_bsp/board/rdkrx62n/hwsetup.d \
r_bsp/board/rdkrx62n/lowlvl.d \
r_bsp/board/rdkrx62n/lowsrc.d \
r_bsp/board/rdkrx62n/resetprg.d \
r_bsp/board/rdkrx62n/sbrk.d \
r_bsp/board/rdkrx62n/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
r_bsp/board/rdkrx62n/%.obj: ../r_bsp/board/rdkrx62n/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest001","C:\Users\fpolo\e2_studio\workspace\KitTest001\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest001\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest001\generate" -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest001","C:\Users\fpolo\e2_studio\workspace\KitTest001\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest001\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest001\generate" -lang=c99 -nomessage -output=obj -obj_path="r_bsp/board/rdkrx62n" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

