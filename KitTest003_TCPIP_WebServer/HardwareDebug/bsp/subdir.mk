################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../bsp/dbsct.c \
../bsp/hwsetup.c \
../bsp/intprg.c \
../bsp/lcd.c \
../bsp/resetprg.c \
../bsp/sbrk.c \
../bsp/vecttbl.c 

COMPILER_OBJS += \
bsp/dbsct.obj \
bsp/hwsetup.obj \
bsp/intprg.obj \
bsp/lcd.obj \
bsp/resetprg.obj \
bsp/sbrk.obj \
bsp/vecttbl.obj 

C_DEPS += \
bsp/dbsct.d \
bsp/hwsetup.d \
bsp/intprg.d \
bsp/lcd.d \
bsp/resetprg.d \
bsp/sbrk.d \
bsp/vecttbl.d 

# Each subdirectory must supply rules for building sources it contributes
bsp/%.obj: ../bsp/%.c 
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	ccrx -MM -MP -output=dep="$(@:%.obj=%.d)"  -MT="$(@:%.d=%.obj)"  -MT="$(@:%.obj=%.d)" -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -debug -optimize=0 -nologo  "$<"
	ccrx -isa=rxv1 -fpu -include="C:\PROGRA~2\RENESA~1\CS_~1\CC\CC-RX\V208~1.00\include","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\dhcpc","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\hello-world","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\resolv","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\smtp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\telnetd","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webclient","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\apps\webserver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\lib","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip\unix","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\bsp","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\driver","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\uip","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\user-app","C:\Users\fpolo\e2_studio\workspace\KitTest003_TCPIP_WebServer\generate" -lang=c99 -nomessage -output=obj -obj_path="bsp" -debug -optimize=0 -nologo "$<"
	@echo 'Finished Scanning and building: $<'
	@echo.

