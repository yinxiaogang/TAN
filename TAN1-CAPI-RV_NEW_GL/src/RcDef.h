#ifndef COMMONDEF_H_
#define COMMONDEF_H_

//define sensor ID
#define SENSOR_ID_CCD		0
#define SENSOR_ID_CCD1		1
#define SENSOR_ID_CCD2		2
#define SENSOR_ID_IRS   3
#define SENSOR_ID_IRMSS 3
#define SENSOR_ID_HSI   4
#define SENSOR_ID_P5	   		5
#define SENSOR_ID_P10	    6
#define SENSOR_ID_CBERS_IRS	 7
#define SENSOR_ID_MUX	    8
#define SENSOR_ID_WFI	    9

#define RC_MODEL_CAL   0
#define RC_MODEL_STAT  1
#define RC_MODEL_COMP  2
#define RC_MODEL_SINGLE_IMG  3
#define NONE   4

#define DEADPIXEL_MODEL_NONE   0
#define DEADPIXEL_MODEL_NN     1
#define DEADPIXEL_MODEL_LINE   2

#define PIXELNUMBER  6000
#define PIXELBYTE		1

//define time type: UTC or BEIJING
#define TIME_TYPE_UTC      	0
#define TIME_TYPE_BEIJING  	1	
#define ADD   				0

#define  STEP   5
#define  BLOCKS 51

//define file name
#define FILE_NAME_CFG        "Filename.cfg"
#define FILE_NAME_CFG_1      "Filename.cfg"
#define HSCO2_ARP			"TAN1_HSCO2_ARP.HDF"
#define CAPI_ARP 			"TAN1_CAPIX_ARP.HDF"

#define CAL_Result 			"CAL_Result"
#define Instrument_Day_DF 	"TAN1_TEL_"
#define Gps_Day_DF 			"TAN1_GPS_"

#define DIR_CHAR "/"

//by wyg0808
#define  READ_INPUTXML_ERR     11

//char APImgDataFileName[256]; //得到整个路径和除（-bandID.raw）外的文件名字符串,若为高光谱则为整个文件名
//char CCD1RCcoeffFileName[256];
//char CCD2RCcoeffFileName[256];

#endif /*COMMONDEF_H_*/
