################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include src/uip/apps/dhcpc/subdir.mk
-include src/uip/apps/webserver/subdir.mk
-include src/uip/uip/subdir.mk
-include src/uip/unix/subdir.mk
-include src/driver/subdir.mk
-include src/user-app/subdir.mk

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
KitTest006_zeroCopy.abs \
KitTest006_zeroCopy.x

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
KitTest006_zeroCopy.lib

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

KitTest006_zeroCopy.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rlink -noprelink -subcommand="LinkerSubCommand.tmp"  -output="KitTest006_zeroCopy.abs" -form=absolute -nomessage -list=KitTest006_zeroCopy.map -nooptimize -rom=D=R,D_1=R_1,D_2=R_2 -nologo
	@echo 'Finished building target:'
	@echo.


KitTest006_zeroCopy.lib: 
	@echo 'Invoking: Library Generator'
	lbgrx -isa=rxv1 -fpu -lang=c -head=runtime,stdio,stdlib,string,new -output="KitTest006_zeroCopy.lib" -nologo
	@echo 'Finished building:'
	@echo.

KitTest006_zeroCopy.x: KitTest006_zeroCopy.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe KitTest006_zeroCopy.abs KitTest006_zeroCopy.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	-rm -rf src/uip/apps/dhcpc/*.obj src/uip/apps/dhcpc/*.d src/uip/apps/dhcpc/*.asm src/uip/apps/dhcpc/*.p src/uip/apps/dhcpc/*.src src/uip/apps/dhcpc/*.jmp src/uip/apps/dhcpc/*.lst src/uip/apps/dhcpc/*.prn 
	-rm -rf src/uip/apps/webserver/*.obj src/uip/apps/webserver/*.d src/uip/apps/webserver/*.asm src/uip/apps/webserver/*.p src/uip/apps/webserver/*.src src/uip/apps/webserver/*.jmp src/uip/apps/webserver/*.lst src/uip/apps/webserver/*.prn 
	-rm -rf src/uip/uip/*.obj src/uip/uip/*.d src/uip/uip/*.asm src/uip/uip/*.p src/uip/uip/*.src src/uip/uip/*.jmp src/uip/uip/*.lst src/uip/uip/*.prn 
	-rm -rf src/uip/unix/*.obj src/uip/unix/*.d src/uip/unix/*.asm src/uip/unix/*.p src/uip/unix/*.src src/uip/unix/*.jmp src/uip/unix/*.lst src/uip/unix/*.prn 
	-rm -rf src/driver/*.obj src/driver/*.d src/driver/*.asm src/driver/*.p src/driver/*.src src/driver/*.jmp src/driver/*.lst src/driver/*.prn 
	-rm -rf src/user-app/*.obj src/user-app/*.d src/user-app/*.asm src/user-app/*.p src/user-app/*.src src/user-app/*.jmp src/user-app/*.lst src/user-app/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets