HOME = /gpfs3/TanOnLine
GRIB_API_PATH = /gpfs3/TanOnLine/LIB/CAPI_lib/local/grib_api
JASPER_PATH = /gpfs3/TanOnLine/LIB/CAPI_lib/local/jasper
CC = g++
RM = rm -rf

LIBS = \
-lTan_aer -lTan_clm  -lgrib_api_f90  -lgrib_api  -ljasper -lZJPEG_NEW -ljpeg \
-lfreetype -lhdf5 -lhdf5_hl -lhdf5_fortran -lhdf5hl_fortran \
-lCWriteLog_TAN -ltanhdf -lconfig -ltimestring -lParamDll_TAN

LIBDIR = \
-L$(GRIB_API_PATH)/lib \
-L$(JASPER_PATH)/lib \
-L$(HOME)/LIB/hdf5/lib \
-L$(HOME)/LIB 
 
INCLUDE = \
-I"../include" \
-I$(GRIB_API_PATH)/include \
-I$(JASPER_PATH)/include \
-I/gpfs3/TanOnLine/BIN/CAPI_XML/Tv2/aerosol/src \
-I/gpfs3/TanOnLine/BIN/CAPI_XML/Tv2/cloud/src \
-I$(HOME)/include \
-I$(HOME)/include/hdf5/include 

CPP_SRCS = \
../src/CAPI-RV-Main.cpp \
../src/AuxProcess.cpp \
../src/MCMGJpeg.cpp \
../src/Processor.cpp \
../src/ReadInputXmlFile.cpp \
../src/WriteOutputXmlFile.cpp

OBJS = \
./src/CAPI-RV-Main.o \
./src/AuxProcess.o \
./src/MCMGJpeg.o \
./src/Processor.o \
./src/ReadInputXmlFile.o \
./src/WriteOutputXmlFile.o

# All Target
all: TAN1-CAPI-RV
TAN1-CAPI-RV: $(OBJS) 
	$(CC) $(INCLUDE) $(LIBDIR) -o"TAN1-CAPI-RV" $(OBJS) $(LIBS) $(LIBS_SO)
src/%.o: ../src/%.cpp
	$(CC) $(INCLUDE) -O0 -g3 -c -o"$@" "$<"
# Other Targets
.PHONY : clean 
clean :
	-$(RM) $(OBJS) TAN1-CAPI-RV
