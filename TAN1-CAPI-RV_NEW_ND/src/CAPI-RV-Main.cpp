/***********************************************************************************************************
 Copyright (c) 2008,航天503所
 All rights reserved.
 文件名：CAPI-RV-Main.cpp
 编写者：
 编写日期：2016年3月05日
 简要描述： CAPI反演算法处理模块， 
 当前版本：1.0.0
 **************************************************************************************************************/
#define VERSION_DESCRIPSION "version 1.0 \nSupport Data Type: \n TAN1:CAPI\n"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <dlfcn.h>
//#include <zhdf.h>
#include <time.h>
#include "RetrievalStruct.h"
#include "RetrievalStruct.h"
#include "ReadInputXmlFile.h"
#include "WriteOutputXmlFile.h"
#include "AuxProcess.h"
#include "Processor.h"
#include "StructDefine.h"
#include "CWriteLog_TAN.h"
#include "ParamDll_TAN.h"
#include "TanFilename.h"
#include "hdf5.h"
#include "hdf5_hl.h"
#include "zlib.h"
#include "RcDef.h"
#include "PublicErrorCode.h"

#define DIR_CHAR "/"


#include "HSCO2QAErrorCode.h"

using namespace FY3JPEG;  //ZJPEG所在的名字空间
void handler(int sig, siginfo_t *scp, void *addr);
 #ifdef __linux__
extern "C" int tan_clm_(
#else
extern "C" int tan_clm(
#endif
        char  IFL_NWP[256],
		char  NWP_DF[512],
		char  NWP_DFtemp[512],
		char CAPIL1B250HDF_DF[512],
		char CAPIL1B250GEOHDF_DF[512],
		char CAPIL1B1KMHDF_DF[512],
		char CAPIL1B1KMGEOHDF_DF[512],
		char CAPIConfigDIR[256],
		char  Filename_cfg[512],
		char  CAPIARP[512],	
		char WORKDIR[256],
		char LogFilename[512],
		char IFL_CAPI_L2_CLM_HDF[256],
		char CLML2HDF_DF_TEMP[512],
		char Mode[8],
		char CLMresolution[8],
		char CLMVersion[8], 
		int *iReturnCode, 
		char cReturnDescription[256],
		char cErrorDescription[512],
		int *OutputDataNum,
		char DATA[10][512]
		); 

#ifdef __linux__
extern "C" int tan_aer_(
#else
extern "C" int tan_aer(
#endif	
        char  IFL_NWP[256],
		char  NWP_DF[512],
		char  NWP_DFtemp[512],
		char CAPIL1B250HDF_DF[512],
		char CAPIL1B250GEOHDF_DF[512],
		char CAPIL1B1KMHDF_DF[512],
		char CAPIL1B1KMGEOHDF_DF[512],
		char CAPIConfigDIR[256],
		char  Filename_cfg[512],
		char  CAPIARP[512],	
		char WORKDIR[256],
		char LogFilename[512],
		char IFL_CAPI_L2_AER_HDF[256],
		char IFL_CAPI_L2_CLM_HDF[256],
		char AERL2HDF_DF_TEMP[512],
		char CLML2HDF_DF_TEMP[512],
		char Mode[8],
		char AERresolution[8],
		char CLMresolution[8],
		char AERVersion[8],
		char CLMVersion[8], 
		int *iReturnCode, 
		char cReturnDescription[256],
		char cErrorDescription[512],
		int *OutputDataNum,
		char DATA[10][512]
		);

 int main(int argc, char *argv[])
{	    
	    
		clock_t mainstart,mainfinish,clm_start,aer_start,clm_finish, aer_finish; //计时变量
		double maintotaltime,totaltime;           //记录该模块运行时间变量
		mainstart = clock();
		
		printf("\n");
	    printf("************************************************************\n");
		printf("\n");
		printf("            ************************************\n");
		printf("            *                                  *\n");
		printf("            *           CAPI module            *\n");
		printf("            *                                  *\n");
		printf("            ************************************\n");
		printf("\n");
		printf("****************TAN1-CAPIX-RV module begin!******************\n");
		
		char logname[512];
		CWriteLog PGSQAlog;	                  // PGSQAlog：写日志变量
		PGSQAlog.WrPieceMessage("*******TAN1-CAPIX-RV module begin*******\n", 0);        	
		struct sigaction act;
		act.sa_flags = 0;
		sigemptyset(&act.sa_mask);
		act.sa_sigaction = handler;
		sigaction(SIGUSR1,&act,(struct sigaction *)NULL);
		
		L2_CAPIX_AER_HDF   L2_aer_hdf;         //HDF结构体气溶胶变量
		L2_CAPIX_CLM_HDF   L2_clm_hdf;	       //HDF结构体云检测变量					
		TDPInputxml        inputParam;         //读取输入XML结构体变量
		TDPOutputxml       outputParam;        //输出XML结构体变量
		TanFileName        CAPI_TAN1;          //传给主体程序的结构体变量

		memset(&inputParam, 0, sizeof(struct TDPInputxml));
		memset(&outputParam, 0, sizeof(struct TDPOutputxml));
		
		outputParam.iReturnCode = 0;
		
		/****************** 对程序调度参数个数进行判断 *********************/
		if(argc < 3)
		{
			printf("Usage:OPP_RC <input.xml> <output.xml>\n");
			strcpy(outputParam.cReturnDescription, "The number of input Parameter is less than 3 ");
			
			PGSQAlog.WrPieceMessage("The number of input Parameter is less than 3!\n",1);
			return READ_FUNCTION_PARAM_ERR;			
		}
		char cInputXmlFileName[255];      //传递输入XML的变量
		char cOutputXmlFileName[255];     //传递输出XML的变量
		strcpy(cInputXmlFileName, argv[1]);
		strcpy(cOutputXmlFileName, argv[2]);		
		memset(&CAPI_TAN1, 0, sizeof(struct TanFileName));
        
		/********************** 读入文件路径及文件名XML ***************************/
		printf("Start analyse input xml content!\n");
		PGSQAlog.WrPieceMessage("Start to read inputxml file!\n",0);
		int returnCode;     //函数是否正确运行，判别变量，0：正确运行，非零：运行错误
		returnCode = ReadInputXmlFile(&inputParam, cInputXmlFileName, &outputParam,&CAPI_TAN1,&PGSQAlog);
		if( returnCode>0 )
		{
			//写反馈给PP的输出文件
			WriteOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);
			printf("ReadInputXmlFile function failed!\n");
			PGSQAlog.WrPieceMessage(" ReadInputXmlFile function failed!\n",1);
			PGSQAlog.WrOldMessage();
			return  returnCode;
		}		
		PGSQAlog.WrOldMessage();
		
			
		printf("Read inputxml file over!\n\n");
		PGSQAlog.WriteLogFile("Read inputxml file over!",0);
		
		
		/*****************如果L1B数据为耀斑模式，则不进行生产*****************/
		if(strcmp(CAPI_TAN1.Mode,FLARE_MODE) == 0)  
		{
			strcpy(outputParam.cReturnDescription,"L1B data Mode is GL, dose not generate the L2 products!");
			strcpy(outputParam.cErrorDescription,"L1B data Mode is GL, dose not generate the L2 products!");
			WriteOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);
			printf("L1B data Mode is GL, dose not generate the L2 products!\n\n");
			PGSQAlog.WrPieceMessage("L1B data Mode is GL, dose not generate the L2 products!\n",1);
			PGSQAlog.WrOldMessage(); 
			return  returnCode;
		}
		
		
		int Warningcount = 0;
		int ret;  //AuxProcess.h中函数是否正确运行，0：正确运行，非零：运行错误
				
		
		//为了适应和fortran接口，在CAPI_TAN1.CAPIConfigDIR后面全部补上空格
		for(int i=strlen(CAPI_TAN1.CAPIConfigDIR); i<256; i++)
		{
		    CAPI_TAN1.CAPIConfigDIR[i] = ' ';
		}
      
		/* for(int i=strlen(CAPI_TAN1.CAPIL1B1KMHDF_DF); i<512; i++)
		{
		    CAPI_TAN1.CAPIL1B1KMHDF_DF[i] = ' ';
		}
		
		for(int i=strlen(CAPI_TAN1.CAPIL1B1KMGEOHDF_DF); i<512; i++)
		{
		    CAPI_TAN1.CAPIL1B1KMGEOHDF_DF[i] = ' ';
		} */
		
		//printf("读入文件路径及文件名XML的HDF的轨道号\n");
		printf("CAPI_TAN1.CAPIL1B1KMHDF_DF = %s\n",CAPI_TAN1.CAPIL1B1KMHDF_DF);
		printf("CAPI_TAN1.CAPIL1B1KMGEOHDF_DF = %s\n",CAPI_TAN1.CAPIL1B1KMGEOHDF_DF);
		

		/*******************************************************
		*
		*                     云检测反演函数      
		*
		********************************************************/ 
		
		clm_start = clock();
		printf("\n\nStart to load clm inverse algorithm\n");
		PGSQAlog.WriteLogFile("Start to load cloud inverse algorithm!", 0);        		
		
		#ifdef __linux__
		 tan_clm_(
		#else
		 tan_clm(
		#endif
			  CAPI_TAN1.IFL_NWP,
			  CAPI_TAN1.NWP_DF,
			  CAPI_TAN1.NWP_DFtemp,
			  CAPI_TAN1.CAPIL1B250HDF_DF,
			  CAPI_TAN1.CAPIL1B250GEOHDF_DF,
			  CAPI_TAN1.CAPIL1B1KMHDF_DF,
			  CAPI_TAN1.CAPIL1B1KMGEOHDF_DF,
			  CAPI_TAN1.CAPIConfigDIR,
			  CAPI_TAN1.Filename_cfg,
			  CAPI_TAN1.CAPIARP,
			  CAPI_TAN1.WORKDIR,
			  CAPI_TAN1.LogFilename,
			  CAPI_TAN1.IFL_CAPI_L2_CLM_HDF,
			  CAPI_TAN1.CLML2HDF_DF_TEMP,
			  CAPI_TAN1.Mode,
			  CAPI_TAN1.CLMresolution,
			  CAPI_TAN1.CLMVersion,
			  &outputParam.iReturnCode,
			  outputParam.cReturnDescription,
			  outputParam.cErrorDescription,
			  &outputParam.OutputDataNum,
			  CAPI_TAN1.DATA
			  );
	  
        outputParam.cReturnDescription[255] = '\0';
		outputParam.cErrorDescription[511] = '\0';
		
	    if( outputParam.iReturnCode>0 )
	    {
			//写反馈给PP的输出文件			
			WriteOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);			
			printf("%s\n", outputParam.cReturnDescription);
			printf("Cloud inverse algorithm failed!\n");
			PGSQAlog.WriteLogFile("Cloud inverse algorithm failed!\n",1);			
			return  outputParam.iReturnCode;
		}
		
		printf(" Load cloud inverse algorithm success!\n");		
		PGSQAlog.WriteLogFile("Load cloud inverse algorithm  success!", 0);
		
		//统计云检测反演的运行时间
		clm_finish=clock();
		totaltime=(double)(clm_finish-clm_start)/CLOCKS_PER_SEC;
		printf("This CLM CAPI costs %f seconds\n",totaltime);
		 
		 
		/************************************************
		*                 读取云检测全局属性
		*************************************************/
	 	AuxProcess  RandW;  //RandW：AuxProcess输入输出函数类变量，函数申明在AuxProcess.cpp中
		L2_CLM_GlobalAttr  L2_CLM_GLOB;    // L2_CLM_GLOB：全局属性结构体变量 
		
		//为 L2_CLM_GLOB 申请内存
		printf("\nStart to apply memory for L2_CLM_GLOB!\n");
		PGSQAlog.WriteLogFile("Start to apply memory for L2_CLM_GLOB!", 0);
		ret = RandW.ApplyMemory_CLM_Clob(L2_CLM_GLOB);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Apply Memory fail!");
			sprintf(outputParam.cErrorDescription, "Apply Memory fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
        printf(" Apply memory finish!\n");		
		PGSQAlog.WriteLogFile(" Apply memory finish!", 0); 
		
		
        //读取 CLM 1B 数据全局属性		
		printf("\nStart to read L1 global attribute of CLM!\n");
		PGSQAlog.WriteLogFile("Start to read L1 global attribute of CLM!", 0);		
		ret = RandW.Read_CLM_L1_HDF(CAPI_TAN1.CAPIL1B1KMHDF_DF,L2_CLM_GLOB); 
        if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read L1 global attribute of CLM fail!");
			sprintf(outputParam.cErrorDescription, "Read L1 global attribute of CLM fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Read L1 CLM global attribute finish!\n");
		PGSQAlog.WriteLogFile("Read L1 CLM global attribute finish!", 0);		
		
		
		//为L2_aer_hdf，L2_clm_hdf申请内存
		printf("Start to apply memory for image!\n");
		PGSQAlog.WriteLogFile(" Start to apply memory for image!", 0);
		ret = RandW.ApplyMemory(L2_aer_hdf,L2_clm_hdf,L2_CLM_GLOB);
        if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "ApplyMemory fail!");
			sprintf(outputParam.cErrorDescription, "ApplyMemory fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
        printf(" Apply memory finish!\n");		
		PGSQAlog.WriteLogFile(" Apply memory finish!", 0); 
		 
		 //读云检测反演L2数据	
        printf("Start to read date of CLM_L2_HDF!\n");
		PGSQAlog.WriteLogFile("Start to read date of CLM_L2_HDF!", 0);		
		ret = RandW.read_CAPI_CLM_L2_FILE(CAPI_TAN1.CLML2HDF_DF_TEMP,L2_clm_hdf);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read CLM_L2_HDF fail!");
			sprintf(outputParam.cErrorDescription, "Read CLM_L2_HDF fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Read date of CLM_L2_HDF finish!\n");
		PGSQAlog.WriteLogFile("Read date of CLM_L2_HDF finish!", 0);
		
		
		//获取CLM CAPI独有的全局属性
		printf("Start to Read CLM_L2_HDF attribute!\n");
		PGSQAlog.WriteLogFile("Start to Read CLM_L2_HDF attribute!", 0);
		ret = RandW.Read_CLM_CFG(&inputParam,&CAPI_TAN1,L2_CLM_GLOB,L2_clm_hdf);
        if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read CLM_L2_HDF attribute fail!");
			sprintf(outputParam.cErrorDescription, "Read CLM_L2_HDF attribute fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Read CLM_L2_HDF attribute finish!\n");
		PGSQAlog.WriteLogFile("Read CLM_L2_HDF attribute finish!", 0);	
		
		
		/************************************************
		*               写入云检测全部属性
		*************************************************/
		
		//对轨道号赋值（因为L2_CLM_GLOB中轨道号为空）
		sprintf(L2_CLM_GLOB.orbitnumber, "%s",inputParam.orbitnumber); 
		
		//把从配置文件读取的版本日期和版本号写入全局属性  added by liwei @20170627
		sprintf(L2_CLM_GLOB.VersionofSoftware, "%s",CAPI_TAN1.VersionofSoftware);
	    sprintf(L2_CLM_GLOB.DateofSoftware, "%s",CAPI_TAN1.DateofSoftware);
		
		printf("Start to write CLM_L2_HDF global attribute!\n");
		PGSQAlog.WriteLogFile("Start to write CLM_L2_HDF global attribute!", 0);
		ret = RandW.Write_CLM_L2_HDF(CAPI_TAN1.CLML2HDF_DF_TEMP,L2_CLM_GLOB);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write CLM_L2_HDF global attribute fail!");
			sprintf(outputParam.cErrorDescription, "Write CLM_L2_HDF global attribute fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Write CLM_L2_HDF global attribute finish!\n");
		PGSQAlog.WriteLogFile("Write CLM_L2_HDF global attribute finish!", 0);
		
		
		/************************************************
		*               出云检测浏览图
		*************************************************/
		printf("Start to write image of CLM CAPI!\n");
		PGSQAlog.WriteLogFile("Start to write image of CLM CAPI!", 0);
        ret = RandW.write_ClmJpeg(CAPI_TAN1.CLML2JPG_DF_TEMP,L2_clm_hdf,L2_CLM_GLOB);
		 if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write_ClmJpeg fail!");
			sprintf(outputParam.cErrorDescription, "Write_ClmJpeg fail!");

			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}		
		printf(" Write clm image of CLM CAPI finish!\n");
		PGSQAlog.WriteLogFile("Write clm image of CLM CAPI finish!", 0);
		
		
	
		/*********************outputxml:云检测反演数据归档******/

        printf("\nStart to Write CLM output.xml file!\n");
		PGSQAlog.WriteLogFile("Start to Write CLM output.xml file!", 0);		
		 ret = WriteCLMOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write CLM output.xml file fail!");
			sprintf(outputParam.cErrorDescription, "Write CLM output.xml file fail!");

			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Write CLM output.xml file finish!\n");	
        PGSQAlog.WriteLogFile("Write CLM output.xml file finish!", 0);
		
	    /*************云检测反演数据归档完成*************/	
		
		//当云检测反演正常运行时，完成CLM数据存档和output.xml 的完成
		//当气溶胶检测反演正常时，执行CLM 和 AER 的数据归档和 output.xml 的完成，覆盖此处的
		//当气溶胶检测异演正常时，保留此处的CLM数据存档和output.xml 
		
		
	
		
		
	
		
		/********************************************************
		*
		*                     气溶胶反演函数        
		*
		********************************************************/ 
		aer_start = clock();
		printf("\n\nStart to load aer inverse algorithm !\n");
        PGSQAlog.WriteLogFile("Start to load aer inverse algorithm !", 0);	

		
		#ifdef __linux__
			tan_aer_(
		#else
			tan_aer(
		#endif
				 CAPI_TAN1.IFL_NWP,
				 CAPI_TAN1.NWP_DF,
				 CAPI_TAN1.NWP_DFtemp,
				 CAPI_TAN1.CAPIL1B250HDF_DF,
				 CAPI_TAN1.CAPIL1B250GEOHDF_DF,
				 CAPI_TAN1.CAPIL1B1KMHDF_DF,
				 CAPI_TAN1.CAPIL1B1KMGEOHDF_DF,
				 CAPI_TAN1.CAPIConfigDIR,
				 CAPI_TAN1.Filename_cfg,
				 CAPI_TAN1.CAPIARP,	
				 CAPI_TAN1.WORKDIR,
				 CAPI_TAN1.LogFilename,
				 CAPI_TAN1.IFL_CAPI_L2_AER_HDF,
				 CAPI_TAN1.IFL_CAPI_L2_CLM_HDF,
				 CAPI_TAN1.AERL2HDF_DF_TEMP,
				 CAPI_TAN1.CLML2HDF_DF_TEMP,
				 CAPI_TAN1.Mode,
				 CAPI_TAN1.AERresolution,
				 CAPI_TAN1.CLMresolution,
				 CAPI_TAN1.AERVersion,
				 CAPI_TAN1.CLMVersion, 
				 &outputParam.iReturnCode, 
				 outputParam.cReturnDescription,
				 outputParam.cErrorDescription,
				 &outputParam.OutputDataNum,
				 CAPI_TAN1.DATA
				 );
				 
				 outputParam.cReturnDescription[255] = '\0';
		         outputParam.cErrorDescription[511] = '\0';
				 		 	
		if( outputParam.iReturnCode>0 )
		{
			//写反馈给PP的输出文件
			WriteOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);
			printf("%s\n", outputParam.cReturnDescription);
			printf("AER inverse algorithm failed!\n");
			PGSQAlog.WriteLogFile("AER inverse algorithm failed!",1);			
			return  outputParam.iReturnCode;
		} 

		printf(" Load AER inverse algorithm success!\n");
		PGSQAlog.WriteLogFile(" Load AER inverse algorithm success!", 0);
		
		aer_finish=clock();
		totaltime=(double)(aer_finish-aer_start)/CLOCKS_PER_SEC;
		printf("This AER CAPI costs %f seconds\n",totaltime);
		
		
		/************************************************
		*               读取气溶胶全局属性
		*************************************************/
		printf("\nStart to read L1 global attribute of AER!\n");
		PGSQAlog.WriteLogFile("Start to read L1 global attribute of AER!", 0);
		//读取气溶胶1B全局属性
		ret = RandW.Read_AER_L1_HDF(CAPI_TAN1.CAPIL1B1KMGEOHDF_DF,L2_CLM_GLOB);
        if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read L1 global attribute of AER fail!");
			sprintf(outputParam.cErrorDescription, "Read L1 global attribute of AER fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Read L1 global attribute of AER finish!\n");
		PGSQAlog.WriteLogFile("Read L1 global attribute of AER finish!", 0);
		
		//读气溶胶反演L2数据
        printf("Start to read date of AER_L2_HDF!\n");
		PGSQAlog.WriteLogFile("Start to read date of AER_L2_HDF!", 0);	


		
		ret = RandW.read_CAPI_AER_L2_FILE(CAPI_TAN1.AERL2HDF_DF_TEMP,L2_aer_hdf);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read AER_L2_HDF fail!");
			sprintf(outputParam.cErrorDescription, "Read AER_L2_HDF fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Read date of AER_L2_HDF finish!\n");
		PGSQAlog.WriteLogFile("Read date of AER_L2_HDF finish!", 0);
		
		//获取气溶胶模块独有的全局属性
		printf("Start to read AER_L2_HDF attribute!\n");
		PGSQAlog.WriteLogFile("Start to read AER_L2_HDF attribute!", 0);		
        ret = RandW.Read_AER_CFG(&inputParam,&CAPI_TAN1,L2_CLM_GLOB,L2_aer_hdf);
        if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Read AER_L2_HDF attribute fail!");
			sprintf(outputParam.cErrorDescription, "Read AER_L2_HDF attribute fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}		 
        printf(" Read AER_L2_HDF attribute finish!\n");
		PGSQAlog.WriteLogFile("Read AER_L2_HDF attribute finish!", 0);
		
		
		
		/************************************************
		*               写入气溶胶全部属性
		*************************************************/
		printf("Start to write global attribute to AER_L2_HDF!\n");
		PGSQAlog.WriteLogFile("Start to write global attribute to AER_L2_HDF!", 0);
		
		sprintf(L2_CLM_GLOB.orbitnumber, "%s",inputParam.orbitnumber); //对轨道号赋值（因为 L2_CLM_GLOB 中轨道号为空）
	   
        //把从配置文件读取的版本日期和版本号写入全局属性  added by liwei @20170627
	    sprintf(L2_CLM_GLOB.VersionofSoftware, "%s",CAPI_TAN1.VersionofSoftware);
	    sprintf(L2_CLM_GLOB.DateofSoftware, "%s",CAPI_TAN1.DateofSoftware);
	
		ret = RandW.Write_AER_L2_HDF(CAPI_TAN1.AERL2HDF_DF_TEMP,L2_CLM_GLOB);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write AER_L2_HDF global attribute fail!");
			sprintf(outputParam.cErrorDescription, "Write AER_L2_HDF global attribute fail!");
			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Write AER_L2_HDF global attribute finish!\n");
		PGSQAlog.WriteLogFile("Write AER_L2_HDF global attribute finish!", 0);
		
		
 	
		/************************************************
		*               出气溶胶反演浏览图
		*************************************************/

		
		printf("Start to Write aer image!\n");
		PGSQAlog.WriteLogFile("Start to write aer image!", 0);
		ret = RandW.write_AerJpeg(CAPI_TAN1.AERL2JPG_DF_TEMP,L2_aer_hdf,L2_clm_hdf,L2_CLM_GLOB);
		 if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write_AerJpeg fail!");
			sprintf(outputParam.cErrorDescription, "Write_AerJpeg fail!");

			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Write aer image of CAPI finish!\n");
		PGSQAlog.WriteLogFile("Write aer image of CAPI finish!", 0);
		
		
		
		 		
		//****************************outputxml:数据归档***************************	
		
        printf("\nStart to Write output.xml file!\n");
		PGSQAlog.WriteLogFile("Start to Write output.xml file!", 0);		
		 ret = WriteOutputXmlFile(outputParam, cOutputXmlFileName,&CAPI_TAN1);
		if (ret > 0) 
		{
			outputParam.iReturnCode = ret;
			sprintf(outputParam.cReturnDescription, "Write output.xml file fail!");
			sprintf(outputParam.cErrorDescription, "Write output.xml file fail!");

			RandW.WrReportFile(ret, Warningcount);
			return ret;
		}
		printf(" Write output.xml file finish!\n");	
        PGSQAlog.WriteLogFile("Write output.xml file finish!", 0);
		
		
		//**************************释放变量申请的空间*****************************
		printf("\nStart to free memory!\n");	
        PGSQAlog.WriteLogFile("Start to free memory!", 0);
		RandW.FreeMemory(L2_aer_hdf,L2_clm_hdf,L2_CLM_GLOB);
		printf(" Free memory finish!\n");	
        PGSQAlog.WriteLogFile("Free memory finish!", 0);
		printf("*****************TAN1-CAPIX-RV module End!*****************\n");
		PGSQAlog.WriteLogFile("*********TAN1-CAPIX-RV module End!*********!\n", 0); 			
	
	//**************************统计模块运行时间*****************************
	mainfinish=clock();
	maintotaltime=(double)(mainfinish-mainstart)/CLOCKS_PER_SEC;
	printf("Used Time: %f seconds\n",maintotaltime);
	printf("DONE!\n");
	
	return SUCCESS;
}

void handler(int sig, siginfo_t *scp, void *addr)
{
	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sigaddset(&sa.sa_mask, SIGUSR1);
	sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
	fprintf(stdout, "Entering handler for SIGUSR1...\n");
	fflush(stdout);
	exit(0xEE);
   }

   
