################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp *.bls *.libelf *.rel *.lbk *.abs  *.obj *.p *.pp *.d *.asm 1st/*.obj 1st/*.d 1st/*.asm 1st/*.abs

# All of the sources participating in the build are defined here
-include sources.mk
-include FreeRTOS/subdir.mk
-include FreeRTOS/portable/Renesas/RX600/subdir.mk
-include FreeRTOS/portable/MemMang/subdir.mk
-include src/subdir.mk

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
KitRTOS_Test019_TaskNotification_pdFalse.abs \
KitRTOS_Test019_TaskNotification_pdFalse.x

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
KitRTOS_Test019_TaskNotification_pdFalse.lib

OBJS := \
$(COMPILER_OBJS) \
$(INDIVIDUAL_COMPILER_OBJS) \
$(ASSEMBLER_OBJS) \
$(CUSTOM_OBJS)

# All Target
all: $(LINKER_OUTPUT_OUTPUTS) 
	@echo 'Build complete.'

KitRTOS_Test019_TaskNotification_pdFalse.abs: $(OBJS)  $(ALL_ASMS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rlink -noprelink -subcommand="LinkerSubCommand.tmp"  -output="KitRTOS_Test019_TaskNotification_pdFalse.abs" -form=absolute -nomessage -list=KitRTOS_Test019_TaskNotification_pdFalse.map -nooptimize -rom=D=R,D_1=R_1,D_2=R_2 -nologo
	@echo 'Finished building target:'
	@echo.


KitRTOS_Test019_TaskNotification_pdFalse.lib: 
	@echo 'Invoking: Library Generator'
	lbgrx -isa=rxv1 -fpu -lang=c -head=runtime,stdio,stdlib,string,new -output="KitRTOS_Test019_TaskNotification_pdFalse.lib" -nologo
	@echo 'Finished building:'
	@echo.

KitRTOS_Test019_TaskNotification_pdFalse.x: KitRTOS_Test019_TaskNotification_pdFalse.abs
		-C:\Renesas\e2_studio\eclipse\/../\Utilities\\ccrx\renesas_cc_converter.exe KitRTOS_Test019_TaskNotification_pdFalse.abs KitRTOS_Test019_TaskNotification_pdFalse.x

# Other Targets
clean:
	-$(RM) $(LINKER_OUTPUT_OUTPUTS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	-rm -rf FreeRTOS/*.obj FreeRTOS/*.d FreeRTOS/*.asm FreeRTOS/*.p FreeRTOS/*.src FreeRTOS/*.jmp FreeRTOS/*.lst FreeRTOS/*.prn 
	-rm -rf FreeRTOS/portable/Renesas/RX600/*.obj FreeRTOS/portable/Renesas/RX600/*.d FreeRTOS/portable/Renesas/RX600/*.asm FreeRTOS/portable/Renesas/RX600/*.p FreeRTOS/portable/Renesas/RX600/*.src FreeRTOS/portable/Renesas/RX600/*.jmp FreeRTOS/portable/Renesas/RX600/*.lst FreeRTOS/portable/Renesas/RX600/*.prn 
	-rm -rf FreeRTOS/portable/MemMang/*.obj FreeRTOS/portable/MemMang/*.d FreeRTOS/portable/MemMang/*.asm FreeRTOS/portable/MemMang/*.p FreeRTOS/portable/MemMang/*.src FreeRTOS/portable/MemMang/*.jmp FreeRTOS/portable/MemMang/*.lst FreeRTOS/portable/MemMang/*.prn 
	-rm -rf src/*.obj src/*.d src/*.asm src/*.p src/*.src src/*.jmp src/*.lst src/*.prn 
	@echo
	@echo 'Clean complete.'

.PHONY: all clean 
-include ../makefile.targets