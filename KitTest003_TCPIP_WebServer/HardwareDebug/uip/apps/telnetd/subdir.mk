################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../uip/apps/telnetd/shell.c \
../uip/apps/telnetd/telnetd.c 

COMPILER_OBJS += \
uip/apps/telnetd/shell.obj \
uip/apps/telnetd/telnetd.obj 

C_DEPS += \
uip/apps/telnetd/shell.d \
uip/apps/telnetd/telnetd.d 

# Each subdirectory must supply rules for building sources it contributes
uip/apps/telnetd/%.obj: ../uip/apps/telnetd/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -output=obj -obj_path="uip/apps/telnetd" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

