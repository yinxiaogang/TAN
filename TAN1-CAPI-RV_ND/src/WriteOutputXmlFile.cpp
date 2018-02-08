/***********************************************************************************************************
Copyright (c) 2016,航天503所
All rights reserved.
文件名：WriteOutputXmlFile.cpp
编写者：huyuan
编写日期：2016年1月
简要描述： 输出反馈给PP的outputXML
  
修改记录：
当前版本：1.0.0
**************************************************************************************************************/
#include "WriteOutputXmlFile.h"
#include "HSCO2QAErrorCode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "AuxProcess.h"
#include "RetrievalDef.h"
#include "WinOrLinux.h"
#include "ParamDll_TAN.h"
#include "timestring.h"
#include "TanFilename.h"
#include "hdf5.h"
#include "hdf5_hl.h"
#include "tanhdf.h"
#include "StructDefine.h"
#include "PublicErrorCode.h"
//写输出XML文件


 
 //去掉字符串右边空格
 void  ringhtstrim(char *test0,char *result) 
{
	   int in = 0;
	   char *buf       = test0;
	   char *outer_ptr = NULL;
	   char *inner_ptr = NULL;
	   char *p[strlen(test0)];
	   char tempresult[256];
	   char sep[]=" \0";
	
	   //将字符串用sep分割后存放到p中
	   while((p[in] = strtok_r(buf,sep,&outer_ptr)) != NULL)
	    {
		  buf = p[in];
		  while((p[in]=strtok_r(buf,sep, &inner_ptr)) != NULL)
		  {
			  in++;
			  buf = NULL;
		  }
		   buf = NULL;
	    } 
		
		//将P中小字符段连接起来，中间加上“ ”
		for(int i=0; i<in; i++)    
		{
			if(0==i)
			{
				strcpy(tempresult,p[i]);
				strcat(tempresult," ");
			}
			else
			{
				strcat(tempresult,p[i]);
				strcat(tempresult," ");
			}
			
		} 
		
		//去掉最后一个空格
		for(int i=0; i<strlen(tempresult)-1;i++)
		{
			result[i] = tempresult[i];
		}
		
}



int WriteOutputXmlFile(struct TDPOutputxml outputParam, char *cFileName,struct TanFileName *tanfilename)
{
	FILE *fp;
	int retValue;
	
	
	//去掉字符串 outputParam.cReturnDescription 后面所有的空格,赋给 cReturnDescription
	char cReturnDescription[256];
	char cErrorDescription[512];
	memset(cReturnDescription,0,256);
	memset(cErrorDescription,0,512);
	
	ringhtstrim(outputParam.cReturnDescription,cReturnDescription);   //去掉字符串右边空格
	ringhtstrim(outputParam.cErrorDescription,cErrorDescription);     //去掉字符串右边空格
	
	
	//开始写output.xml文件
	fp = fopen(cFileName, "w");
	if(fp == NULL)
	{
		printf("Open output.xml file: %s error!\n",cFileName);
		return OUTPUT_PRODUCT_ERR;
	}  
 	
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"GB2312\"?>\n");
	fprintf(fp, "<root>\n");
	fprintf(fp, "\t<data>\n");
	fprintf(fp, "\t\t<ReturnCode>%d</ReturnCode>\n", outputParam.iReturnCode);
	fprintf(fp, "\t\t<Description>%s",cReturnDescription); 
	
	if(outputParam.iReturnCode != 0 && strcmp(cErrorDescription,cReturnDescription)!= 0 )   //在错误的时候
	{	
		fprintf(fp, " -- %s",cErrorDescription); 
	}/**/
   
	fprintf(fp, "</Description>\n"); 
	
	
	
	/**********在耀斑模式（“GL”）下不进行生成产品*************/
	if(outputParam.iReturnCode == 0 && strcmp(tanfilename->Mode,FLARE_MODE)!= 0)
	{
		fprintf(fp, "\t\t<IFlastProduct>Y</IFlastProduct>\n");//输出文件是否最终产品
		fprintf(fp, "\t\t<Detector>CAPIX</Detector>\n");//产品的载荷名称
		fprintf(fp, "\t\t<Mode>%s</Mode>\n",tanfilename->Mode);//模式
		fprintf(fp, "\t\t<Level>L2</Level>\n\n");//产品的级别
		char *shcmmd=new char[1024];
		if(shcmmd == NULL)
		{
			printf("Apply memory failed!\n");
			return SYSTEM_APPLY_MEMORY_ERR;
		
		} 
		
		/****************************HDF数据归档******************************/
        //云检测HDF文件压缩并归档		
		sprintf(shcmmd, "/TanOnLine/LIB/hdf-5-18/bin/h5repack -f GZIP=5 %s %s",  tanfilename->CLML2HDF_DF_TEMP,tanfilename->CLML2HDF_DF);		
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		}
		
		//气溶胶HDF文件压缩并归档
		sprintf(shcmmd, "/TanOnLine/LIB/hdf-5-18/bin/h5repack -f GZIP=5 %s %s",  tanfilename->AERL2HDF_DF_TEMP,tanfilename->AERL2HDF_DF);		
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		}
		
        //计数归档文件数量		
		int file1,file2; 
		int Returnt_temp;
		
		char SDSpath[64];
		file1 = 0;		
		FILE *file_HDF_clm;
		L2_CAPIX_CLM_HDF L2_clm_temp;
		if(NULL != (file_HDF_clm = fopen(tanfilename->CLML2HDF_DF,"r")))    //判断文件是否成功打开
		{	
	        //clm 结构体变量申请内存
			L2_clm_temp.Geo.Longitude = NULL;	
			if(L2_clm_temp.Geo.Longitude == NULL)
			{
				if( (L2_clm_temp.Geo.Longitude = (float *)calloc(400000*21,sizeof(float))) == NULL  )
				{
					return SYSTEM_APPLY_MEMORY_ERR;
				}
			}
	        int Returnt_temp;
			strcpy(SDSpath,"Geo/Longitude");
			Returnt_temp = Readhdf(tanfilename->CLML2HDF_DF,SDSpath,L2_clm_temp.Geo.Longitude,H5T_NATIVE_FLOAT);
			if(Returnt_temp == 0)
			{
				++file1;
				Returnt_temp = -1;
			}			
		}
		
		file2 = 0;
		FILE *file_HDF_aer;
		L2_CAPIX_AER_HDF L2_aer_temp;
		if(NULL != (file_HDF_aer = fopen(tanfilename->AERL2HDF_DF,"r")))      //判断文件是否成功打开
		{ 
		    //aer 结构体变量申请内存
	        L2_aer_temp.Geo.Longitude = NULL;	
			if(L2_aer_temp.Geo.Longitude == NULL)
			{
				if( (L2_aer_temp.Geo.Longitude = (float *)calloc(400000*21,sizeof(float))) == NULL  )
				{
					return SYSTEM_APPLY_MEMORY_ERR;
				}
			}
	        strcpy(SDSpath,"Geo/Longitude");			
			Returnt_temp = Readhdf(tanfilename->AERL2HDF_DF,SDSpath,L2_aer_temp.Geo.Longitude,H5T_NATIVE_FLOAT);
			if(Returnt_temp == 0)
			{
				++file2;
				Returnt_temp = -1;				
			}			
		}
		//输出产品HDF文件数量
		outputParam.OutputDataNum = file1+file2;
		fprintf(fp, "\t\t<OutputDataNum>%d</OutputDataNum>\n",outputParam.OutputDataNum);
		if(file1 == 1)
		{
				fprintf(fp, "\t\t<DATA1>%s</DATA1>\n",tanfilename->CLML2HDF_DF);
		}
		if(file2 == 1)
		{
				fprintf(fp, "\t\t<DATA2>%s</DATA2>\n",tanfilename->AERL2HDF_DF);
		}
		
		/****************************浏览图数据归档******************************/
		//云检测浏览图归档		
		sprintf(shcmmd, "cp %s %s",  tanfilename->CLML2JPG_DF_TEMP,tanfilename->CLML2JPG_DF);
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		} 
		//气溶胶浏览图归档
		sprintf(shcmmd, "cp %s %s",  tanfilename->AERL2JPG_DF_TEMP, tanfilename->AERL2JPG_DF);
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		} 
		int file3,file4;      //统计浏览图个数变量		
		file3 = 0;		
		FILE *file_jpg_clm;		
		if(NULL != (file_jpg_clm = fopen(tanfilename->CLML2JPG_DF,"r")))    //判断文件是否成功打开
		{		        
			++file3;				
		}
		file4 = 0;
		FILE *file_jpg_aer;
		if(NULL != (file_jpg_aer = fopen(tanfilename->AERL2JPG_DF,"r")))      //判断文件是否成功打开
		{ 
		    ++file4;					
		}		
		outputParam.BGNum = file3+file4;
		fprintf(fp, "\n\t\t<BGNum>%d</BGNum>\n",outputParam.BGNum);//产品JPG文件数量
		if(file3 == 1)
		{
				fprintf(fp, "\t\t<BG_DATA1>%s</BG_DATA1>\n",tanfilename->CLML2JPG_DF);
		}
		if(file4 == 1)
		{
				fprintf(fp, "\t\t<BG_DATA2>%s</BG_DATA2>\n",tanfilename->AERL2JPG_DF);
		}
		
	   delete []shcmmd;
	}	
	fprintf(fp, "\t</data>\n");
	fprintf(fp, "</root>\n");
	fclose(fp);
	return SUCCESS;
}





//该模块只针对于CLM， added by liwei @20170623

int WriteCLMOutputXmlFile(struct TDPOutputxml outputParam, char *cFileName,struct TanFileName *tanfilename)
{
	FILE *fp;
	int retValue;
	
	
	//去掉字符串 outputParam.cReturnDescription 后面所有的空格,赋给 cReturnDescription
	char cReturnDescription[256];
	char cErrorDescription[512];
	memset(cReturnDescription,0,256);
	memset(cErrorDescription,0,512);
	
	ringhtstrim(outputParam.cReturnDescription,cReturnDescription);   //去掉字符串右边空格
	ringhtstrim(outputParam.cErrorDescription,cErrorDescription);     //去掉字符串右边空格
	
	
	//开始写output.xml文件
	fp = fopen(cFileName, "w");
	if(fp == NULL)
	{
		printf("Open output.xml file: %s error!\n",cFileName);
		return OUTPUT_PRODUCT_ERR;
	}  
 	
	fprintf(fp, "<?xml version=\"1.0\" encoding=\"GB2312\"?>\n");
	fprintf(fp, "<root>\n");
	fprintf(fp, "\t<data>\n");
	fprintf(fp, "\t\t<ReturnCode>%d</ReturnCode>\n", outputParam.iReturnCode);
	fprintf(fp, "\t\t<Description>%s",cReturnDescription); 
	
	if(outputParam.iReturnCode != 0 && strcmp(cErrorDescription,cReturnDescription)!= 0 )   //在错误的时候
	{	
		fprintf(fp, " -- %s",cErrorDescription); 
	}/**/
   
	fprintf(fp, "</Description>\n"); 
	
	
	
	/**********在耀斑模式（“GL”）下不进行生成产品*************/
	if(outputParam.iReturnCode == 0 && strcmp(tanfilename->Mode,FLARE_MODE)!= 0)
	{
		fprintf(fp, "\t\t<IFlastProduct>Y</IFlastProduct>\n");//输出文件是否最终产品
		fprintf(fp, "\t\t<Detector>CAPIX</Detector>\n");//产品的载荷名称
		fprintf(fp, "\t\t<Mode>%s</Mode>\n",tanfilename->Mode);//模式
		fprintf(fp, "\t\t<Level>L2</Level>\n\n");//产品的级别
		char *shcmmd=new char[1024];
		if(shcmmd == NULL)
		{
			printf("Apply memory failed!\n");
			return SYSTEM_APPLY_MEMORY_ERR;
		
		} 
		
		/****************************HDF数据归档******************************/
        //云检测HDF文件压缩并归档		
		sprintf(shcmmd, "/TanOnLine/LIB/hdf-5-18/bin/h5repack -f GZIP=5 %s %s",  tanfilename->CLML2HDF_DF_TEMP,tanfilename->CLML2HDF_DF);		
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		}
		
		
		
        //计数归档文件数量		
		int file1; 
		int Returnt_temp;
		
		char SDSpath[64];
		file1 = 0;		
		FILE *file_HDF_clm;
		L2_CAPIX_CLM_HDF L2_clm_temp;
		if(NULL != (file_HDF_clm = fopen(tanfilename->CLML2HDF_DF,"r")))    //判断文件是否成功打开
		{	
	        //clm 结构体变量申请内存
			L2_clm_temp.Geo.Longitude = NULL;	
			if(L2_clm_temp.Geo.Longitude == NULL)
			{
				if( (L2_clm_temp.Geo.Longitude = (float *)calloc(400000*21,sizeof(float))) == NULL  )
				{
					return SYSTEM_APPLY_MEMORY_ERR;
				}
			}
	        int Returnt_temp;
			strcpy(SDSpath,"Geo/Longitude");
			Returnt_temp = Readhdf(tanfilename->CLML2HDF_DF,SDSpath,L2_clm_temp.Geo.Longitude,H5T_NATIVE_FLOAT);
			if(Returnt_temp == 0)
			{
				++file1;
				Returnt_temp = -1;
			}			
		}
		
		
		//输出产品HDF文件数量
		outputParam.OutputDataNum = file1;
		fprintf(fp, "\t\t<OutputDataNum>%d</OutputDataNum>\n",outputParam.OutputDataNum);
		if(file1 == 1)
		{
				fprintf(fp, "\t\t<DATA1>%s</DATA1>\n",tanfilename->CLML2HDF_DF);
		}
		
		
		/****************************浏览图数据归档******************************/
		//云检测浏览图归档		
		sprintf(shcmmd, "cp %s %s",  tanfilename->CLML2JPG_DF_TEMP,tanfilename->CLML2JPG_DF);
		retValue = system(shcmmd);
		if(retValue != 0)
		{
				outputParam.iReturnCode = COPY_PRODUCT_FILE_FAILED;
				strcpy(outputParam.cReturnDescription,"COPY_PRODUCT_FILE_FAILED");
				return COPY_PRODUCT_FILE_FAILED;
		} 
		
		int file3;      //统计浏览图个数变量		
		file3 = 0;		
		FILE *file_jpg_clm;		
		if(NULL != (file_jpg_clm = fopen(tanfilename->CLML2JPG_DF,"r")))    //判断文件是否成功打开
		{		        
			++file3;				
		}
		
		outputParam.BGNum = file3;
		fprintf(fp, "\n\t\t<BGNum>%d</BGNum>\n",outputParam.BGNum);//产品JPG文件数量
		if(file3 == 1)
		{
				fprintf(fp, "\t\t<BG_DATA1>%s</BG_DATA1>\n",tanfilename->CLML2JPG_DF);
		}
		
		
	   delete []shcmmd;
	}	
	fprintf(fp, "\t</data>\n");
	fprintf(fp, "</root>\n");
	fclose(fp);
	return SUCCESS;
}
