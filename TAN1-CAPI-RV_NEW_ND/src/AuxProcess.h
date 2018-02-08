#ifndef AUXPROCESS_H_
#define AUXPROCESS_H_
#define PointNumQKM 1600     //250m波段通道数
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
//#include "ParamDll.h"
#include "StructDefine.h"
#include "RetrievalStruct.h"
#include "hdf5.h"
#include "hdf5_hl.h"
#include "TanFilename.h"
#include "H5public.h"
#include "JPEG.h"
#include "CWriteLog_TAN.h"
#include "ParamDll_TAN.h"


using namespace std;
using namespace FY3JPEG;  //ZJPEG所在的名字空间

class AuxProcess
{
	public:	
		//ActualFrames
		int FrameNumQKM;
		int FrameNum1KM;
		int FrameNumAnc;
		RetrievalInputParam *pInputParam;
		RetrievalOutputParam *pOutputParam;
		int frame_index;//input from daqisuo
		int sounding_index;//input from daqisuo
		int RetrievalPonitNum;//input from daqisuo
		//L1 结构体变量
		L1B_CAPIX_HDF l1b_CAPI;
		L1B_CAPIX_NONPOLARIZATION l1b_CAPI_CLM;
		L1B_CAPIX_POLARIZATION l1b_CAPI_CLM2;
		//L2 结构体变量
		L2_CAPIX_CLM_HDF  l2_CML;
		L2_CAPIX_AER_HDF  l2_AER;
         
        //L2 写入全局属性
        L2_CLM_GlobalAttr  L2_AER_Glob;	

		time_t time_start, time_end;
		int* WarnCount;
		int  WarnCode;
		char WarnDescp[512];
		//各文件名
		char l1b_CAPIFileName[256];
		char l2_CMLFileName[256];
		char l2_AERFileName[256];
	public:
		AuxProcess();
		virtual ~AuxProcess();		
		void init(RetrievalInputParam* inputXML, RetrievalOutputParam* outputXML,int* Warningcount);
		//申请变量内存
		int ApplyMemory(L2_CAPIX_AER_HDF &L2_aer_Memory,L2_CAPIX_CLM_HDF &L2_clm_Memory,L2_CLM_GlobalAttr &L2_GlobalAttrH);	
		
		//读L2 HDF数据
		//updated by liwei @20170209 把读取L2数据中分开为单独读取云检测和气溶胶的两个函数
		int read_CAPI_CLM_L2_FILE(char *input_clm,L2_CAPIX_CLM_HDF &L2_clm_HDF);
		int read_CAPI_AER_L2_FILE(char *input_aer,L2_CAPIX_AER_HDF &L2_aer_HDF);
		
		//出浏览图
		int write_ClmJpeg(char *infilename_clm_jpg,L2_CAPIX_CLM_HDF &L2_clm_HDFT,L2_CLM_GlobalAttr &L2_CLM_GLOB);
		int write_AerJpeg(char *infilename_aer_jpg,L2_CAPIX_AER_HDF &L2_aer_HDF6,L2_CAPIX_CLM_HDF &L2_clm_HDFT,L2_CLM_GlobalAttr &L2_CLM_GLOB);
		int write_SciJpeg(char *input_aer_jpg,char *input_clm_jpg,L2_CAPIX_AER_HDF &L2_aer_HDF1,L2_CAPIX_CLM_HDF &L2_clm_HDF1);

		int read_CAPI_L1B_FILE();
		int Write_CLM_L2_HDF();
		
		//写入全局属性
		int ApplyMemory_CLM_Clob(L2_CLM_GlobalAttr &L2_CLM_GLOB1);
		int Read_CLM_L1_HDF(char *input_CLM_L1b_DF,L2_CLM_GlobalAttr &L2_CLM_Glob1);
		int Read_CLM_CFG(struct TDPInputxml *inputParam1,TanFileName *tanfilename2,L2_CLM_GlobalAttr &L2_CLM_GLOB9,L2_CAPIX_CLM_HDF &L2_clm_HDF1);
		int Write_CLM_L2_HDF(char *input_CLM1,L2_CLM_GlobalAttr &L2_CLM_GLOB1);
		
		int Read_AER_L1_HDF(char *input_AER_L1b_DF,L2_CLM_GlobalAttr &L2_AER_GLOB22);
		int Read_AER_CFG(struct TDPInputxml *inputParam1,TanFileName *tanfilename2,L2_CLM_GlobalAttr &L2_CLM_GLOB9,L2_CAPIX_AER_HDF &L2_aer_HDF1);
		int Write_AER_L2_HDF(char *input_aer22,L2_CLM_GlobalAttr &L2_CLM_GLOB22);
		
		//读AP模块输出 
		int WrReportFile(int errorcode,int Warningcount);  // 若错误码为0则输出报告内容，否则输出错误码
		void WrWarningMessage(int code, char* descp, int* Warningcount);
		//释放内存函数
		void FreeMemory(L2_CAPIX_AER_HDF &L2_capi_aer_hdf,L2_CAPIX_CLM_HDF &L2_capi_clm_hdf,L2_CLM_GlobalAttr &L2_CLM_GLOB11);
		void TimeConvert ( time_t timeCode, char *timeString );		
	};
	#endif /*AUXPROCESS_H_*/
//#define MAX_PATH_LENGTH			256

