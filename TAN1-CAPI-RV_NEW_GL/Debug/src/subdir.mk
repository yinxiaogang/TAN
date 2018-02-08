################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Src/AuxProcess.cpp \
../Src/CAPI-DPPS-QA.cpp \
../Src/MCMGJpeg.cpp \
../Src/Processor.cpp \
../Src/ReadInputXmlFile.cpp \
../Src/WriteOutputXmlFile.cpp 

OBJS += \
./Src/AuxProcess.o \
./Src/CAPI-DPPS-QA.o \
./Src/MCMGJpeg.o \
./Src/Processor.o \
./Src/ReadInputXmlFile.o \
./Src/WriteOutputXmlFile.o 

CPP_DEPS += \
./Src/AuxProcess.d \
./Src/CAPI-DPPS-QA.d \
./Src/MCMGJpeg.d \
./Src/Processor.d \
./Src/ReadInputXmlFile.d \
./Src/WriteOutputXmlFile.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/common/jpeg/include -I/home/wms/libf3c/ZJPEG_NEW/Debug -I/home/wyg/workspace/workspace/MCMGJPG -I/home/common/PGS/include -I/home/common/PGS/include/tanhdf -I/home/mjp/workspace/RC_include -I/home/common/xml/include/libxml2/libxml -I/home/common/hdf5/include -I/home/mjp/workspace/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


