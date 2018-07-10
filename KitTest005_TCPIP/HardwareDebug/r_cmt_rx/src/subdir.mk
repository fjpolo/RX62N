################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../r_cmt_rx/src/r_cmt_rx.c 

COMPILER_OBJS += \
r_cmt_rx/src/r_cmt_rx.obj 

C_DEPS += \
r_cmt_rx/src/r_cmt_rx.d 

# Each subdirectory must supply rules for building sources it contributes
r_cmt_rx/src/%.obj: ../r_cmt_rx/src/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\driver","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\src","C:\MinGW\include" -define=__RX=1,__GXX_EXPERIMENTAL_CXX0X__ -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\driver","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\src","C:\MinGW\include" -define=__RX=1,__GXX_EXPERIMENTAL_CXX0X__ -lang=c99 -nomessage -output=obj -obj_path="r_cmt_rx/src" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

