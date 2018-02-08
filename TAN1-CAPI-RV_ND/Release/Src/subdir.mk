################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/OPP_RC.cpp \
../Src/ReadInputXmlFile.cpp \
../Src/WriteOutputXmlFile.cpp 

OBJS += \
./Src/OPP_RC.o \
./Src/ReadInputXmlFile.o \
./Src/WriteOutputXmlFile.o 

CPP_DEPS += \
./Src/OPP_RC.d \
./Src/ReadInputXmlFile.d \
./Src/WriteOutputXmlFile.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


