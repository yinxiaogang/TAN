//StructDefine.h
#include <string.h>
//定义结构变量
using namespace std;
#if !defined(_STRUCT_DEFINE_)
#define _STRUCT_DEFINE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define FRAME_LEN 9828
#define L1BSOUNDING_NUM 1600
#define L1BSOUNDING_NUM1 5
#define CO2_COLUMN 500
#define O2A_COLUMN 1242
#define FLARE_MODE "GL"    //耀斑模式标识“GL”

#ifndef BYTE
#define BYTE unsigned char
#endif

//test
typedef struct
{
	char *SatelliteName;
	char *SensorName;
	char *FileName;
	char *AncillaryDataDescriptors;
	char *InputPointer;
	char *DataLevel;
	char orbitnumber[8];
	int StartOrbitNumber[1];
	int StopOrbitNumber[1];
	int ActualFrames[3];
	int ExpectedFrames[2];
	char *L2FullPhysicsAlgorithmDescriptor;
	char *SoftwareVersion;
	char *SoftwareRevisionDate;
	char *ObservingBeginningDate;
	char *ObservingBeginningTime;
	char *ObservingEndingDate;
	char *ObservingEndingTime;
	char *DataCreatingDate;
	char *DataCreatingTime;
	int NumberDataLevel[1];
	float LeftTopLat[1];
	float LeftTopLon[1];
	float RightTopLat[1];
	float RightTopLon[1];
	float LeftBottomLat[1];
	float LeftBottomLon[1];
	float RightBottomLat[1];
	float RightBottomLon[1];
	char *Resolution;
	char *UnitResolution;
	int DataLines[1];
	int DataPixels[1];
	char *AdditionalAnotation;
	char *HDFVersionId;
	

	char VersionofSoftware[8];
	char DateofSoftware[16];
	
}L2_CLM_GlobalAttr;

//L1b CAPI
typedef struct
{
	float *Ref_038_QKM;
	float *Ref_087_QKM;
}L1B_CAPIX_NONPOLARIZATION;

typedef struct
{
	float *Ref_067_I_QKM;
	float *Ref_067_Q_QKM;
	float *Ref_067_U_QKM;
}L1B_CAPIX_POLARIZATION;

typedef struct
{
	int *frame_DarkCurrent_QKM;
	int *frame_id_QKM;
	float *frame_time_second_J2000;
	unsigned int *frame_qual_flag_QKM;
	float *coef_Cal;
	char  *OpertionMode;
}L1B_CAPIX_ANCILLARY;

typedef struct
{
	L1B_CAPIX_NONPOLARIZATION NonPolarization;
	L1B_CAPIX_POLARIZATION    Polarization;
	L1B_CAPIX_ANCILLARY       Ancillary;
}L1B_CAPIX_HDF;
//end l1b CAPI

//l2 CLM
typedef struct
{
	int *Cloud_Mask;
	int *Confidence_Flag;
	int *Quality_Flag;
}L2_CAPIX_CLM_DATA;

typedef struct
{
	float *Latitude;
	float *Longitude;
}L2_CAPIX_CLM_GEO;

typedef struct {
	L2_CAPIX_CLM_DATA Data;
	L2_CAPIX_CLM_GEO Geo;
}L2_CAPIX_CLM_HDF;


//l2 AEROLOL
typedef struct
{
	float *Aerosol_Optical_Depth;
	float *Angstrom_Exponent;
	float *Fine_Mode_Fraction;
	float *Surface_Reflectance_670;
	float *Surface_Reflectance_870;
	float *Surface_Reflectance_1640;	
	int *Quality_Flag;
}L2_CAPIX_AER_DATA;

typedef struct
{
	float *Latitude;
	float *Longitude;
}L2_CAPIX_AER_GEO;

typedef struct {
	L2_CAPIX_AER_DATA Data;
	L2_CAPIX_AER_GEO Geo;
}L2_CAPIX_AER_HDF;


#endif //_STRUCT_DEFINE_
