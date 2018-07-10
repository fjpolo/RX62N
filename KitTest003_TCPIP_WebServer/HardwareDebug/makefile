################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include uip/apps/dhcpc/subdir.mk
-include uip/apps/webserver/subdir.mk
-include uip/doc/subdir.mk
-include uip/uip/subdir.mk
-include uip/apps/resolv/subdir.mk
-include uip/apps/webclient/subdir.mk
-include driver/subdir.mk
-include uip/lib/subdir.mk
-include uip/unix/subdir.mk
-include uip/apps/telnetd/subdir.mk
-include uip/apps/hello-world/subdir.mk
-include uip/apps/smtp/subdir.mk
-include user-app/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif 
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif 
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif 
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif 
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif 
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif 
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif 
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif 
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif 
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif 
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif 
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif 
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif 
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif 
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif


-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables
LINKER_OUTPUT_OUTPUTS += \
KitTest003_TCPIP_WebServer.abs \
KitTest003_TCPIP_WebServer.x

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
KitTest003_TCPIP_WebServer.lib

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

KitTest003_TCPIP_WebServer.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rlink -noprelink -subcommand="LinkerSubCommand.tmp"  -output="KitTest003_TCPIP_WebServer.abs" -form=absolute -nomessage -list=KitTest003_TCPIP_WebServer.map -nooptimize -rom=D=R,D_1=R_1,D_2=R_2 -nologo
	@echo 'Finished building target:'
	@echo.


KitTest003_TCPIP_WebServer.lib: 
	@echo 'Invoking: Library Generator'
	lbgrx -isa=rxv1 -fpu -lang=c -head=runtime,stdio,stdlib,string,new -output="KitTest003_TCPIP_WebServer.lib" -nologo
	@echo 'Finished building:'
	@echo.

KitTest003_TCPIP_WebServer.x: KitTest003_TCPIP_WebServer.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe KitTest003_TCPIP_WebServer.abs KitTest003_TCPIP_WebServer.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	-rm -rf uip/apps/dhcpc/*.obj uip/apps/dhcpc/*.d uip/apps/dhcpc/*.asm uip/apps/dhcpc/*.p uip/apps/dhcpc/*.src uip/apps/dhcpc/*.jmp uip/apps/dhcpc/*.lst uip/apps/dhcpc/*.prn 
	-rm -rf uip/apps/webserver/*.obj uip/apps/webserver/*.d uip/apps/webserver/*.asm uip/apps/webserver/*.p uip/apps/webserver/*.src uip/apps/webserver/*.jmp uip/apps/webserver/*.lst uip/apps/webserver/*.prn 
	-rm -rf uip/doc/*.obj uip/doc/*.d uip/doc/*.asm uip/doc/*.p uip/doc/*.src uip/doc/*.jmp uip/doc/*.lst uip/doc/*.prn 
	-rm -rf uip/uip/*.obj uip/uip/*.d uip/uip/*.asm uip/uip/*.p uip/uip/*.src uip/uip/*.jmp uip/uip/*.lst uip/uip/*.prn 
	-rm -rf uip/apps/resolv/*.obj uip/apps/resolv/*.d uip/apps/resolv/*.asm uip/apps/resolv/*.p uip/apps/resolv/*.src uip/apps/resolv/*.jmp uip/apps/resolv/*.lst uip/apps/resolv/*.prn 
	-rm -rf uip/apps/webclient/*.obj uip/apps/webclient/*.d uip/apps/webclient/*.asm uip/apps/webclient/*.p uip/apps/webclient/*.src uip/apps/webclient/*.jmp uip/apps/webclient/*.lst uip/apps/webclient/*.prn 
	-rm -rf driver/*.obj driver/*.d driver/*.asm driver/*.p driver/*.src driver/*.jmp driver/*.lst driver/*.prn 
	-rm -rf uip/lib/*.obj uip/lib/*.d uip/lib/*.asm uip/lib/*.p uip/lib/*.src uip/lib/*.jmp uip/lib/*.lst uip/lib/*.prn 
	-rm -rf uip/unix/*.obj uip/unix/*.d uip/unix/*.asm uip/unix/*.p uip/unix/*.src uip/unix/*.jmp uip/unix/*.lst uip/unix/*.prn 
	-rm -rf uip/apps/telnetd/*.obj uip/apps/telnetd/*.d uip/apps/telnetd/*.asm uip/apps/telnetd/*.p uip/apps/telnetd/*.src uip/apps/telnetd/*.jmp uip/apps/telnetd/*.lst uip/apps/telnetd/*.prn 
	-rm -rf uip/apps/hello-world/*.obj uip/apps/hello-world/*.d uip/apps/hello-world/*.asm uip/apps/hello-world/*.p uip/apps/hello-world/*.src uip/apps/hello-world/*.jmp uip/apps/hello-world/*.lst uip/apps/hello-world/*.prn 
	-rm -rf uip/apps/smtp/*.obj uip/apps/smtp/*.d uip/apps/smtp/*.asm uip/apps/smtp/*.p uip/apps/smtp/*.src uip/apps/smtp/*.jmp uip/apps/smtp/*.lst uip/apps/smtp/*.prn 
	-rm -rf user-app/*.obj user-app/*.d user-app/*.asm user-app/*.p user-app/*.src user-app/*.jmp user-app/*.lst user-app/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets