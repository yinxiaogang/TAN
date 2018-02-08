#ifndef COMMONDEF_H_
#define COMMONDEF_H_


//define time type: UTC or BEIJING
#define TIME_TYPE_UTC      	0
#define TIME_TYPE_BEIJING  	1
#define ADD   				0

#define FAIL_APPLY_MEMORY 1
#define FAIL_READ_HDF 2

//define file name
#define HSCO2_L1B_FILE_NAME			"FY3C_MERSI_GBAL_L1_20150105_0020_GEO1K_MS.HDF"
#define RC_CONFIG_FILE_NAME 		"RC_Parameter.cfg"//RC_ParamFile.XCONF
#define DIR_CHAR "/"

//char APImgDataFileName[256]; //得到整个路径和除（-bandID.raw）外的文件名字符串,若为高光谱则为整个文件名
//char CCD1RCcoeffFileName[256];
//char CCD2RCcoeffFileName[256];

#endif /*COMMONDEF_H_*/
