################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include r_bsp/board/rskrx62n/subdir.mk
-include r_sys_time_rx/src/subdir.mk
-include src/tcp_blocking_sample/subdir.mk
-include r_bsp/mcu/all/subdir.mk
-include r_bsp/mcu/rx62n/subdir.mk
-include r_cmt_rx/src/subdir.mk
-include driver/subdir.mk

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
KitTest005_TCPIP.abs

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
KitTest005_TCPIP.lib

CONVERTOR_OUTPUT_OUTPUTS += \
KitTest005_TCPIP.mot \
KitTest005_TCPIP.x

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(CONVERTOR_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

KitTest005_TCPIP.mot: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Invoking: Converter'
	@echo 'Building target:'
	rlink "KitTest005_TCPIP.abs" -form=stype -output="KitTest005_TCPIP.mot" -byte_count=10 -nomessage
	@echo 'Finished building target:'
	@echo.


KitTest005_TCPIP.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)   \
C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib\T4_Library_rxv1_ether_little.lib
	@echo 'Invoking: Linker'
	rlink -library="C:\Users\fpolo\e2_studio\workspace\KitTest005_TCPIP\r_t4_rx\lib\T4_Library_rxv1_ether_little.lib" -noprelink -subcommand="LinkerSubCommand.tmp"  -output="KitTest005_TCPIP.abs" -form=absolute -nomessage -vect=_undefined_interrupt_source_isr -list=KitTest005_TCPIP.map -nooptimize -rom=D=R,D_1=R_1,D_2=R_2
	@echo 'Finished building:'
	@echo.


KitTest005_TCPIP.lib: 
	@echo 'Invoking: Library Generator'
	lbgrx -isa=rxv1 -fpu -lang=c99 -head=runtime,stdio,stdlib,string,new -output="KitTest005_TCPIP.lib"
	@echo 'Finished building:'
	@echo.

KitTest005_TCPIP.x: KitTest005_TCPIP.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe KitTest005_TCPIP.abs KitTest005_TCPIP.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS)
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.src src/*.jmp src/*.lst src/*.prn 
	-rm -rf r_bsp/board/rskrx62n/*.obj r_bsp/board/rskrx62n/*.d r_bsp/board/rskrx62n/*.asm r_bsp/board/rskrx62n/*.p r_bsp/board/rskrx62n/*.src r_bsp/board/rskrx62n/*.jmp r_bsp/board/rskrx62n/*.lst r_bsp/board/rskrx62n/*.prn 
	-rm -rf r_sys_time_rx/src/*.obj r_sys_time_rx/src/*.d r_sys_time_rx/src/*.asm r_sys_time_rx/src/*.p r_sys_time_rx/src/*.src r_sys_time_rx/src/*.jmp r_sys_time_rx/src/*.lst r_sys_time_rx/src/*.prn 
	-rm -rf src/tcp_blocking_sample/*.obj src/tcp_blocking_sample/*.d src/tcp_blocking_sample/*.asm src/tcp_blocking_sample/*.p src/tcp_blocking_sample/*.src src/tcp_blocking_sample/*.jmp src/tcp_blocking_sample/*.lst src/tcp_blocking_sample/*.prn 
	-rm -rf r_bsp/mcu/all/*.obj r_bsp/mcu/all/*.d r_bsp/mcu/all/*.asm r_bsp/mcu/all/*.p r_bsp/mcu/all/*.src r_bsp/mcu/all/*.jmp r_bsp/mcu/all/*.lst r_bsp/mcu/all/*.prn 
	-rm -rf r_bsp/mcu/rx62n/*.obj r_bsp/mcu/rx62n/*.d r_bsp/mcu/rx62n/*.asm r_bsp/mcu/rx62n/*.p r_bsp/mcu/rx62n/*.src r_bsp/mcu/rx62n/*.jmp r_bsp/mcu/rx62n/*.lst r_bsp/mcu/rx62n/*.prn 
	-rm -rf r_cmt_rx/src/*.obj r_cmt_rx/src/*.d r_cmt_rx/src/*.asm r_cmt_rx/src/*.p r_cmt_rx/src/*.src r_cmt_rx/src/*.jmp r_cmt_rx/src/*.lst r_cmt_rx/src/*.prn 
	-rm -rf driver/*.obj driver/*.d driver/*.asm driver/*.p driver/*.src driver/*.jmp driver/*.lst driver/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets