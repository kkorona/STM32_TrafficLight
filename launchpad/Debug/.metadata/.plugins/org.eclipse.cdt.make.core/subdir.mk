################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.make.core/specs.cpp 

C_SRCS += \
../.metadata/.plugins/org.eclipse.cdt.make.core/specs.c 

OBJS += \
./.metadata/.plugins/org.eclipse.cdt.make.core/specs.o 


# Each subdirectory must supply rules for building sources it contributes
.metadata/.plugins/org.eclipse.cdt.make.core/%.o: ../.metadata/.plugins/org.eclipse.cdt.make.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\CoreSupport" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\DeviceSupport" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\DeviceSupport\Startup" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\STM32F10x_StdPeriph_Driver_v3.5\src" -O0 --cpu=cortex-m3 -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

.metadata/.plugins/org.eclipse.cdt.make.core/%.o: ../.metadata/.plugins/org.eclipse.cdt.make.core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM C Compiler 5'
	armcc -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\CoreSupport" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\DeviceSupport" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\CMSIS\DeviceSupport\Startup" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\STM32F10x_StdPeriph_Driver_v3.5\inc" -I"C:\Users\Team5\Desktop\term\launchpad\Libraries\STM32F10x_StdPeriph_Driver_v3.5\src" -O0 --cpu=cortex-m3 -g -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


