################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/tcp_nonblocking_cancel_sample/config_tcpudp.c \
../src/tcp_nonblocking_cancel_sample/echo_srv.c 

COMPILER_OBJS += \
src/tcp_nonblocking_cancel_sample/config_tcpudp.obj \
src/tcp_nonblocking_cancel_sample/echo_srv.obj 

C_DEPS += \
src/tcp_nonblocking_cancel_sample/config_tcpudp.d \
src/tcp_nonblocking_cancel_sample/echo_srv.d 

# Each subdirectory must supply rules for building sources it contributes
src/tcp_nonblocking_cancel_sample/%.obj: ../src/tcp_nonblocking_cancel_sample/%.c src/tcp_nonblocking_cancel_sample/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\driver","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\src","C:\MinGW\include" -define=__RX=1,__GXX_EXPERIMENTAL_CXX0X__ -lang=c99 -nomessage -debug -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_bsp","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_cmt_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_config","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_sys_time_rx\src","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\driver","C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\src","C:\MinGW\include" -define=__RX=1,__GXX_EXPERIMENTAL_CXX0X__ -lang=c99 -nomessage -output=obj -obj_path="src/tcp_nonblocking_cancel_sample" -debug -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

