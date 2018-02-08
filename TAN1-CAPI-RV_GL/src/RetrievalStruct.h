#ifndef _INVERSION_STRUCT_H_
#define _INVERSION_STRUCT_H_

struct RetrievalInputParam {

	char cSatID[16];				//卫星标识
	char cSensorType[8];
	char cProductLevel[8]; 			//产品级别
	long lProductID; 				//产品号
	char cLocalWorkDir[256]; 		//本地工作目录
	char cWorkConfigDir[256]; 		//配置文件目录
};

struct RetrievalOutputParam {
	int iReturnCode;
	char cReturnDescription[256];
	char cStartTimeString[30];
	char cEndTimeString[30];

};

#endif /*RCIEPSTRUCT_H_*/
