################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../uip/uip/main.c \
../uip/uip/psock.c \
../uip/uip/timer.c \
../uip/uip/uip-fw.c \
../uip/uip/uip-neighbor.c \
../uip/uip/uip-split.c \
../uip/uip/uip.c \
../uip/uip/uip_arp.c \
../uip/uip/uiplib.c 

COMPILER_OBJS += \
uip/uip/main.obj \
uip/uip/psock.obj \
uip/uip/timer.obj \
uip/uip/uip-fw.obj \
uip/uip/uip-neighbor.obj \
uip/uip/uip-split.obj \
uip/uip/uip.obj \
uip/uip/uip_arp.obj \
uip/uip/uiplib.obj 

C_DEPS += \
uip/uip/main.d \
uip/uip/psock.d \
uip/uip/timer.d \
uip/uip/uip-fw.d \
uip/uip/uip-neighbor.d \
uip/uip/uip-split.d \
uip/uip/uip.d \
uip/uip/uip_arp.d \
uip/uip/uiplib.d 

# Each subdirectory must supply rules for building sources it contributes
uip/uip/%.obj: ../uip/uip/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -output=obj -obj_path="uip/uip" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

