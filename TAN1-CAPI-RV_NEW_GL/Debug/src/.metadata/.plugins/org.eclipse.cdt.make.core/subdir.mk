################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/.metadata/.plugins/org.eclipse.cdt.make.core/specs.cpp 

C_SRCS += \
../Src/.metadata/.plugins/org.eclipse.cdt.make.core/specs.c 

OBJS += \
./Src/.metadata/.plugins/org.eclipse.cdt.make.core/specs.o 

CPP_DEPS += \
./Src/.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 

C_DEPS += \
./Src/.metadata/.plugins/org.eclipse.cdt.make.core/specs.d 


# Each subdirectory must supply rules for building sources it contributes
Src/.metadata/.plugins/org.eclipse.cdt.make.core/%.o: ../Src/.metadata/.plugins/org.eclipse.cdt.make.core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Src/.metadata/.plugins/org.eclipse.cdt.make.core/%.o: ../Src/.metadata/.plugins/org.eclipse.cdt.make.core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/common/jpeg/include -I/home/wms/libf3c/ZJPEG_NEW/Debug -I/home/wyg/workspace/workspace/MCMGJPG -I/home/common/PGS/include -I/home/common/PGS/include/tanhdf -I/home/mjp/workspace/RC_include -I/home/common/xml/include/libxml2/libxml -I/home/common/hdf5/include -I/home/mjp/workspace/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


