################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./DSP2802x_CodeStartBranch.obj" \
"./DSP2802x_CpuTimers.obj" \
"./DSP2802x_DefaultIsr.obj" \
"./DSP2802x_EPwm.obj" \
"./DSP2802x_GlobalVariableDefs.obj" \
"./DSP2802x_PieCtrl.obj" \
"./DSP2802x_PieVect.obj" \
"./DSP2802x_SysCtrl.obj" \
"./DSP2802x_usDelay.obj" \
"./Example_2802xEPwmDeadBand.obj" \
"C:/Users/zzy_c/DSP_P/v125/DSP2802x_common/cmd/28027_RAM_lnk.cmd" \
"C:/Users/zzy_c/DSP_P/v125/DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.cmd" \
$(GEN_CMDS__FLAG) \
-lrts2800_ml.lib \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Example_2802xEPwmDeadBand.out \

EXE_OUTPUTS__QUOTED += \
"Example_2802xEPwmDeadBand.out" \

BIN_OUTPUTS += \
Example_2802xEPwmDeadBand.hex \

BIN_OUTPUTS__QUOTED += \
"Example_2802xEPwmDeadBand.hex" \


# All Target
all: Example_2802xEPwmDeadBand.out

# Tool invocations
Example_2802xEPwmDeadBand.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C2000 Linker'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/cl2000" -v28 -ml -mt --define=_DEBUG --define=LARGE_MODEL -g --diag_warning=225 --issue_remarks --quiet --asm_directory="C:/Users/zzy_c/DSP_P/v125/DSP2802x_examples_ccsv4/epwm_deadband/Debug" -z -m"Example_2802xEPwmDeadBand.map" --stack_size=0x380 --warn_sections -i"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/lib" -i"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/include" -i"C:/Users/zzy_c/DSP_P/v125/DSP2802x_examples_ccsv4/epwm_deadband" -i"C:/Users/zzy_c/DSP_P/v125/DSP2802x_headers/include" --reread_libs --xml_link_info="Example_2802xEPwmDeadBand_linkInfo.xml" --entry_point=code_start --rom_model -o "Example_2802xEPwmDeadBand.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Example_2802xEPwmDeadBand.hex: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: C2000 Hex Utility'
	"D:/ti/ccsv8/tools/compiler/ti-cgt-c2000_18.1.2.LTS/bin/hex2000"  -o "Example_2802xEPwmDeadBand.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "DSP2802x_CodeStartBranch.obj" "DSP2802x_CpuTimers.obj" "DSP2802x_DefaultIsr.obj" "DSP2802x_EPwm.obj" "DSP2802x_GlobalVariableDefs.obj" "DSP2802x_PieCtrl.obj" "DSP2802x_PieVect.obj" "DSP2802x_SysCtrl.obj" "DSP2802x_usDelay.obj" "Example_2802xEPwmDeadBand.obj" 
	-$(RM) "DSP2802x_CpuTimers.d" "DSP2802x_DefaultIsr.d" "DSP2802x_EPwm.d" "DSP2802x_GlobalVariableDefs.d" "DSP2802x_PieCtrl.d" "DSP2802x_PieVect.d" "DSP2802x_SysCtrl.d" "Example_2802xEPwmDeadBand.d" 
	-$(RM) "DSP2802x_CodeStartBranch.d" "DSP2802x_usDelay.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

