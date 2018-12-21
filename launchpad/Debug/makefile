################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include Libraries/STM32F10x_StdPeriph_Driver_v3.5/src/subdir.mk
-include Libraries/CMSIS/DeviceSupport/Startup/subdir.mk
-include Libraries/CMSIS/DeviceSupport/subdir.mk
-include Libraries/CMSIS/CoreSupport/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: launchpad.axf

# Tool invocations
launchpad.axf: $(OBJS) C:\Users\Team03\Desktop\term\launchpad\stm32-test.sct $(USER_OBJS) $(LIBS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker 5'
	armlink --cpu=cortex-m3 --scatter="C:\Users\Team03\Desktop\term\launchpad\stm32-test.sct" --info=sizes -o "launchpad.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) $(EXECUTABLES) launchpad.axf 
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
