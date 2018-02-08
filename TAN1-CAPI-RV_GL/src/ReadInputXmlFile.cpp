/***********************************************************************************************************
Copyright (c) 2016,航天503所
All rights reserved.
文件名：ReandInputXmlFile.cpp
编写者：汪红强
编写日期：2008年3月05日
简要描述： 输出反馈给PP的outputXML
  
修改记录：
当前版本：1.0.0
**************************************************************************************************************/
#include <string.h>
//#include "TDPErrorCode.h"
#include "HSCO2QAErrorCode.h"
#include "ReadInputXmlFile.h"
#include "config.h"
#include "RcDef.h"
#include "ParamDll_TAN.h"
#include "PublicErrorCode.h"


#include <stdlib.h>
#include <unistd.h>

char* GetFilename(char* fullpathname)  //获取该文件名（路径名+文件名）中文件名（不含路径）  
{ 
	char* save_name, *pos; 
	int name_len; 
	name_len = strlen(fullpathname); 
	pos = fullpathname + name_len; 
	while(*pos != '/' && pos != fullpathname) 
		pos --; 
	if(pos == fullpathname) 
	{ 
		save_name = fullpathname+1; 
		return save_name; 
	} 
	name_len = name_len-(pos-fullpathname); 
	save_name = (char*) malloc(name_len+1); 
	memcpy(save_name,pos+1,name_len); 
	return save_name; 
} 

char* GetFilepath(char* fullpathname)  //获取路径名
{
	int flag_X = 0;
	int  i = 0;
	while(fullpathname[i])
	{
		if('/' == fullpathname[i])
			flag_X = i;
		i++;
	}
	char* filePath=new char[255];
	strncpy(filePath,fullpathname,flag_X+1);
	return filePath;
	delete filePath;
	
}

void nextday(char date[])
{
	int y=0,m=0,d=0;
	char tmp[5];
	strncpy(tmp, date, 4);
	sscanf(tmp, "%d", &y);
	strncpy(tmp, date+4, 2);
	tmp[2] = '\0';
	sscanf(tmp, "%d", &m);
	strncpy(tmp, date+6, 2);
	sscanf(tmp, "%d", &d);
	
	int flag = 0;
	if ((y%4==0 && y%100!=0) || y%400==0)
	{
		flag = 1;
	}
	
	if (m==2 && d==(28+flag))
	{
		m++;
		d = 1;
	}
	else if ((m==4||m==6||m==9||m==11) && d==30)
	{
		m++;
		d = 1;
	}
	else if ((m==1||m==3||m==5||m==7||m==8||m==10||m==12) && d==31)
	{
		m++;
		d = 1;
	}
	else
	{
		d++;
	}
	
	if (m == 13)
	{
		y++;
		m = 1;
	}
	
	sprintf(date, "%d", y);
	if (m < 10)
	{
		sprintf(tmp, "0%d", m);
	}
	else
	{
		sprintf(tmp, "%d", m);
	}
	strcat(date, tmp);
	if (d < 10)
	{
		sprintf(tmp, "0%d", d);
	}
	else
	{
		sprintf(tmp, "%d", d);
	}
	strcat(date, tmp);
}




void prevday(char date[])
{
	int y=0,m=0,d=0;
	char tmp[5];
	strncpy(tmp, date, 4);
	sscanf(tmp, "%d", &y);
	strncpy(tmp, date+4, 2);
	tmp[2] = '\0';
	sscanf(tmp, "%d", &m);
	strncpy(tmp, date+6, 2);
	sscanf(tmp, "%d", &d);
	
	int flag = 0;
	if ((y%4==0 && y%100!=0) || y%400==0)
	{
		flag = 1;
	}
	
	if (m==3 && d==1)
	{
		m = 2;
		d = 28+flag;
	}
	else if ((m==5||m==7||m==10||m==12) && d==1)
	{
		m--;
		d = 30;
	}
	else if ((m==2||m==4||m==6||m==8||m==9||m==11||m==1) && d==1)
	{
		m--;
		d = 31;
	}
	else
	{
		d--;
	}
	
	if (m == 0)
	{
		y--;
		m = 12;
	}
	
	sprintf(date, "%d", y);
	if (m < 10)
	{
		sprintf(tmp, "0%d", m);
	}
	else
	{
		sprintf(tmp, "%d", m);
	}
	strcat(date, tmp);
	if (d < 10)
	{
		sprintf(tmp, "0%d", d);
	}
	else
	{
		sprintf(tmp, "%d", d);
	}
	strcat(date, tmp);
}


//把文件名中时刻向前推12小时
int prev12h(char strT213File[])
{
	char filedate[9];
	char tmp[4];
	int hour = -1;
	int time = -1;
	
	char *T639filepath = GetFilepath(strT213File);
	char *T639filename = GetFilename(strT213File);

	printf("T639filepath = %s\n",T639filepath);
	printf("T639filename = %s\n",T639filename);

	strncpy(filedate, &T639filename[16], 2);
	filedate[2] = '\0';
	sscanf(filedate, "%d", &hour);

	strncpy(filedate, &T639filename[19], 3);
	filedate[3] = '\0';
	sscanf(filedate, "%d", &time);
	
	strncpy(filedate, &T639filename[8], 8);
	filedate[8] = '\0';
	
	
	switch (hour)
	{
	case 0:
		hour = 12;
		prevday(filedate);    //日期转化为前一天  added by liwei @20160602
		break;
	case 12:
		hour = 0;
		break;
	default:
		return -1;
	}
	
	
	strcat(T639filepath, "gmf.639."); 
    strcpy(strT213File,T639filepath);	
	
	if (hour < 10)
	{
		strcat(strT213File, filedate);
		sprintf(tmp, "0%d", hour);
		strcat(strT213File, tmp);

		//根据吴春强最新意见修改：
		//由于T639原始预报场数据滞后到达业务系统，因此业务系统处理时需要使用前一天的原始预报场数据。
		//strcat(strT213File, "000.grb2");
		sprintf(tmp, "0%d", time+12);
		strcat(strT213File, tmp);
		strcat(strT213File, ".grb2");
	}
	else if (hour == 24)
	{
		nextday(filedate);  //日期转化为后一天 added by liwei @20160602
		strcat(strT213File, filedate);
		strcat(strT213File, "12012.grb2");		
	}
	else
	{
		strcat(strT213File, filedate);
		sprintf(tmp, "%d", hour);
		strcat(strT213File, tmp);

		sprintf(tmp, "0%d", time+12);//根据吴春强提供的匹配策略更改。20141028 updated by zhongf。
		strcat(strT213File, tmp);
		strcat(strT213File, ".grb2");
	}
	printf("strT213File = %s\n",strT213File);
	
	T639filepath = NULL;
	T639filepath = NULL;
	return SUCCESS;  // 1;
}


//解析输入XML文件
int ReadInputXmlFile(struct TDPInputxml *inputParam, char *xmlfileName, struct TDPOutputxml *outputParam,struct TanFileName *tanfilename,CWriteLog *log)
{	
	PList plist;
	int iLength;
	int retValue;
	char strlog[256];
	
	retValue = plist.MakeList(xmlfileName);
	if(retValue != 1)
	{
		outputParam->iReturnCode =  OPEN_INPUT_XML_FAILED;
		strcpy(outputParam->cReturnDescription,"OPEN_INPUT_XML_FILE_FAILED");
		strcpy(outputParam->cErrorDescription,"OPEN_INPUT_XML_FILE_FAILED");
		return  PARSE_INPUTXML_ERR;
	}
	//todo: parse input XML file
	CConfig xmlConfig(xmlfileName);
	retValue = xmlConfig.GetValue("WORKDIR", inputParam->WORKDIR);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get WORKDIR from input XML!");
		strcpy(outputParam->cErrorDescription,"Fail to get WORKDIR from input XML!");
		return PARSE_INPUTXML_ERR;
	}

	//get log file  name
	sprintf(tanfilename->LogFilename, "%slog.txt", inputParam->WORKDIR);
	//printf("\n%s\n\n",tanfilename->LogFilename);
	log->FormTheLogFileName(tanfilename->LogFilename);
	log->WrOldMessage();
	
	retValue = xmlConfig.GetValue("Detector", inputParam->Detector);
	if(retValue != 0)
	{
		log->WriteLogFile( "Fail to get Detector from input XML!",-1);
		strcpy(outputParam->cErrorDescription,"Fail to get Detector from input XML!");
		strcpy(inputParam->Detector, "HSCO2");
	}
	retValue = xmlConfig.GetValue("NWPType", inputParam->NWPType);
	if(retValue != 0)
	{
		log->WriteLogFile( "Fail to get NWPType from input XML!",-1);
		strcpy(outputParam->cErrorDescription,"Fail to get NWPType from input XML!");
		strcpy(inputParam->NWPType, "T639");
	}
	retValue = xmlConfig.GetValue("TaskSource", inputParam->TaskSource);
	if(retValue != 0)
	{
		log->WriteLogFile( "Fail to get TaskSource from input XML!",-1);
		strcpy(outputParam->cErrorDescription,"Fail to get TaskSource from input XML!");
		strcpy(inputParam->TaskSource, "COSS");
	}
	retValue = xmlConfig.GetValue("ProductLevel", inputParam->cProductLevel);
	if(retValue != 0)
	{
		log->WriteLogFile( "Fail to get ProductLevel from input XML!",-1);
		strcpy(outputParam->cErrorDescription,"Fail to get ProductLevel from input XML!");
		strcpy(inputParam->cProductLevel, "L2");
	}
	retValue = xmlConfig.GetValue("ConfigDIR", inputParam->ConfigDIR);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get ConfigDIR from input XML!");
		strcpy(outputParam->cErrorDescription, "Fail to get ConfigDIR from input XML!");
		return PARSE_INPUTXML_ERR;
	}

	retValue = xmlConfig.GetValue("InputGPSDataPath", inputParam->InputGPSDataPath);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get InputGPSDataPath from input XML!");
		strcpy(outputParam->cErrorDescription, "Fail to get InputGPSDataPath from input XML!");
		return PARSE_INPUTXML_ERR;
	}

	retValue = xmlConfig.GetValue("InputNWPDataPath", inputParam->InputNWPDataPath);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get InputNWPDataPath from input XML!");
		strcpy(outputParam->cErrorDescription, "Fail to get InputNWPDataPath from input XML!");
		return PARSE_INPUTXML_ERR;
	}
	retValue = xmlConfig.GetValue("L0DIR", inputParam->L0DIR);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get L0DIR from input XML!");
		strcpy(outputParam->cErrorDescription, "Fail to get L0DIR from input XML!");
		return PARSE_INPUTXML_ERR;
	}
	retValue = xmlConfig.GetValue("L1ProductPath", inputParam->L1ProductPath);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get L1ProductPath from input XML!");
		strcpy(outputParam->cErrorDescription,  "Fail to get L1ProductPath from input XML!");
		return PARSE_INPUTXML_ERR;
	}
	retValue = xmlConfig.GetValue("L2ProductPath", inputParam->L2ProductPath);
	if(retValue != 0)
	{
		outputParam->iReturnCode = PARSE_INPUTXML_ERR;
		strcpy(outputParam->cReturnDescription, "Fail to get L2ProductPath from input XML!");
		strcpy(outputParam->cErrorDescription, "Fail to get L2ProductPath from input XML!");
		return PARSE_INPUTXML_ERR;
	}
	/* retValue = xmlConfig.GetValue("OrbitNumber", inputParam->orbitnumber);
	if(retValue != 0)
	{
		log->WriteLogFile( "Fail to get Orbitnumber from input XML!",-1);
		strcpy(inputParam->orbitnumber, "XXXXX");
	}
	strcpy(tanfilename->orbitNumber,inputParam->orbitnumber); */	
	
	if(0 == strcasecmp(inputParam->cProductLevel, "L1") || 0 == strcasecmp(inputParam->cProductLevel, "L2") )
	{
		if(0 == strcasecmp(inputParam->Detector, "HSCO2") || 0 == strcasecmp(inputParam->Detector, "MULSS") )
		{
			retValue = xmlConfig.GetValue("HSCO2_L0_MODE", inputParam->HSCO2L0_DF);
			if(retValue != 0)
			{
				outputParam->iReturnCode = PARSE_INPUTXML_ERR;
				strcpy(outputParam->cReturnDescription, "Fail to get HSCO2_L0_MODE from input XML!");
				strcpy(outputParam->cErrorDescription, "Fail to get HSCO2_L0_MODE from input XML!");
				return PARSE_INPUTXML_ERR;
			}
			strcpy(tanfilename->HSCO2_L0_DF, inputParam->HSCO2L0_DF);
		}
		if(0 == strcasecmp(inputParam->Detector, "CAPIX") || 0 == strcasecmp(inputParam->Detector, "CAPI") ||0 == strcasecmp(inputParam->Detector, "MULSS") )
		{
			retValue = xmlConfig.GetValue("CAPI_L0_MODE", inputParam->CAPIL0_DF);
			if(retValue != 0)
			{
				outputParam->iReturnCode = PARSE_INPUTXML_ERR;
				strcpy(outputParam->cReturnDescription, "Fail to get CAPI_L0_MODE from input XML!");
				strcpy(outputParam->cErrorDescription, "Fail to get CAPI_L0_MODE from input XML!");
				return PARSE_INPUTXML_ERR;
			}
			strcpy(tanfilename->CAPI_L0_DF, inputParam->CAPIL0_DF);
		}
	}	
	char *FileName;
	
	//打印解析xml获取的文件名
	printf("tanfilename->CAPI_L0_DF:%s\n",tanfilename->CAPI_L0_DF);
	if(access(tanfilename->CAPI_L0_DF,F_OK) != 0)
	{
		sprintf(strlog,"Error: %s is not exist",tanfilename->CAPI_L0_DF);
		printf("%s!\n",strlog);
		log->WriteLogFile(strlog,-1);
		strcpy(outputParam->cReturnDescription,"Read data fialure");
		strcpy(outputParam->cErrorDescription,strlog);
		outputParam->iReturnCode = READ_AUXFILE_ERROR;
		return READ_AUXFILE_ERROR;
	}	
	
	
	//从L0数据描述记录区中获取起始扫描轨道号	
	unsigned int *orbtnum = (unsigned int*)calloc(2,sizeof(unsigned int));
	unsigned int startorbtnum = 0;
	unsigned int stoporbtnum = 0;
	FILE *p = fopen(tanfilename->CAPI_L0_DF,"rb");
	if(p == NULL)
	{
		printf("CAPI L0 file is null\n");
		return READ_AUXFILE_ERROR;
	}
	//获取起始、结束扫描轨道号
	fseek(p,8,0);
	fread(orbtnum,4,2,p);
	startorbtnum = reverseuint(orbtnum[0]);
	stoporbtnum = reverseuint(orbtnum[1]);
	
	printf("L1Afilename_startorbitnum : %d\n",startorbtnum);
	
	//输出5位十进制整数 左边补0
	sprintf(inputParam->orbitnumber,"%05d",startorbtnum);
	free(orbtnum);
	fclose(p);
	
	//sprintf(inputParam->orbitnumber,"05783");
	printf("inputParam->orbitnumber: %s\n",inputParam->orbitnumber);

	char tempBuff[256];
	sprintf(tempBuff, "%s/HSCO2", inputParam->ConfigDIR);
	strcpy(inputParam->HSCO2ConfigDIR, tempBuff);
	sprintf(tempBuff, "%sCAPI/PGS", inputParam->ConfigDIR);
	strcpy(inputParam->CAPIConfigDIR, tempBuff);
	//by wyg
	strcpy(tanfilename->CAPIConfigDIR, inputParam->CAPIConfigDIR);

	//get config filename
	sprintf(tanfilename->Filename_cfg, "%s%sPUBLIC%s%s", inputParam->ConfigDIR,DIR_CHAR,DIR_CHAR, FILE_NAME_CFG);
	
	//打印解析xml获取的静态配置文件文件名
	printf("tanfilename->Filename_cfg:%s\n",tanfilename->Filename_cfg);
	if(access(tanfilename->Filename_cfg,F_OK) != 0)  //config   added by liwei @20170119
	{
		sprintf(strlog,"Error: %s is not exist",tanfilename->Filename_cfg);
		printf("%s!\n",strlog);
		log->WriteLogFile(strlog,-1);
		strcpy(outputParam->cErrorDescription,"Read data fialure");
		strcpy(outputParam->cErrorDescription,strlog);
		outputParam->iReturnCode = READ_CFG_FILE_ERR;
		return READ_CFG_FILE_ERR;
	}
	
	//打开文件名配置文件
	PList plist1;
	retValue = plist1.MakeList(tanfilename->Filename_cfg);
	if(retValue != 1)
	{
				outputParam->iReturnCode = READ_CFG_FILE_ERR;
				strcpy(outputParam->cReturnDescription,"READ_CFG_FILE_ERR");
				strcpy(outputParam->cErrorDescription,"READ_CFG_FILE_ERR");
				return READ_CFG_FILE_ERR;
	//printf("**278**\n");
	}
    //printf("**280**\n");
	if(plist1.GetValue("HSCO2_CalibrationVer",tanfilename->HSCO2_CalibrationVer) != 1)
	{
			strcpy(tanfilename->HSCO2_CalibrationVer,"V00");
			log->WriteLogFile("READ_Filenamecfg_HSCO2_CalibrationVer_FAILED!\n",-1);
			strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_CalibrationVer_FAILED");
	}
	if(plist1.GetValue("CAPI_CalibrationVer",tanfilename->CAPI_CalibrationVer) != 1)
	{
			strcpy(tanfilename->CAPI_CalibrationVer,"V00");
			log->WriteLogFile("READ_Filenamecfg_CAPI_CalibrationVer_FAILED!\n",-1);
			strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_CalibrationVer_FAILED!");
	}
	int start, stop;
	
	//printf("**294**\n");
	
	if(0 == strcasecmp(inputParam->cProductLevel, "L1")||0 == strcasecmp(inputParam->cProductLevel, "L2"))
	{
			if(plist1.GetValue("L0NameDateStartBit",&start,1) != 1)
			{
					start= 10;
					log->WriteLogFile("READ_Filenamecfg_L0NameDateStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L0NameDateStartBit_FAILED!");
			}
			if(plist1.GetValue("L0NameDateStopBit",&stop,1) != 1)
			{
					stop= 10;
					log->WriteLogFile("READ_Filenamecfg_L0NameDateStopBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L0NameDateStopBit_FAILED!");
			}
			// pick up information from L0 Name
			if(0 == strcasecmp(inputParam->Detector, "HSCO2") || 0 == strcasecmp(inputParam->Detector, "MULSS"))
			{
				iLength = strlen(inputParam->HSCO2L0_DF);
				FileName = strrchr(inputParam->HSCO2L0_DF, '/');// after this code, tempBuff="/***(L0Filename).DAT"
			}
			else
			{
				iLength = strlen(inputParam->CAPIL0_DF);
				FileName = strrchr(inputParam->CAPIL0_DF, '/');// after this code, tempBuff="/***(L0Filename).DAT"
			}
			char *buf= new char[iLength];
			memset(buf, 0, iLength);
			iLength = strlen(FileName);
			if(NULL != FileName)
			{
				strcpy(buf, FileName);
			}
			//get observe date from HSCO2 L0 filename
			int i, j=0;
			for(i=start+1; i< stop+2 ; i++)
			{
						if(i < iLength)
						{
							tanfilename->DataAcquireDate[j] = buf[i];
							j++;
						}
			}
			//get observe time from HSCO2 L0 filename
			if(plist1.GetValue("L0NameTimeStartBit",&start,1) != 1)
			{
					start= 10;
					log->WriteLogFile("READ_Filenamecfg_L0NameTimeStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L0NameTimeStartBit_FAILED!");
			}
			if(plist1.GetValue("L0NameTimeStopBit",&stop,1) != 1)
			{
					stop= 10;
					log->WriteLogFile("READ_Filenamecfg_L0NameTimeStopBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L0NameTimeStopBit_FAILED!");
			}
			j=0;
			for(i=start+1; i< stop+2 ; i++)
			{
						if( i < iLength)
						{
							tanfilename->DataAcquireTime[j] = buf[i];
							j++;
						}
			}
			//get observe MODE from HSCO2 L0 filename
			if(plist1.GetValue("ModeStartBit",&start,1) != 1)
			{
					start= 10;
					log->WriteLogFile("READ_Filenamecfg_ModeStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_ModeStartBit_FAILED!");
			}
			j=0;
			for(i=start+1; i< start +3 ; i++)
			{
						if( i < iLength)
						{
							tanfilename->Mode[j] = buf[i];
							j++;
						}
			}
			
			
			//get StartOrbitNum from HSCO2 L0 FILE name
			if(plist1.GetValue("StartOrbitNumStartBit",&start,1) != 1)
			{
					start= 10;
					log->WriteLogFile("READ_Filenamecfg_StartOrbitNumStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_StartOrbitNumStartBit_FAILED!");
			}
			j=0;
			for(i=start+1; i< start +6 ; i++)
			{
						if( i < iLength)
						{
							tanfilename->StartOrbitNum[j] = buf[i];
							j++;
						}
			}
			//get StopOrbitNum from HSCO2 L0 FILE name
			if(plist1.GetValue("StopOrbitNumStartBit",&start,1) != 1)
			{
					start= 10;
					log->WriteLogFile("READ_Filenamecfg_StopOrbitNumStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_StopOrbitNumStartBit_FAILED!");
			}
			j=0;
			for(i=start+1; i< start +6 ; i++)
			{
						if( i < iLength)
						{
							tanfilename->StopOrbitNum[j] = buf[i];
							j++;
						}
			}
			delete [] buf;
	}
	else
	{
			if(plist1.GetValue("L3MonthStartBit",&start,1) != 1)
			{
					start= 7;
					log->WriteLogFile("READ_Filenamecfg_L3MonthStartBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L3MonthStartBit_FAILED!");
			}
			if(plist1.GetValue("L3MonthStopBit",&stop,1) != 1)
			{
					stop= 14;
					log->WriteLogFile("READ_Filenamecfg_L3MonthStopBit_FAILED!\n",-1);
					strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_L3MonthStopBit_FAILED!");
			}
			iLength = strlen(inputParam->L3INPUTLIST_DF);
			FileName = strrchr(inputParam->L3INPUTLIST_DF, '/');
			char *buf= new char[iLength];
			memset(buf, 0, iLength);
			iLength = strlen(FileName);
			if(NULL != FileName)
			{
				strcpy(buf, FileName);
			}
			int j=0;
			for(int i=start+1; i< stop+2 ; i++)
			{
						if( i < iLength)
						{
							tanfilename->L3month[j] = buf[i];
							j++;
						}
			}
			delete [] buf;
	}
	
	
	//printf("**445**\n");
	/****************************************  form  HSCO2  FILES NAME  START ****************************************************/
	if(0 == strcasecmp(inputParam->Detector, "HSCO2") || 0 == strcasecmp(inputParam->Detector, "MULSS"))
	{
			if(0 != strcasecmp(inputParam->cProductLevel, "L3"))
			{
					//form HSCO2 L1A filename
					if(plist1.GetValue("HSCO2_L1A.resolution",tanfilename->HSCO2resolution) != 1)
					{
							strcpy(tanfilename->HSCO2resolution,"1X2KM");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1A.resolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1A.resolution_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L1A.Version",tanfilename->Version) != 1)
					{
							strcpy(tanfilename->Version,"NUL");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1A.Version_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1A.Version_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L1A.Channel",tanfilename->Channel) != 1)
					{
							strcpy(tanfilename->Channel,"MLT");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1A.Channel_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1A.Channel_FAILED!");
					}
					if(0 == strcasecmp(&(tanfilename->Mode[1]), "P"))
					{
						strcpy(tanfilename->DataName,"PIX");
					}
					else
					{
						strcpy(tanfilename->DataName,"SAM");
					}
					sprintf(tanfilename->IFL_HSCO2_L1A_HDF, "%s_HSCO2_1A_%s_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF", inputParam->cSatID,tanfilename->DataName,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
															tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer);
					sprintf(tanfilename->IFL_HSCO2_L1A_JPG, "%s_HSCO2_1A_%s_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->DataName,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
							tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->CO2L1AHDF_DF, "%s/HSCO2/%s",inputParam->L1ProductPath,tanfilename->IFL_HSCO2_L1A_HDF);
					sprintf(tanfilename->CO2L1AHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L1A_HDF);
					sprintf(tanfilename->CO2L1AJPG_DF, "%s/HSCO2/%s",inputParam->L1ProductPath,tanfilename->IFL_HSCO2_L1A_JPG);
					sprintf(tanfilename->CO2L1AJPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L1A_JPG);

					//form HSCO2 L1B filename
					if(plist1.GetValue("HSCO2_L1B.resolution",tanfilename->HSCO2resolution) != 1)
					{
							strcpy(tanfilename->HSCO2resolution,"1X2KM");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1B.resolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1B.resolution_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L1B.Version",tanfilename->Version) != 1)
					{
							strcpy(tanfilename->Version,"NUL");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1B.Version_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1B.Version_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L1B.Channel",tanfilename->Channel) != 1)
					{
							strcpy(tanfilename->Channel,"MLT");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L1B.Channel_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L1B.Channel_FAILED!");
					}
						
					
					tempBuff[0] =tanfilename->Mode[0];
					tempBuff[1] = '\0';
					if((0 == strcasecmp(tempBuff, "N"))||(0 == strcasecmp(tempBuff, "G"))||(0 == strcasecmp(tempBuff, "T"))||(0 == strcasecmp(tempBuff, "H")))
					{
						strcpy(tanfilename->DataName,"SCI");
					}
					else
					{
						strcpy(tanfilename->DataName,"CAL");
					}
					sprintf(tanfilename->IFL_HSCO2_L1B_HDF, "%s_HSCO2_1B_%s_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF", inputParam->cSatID,tanfilename->DataName,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
																tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer);
					sprintf(tanfilename->IFL_HSCO2_L1B_JPG, "%s_HSCO2_1B_%s_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->DataName,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
								tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->CO2L1BHDF_DF, "%s/HSCO2/%s",inputParam->L1ProductPath,tanfilename->IFL_HSCO2_L1B_HDF);
					sprintf(tanfilename->CO2L1BHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L1B_HDF);
					sprintf(tanfilename->CO2L1BJPG_DF, "%s/HSCO2/%s",inputParam->L1ProductPath,tanfilename->IFL_HSCO2_L1B_JPG);
					sprintf(tanfilename->CO2L1BJPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L1B_JPG);
			}
			//form HSCO2 L2 filename
			if(0 == strcasecmp(inputParam->cProductLevel, "L2") && 0 == strcasecmp(tanfilename->DataName, "SCI"))
			{

					if(plist1.GetValue("HSCO2_L2.resolution",tanfilename->HSCO2resolution) != 1)
					{
							strcpy(tanfilename->HSCO2resolution,"1X2KM");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L2.resolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L2.resolution_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L2.Version",tanfilename->Version) != 1)
					{
							strcpy(tanfilename->Version,"NUL");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L2.Version_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L2.Version_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L2.Channel",tanfilename->Channel) != 1)
					{
							strcpy(tanfilename->Channel,"MLT");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L2.Channel_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L2.Channel_FAILED!");
					}
					sprintf(tanfilename->IFL_HSCO2_L2_CO2_HDF, "%s_HSCO2_L2_C02_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF", inputParam->cSatID,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
																tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer);
					sprintf(tanfilename->IFL_HSCO2_L2_CO2_JPG, "%s_HSCO2_L2_CO2_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
								tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->IFL_HSCO2_L2_O2A_JPG, "%s_HSCO2_L2_O2A_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->Mode,tanfilename->HSCO2resolution,tanfilename->StartOrbitNum,
									tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->HSCO2_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->CO2L2HDF_DF, "%s/HSCO2/CO2/%s",inputParam->L2ProductPath,tanfilename->IFL_HSCO2_L2_CO2_HDF);
					sprintf(tanfilename->CO2L2HDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L2_CO2_HDF);
					sprintf(tanfilename->CO2L2JPG_DF, "%s/HSCO2/CO2/%s",inputParam->L2ProductPath,tanfilename->IFL_HSCO2_L2_CO2_JPG);
					sprintf(tanfilename->CO2L2JPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L2_CO2_JPG);
					sprintf(tanfilename->O2AL2JPG_DF, "%s/HSCO2/CO2/%s",inputParam->L2ProductPath,tanfilename->IFL_HSCO2_L2_O2A_JPG);
					sprintf(tanfilename->O2AL2JPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L2_O2A_JPG);
			}
			//form HSCO2 L3 filename
			if(0 == strcasecmp(inputParam->cProductLevel, "L3"))
			{
					if(plist1.GetValue("HSCO2_L3.resolution",tanfilename->HSCO2resolution) != 1)
					{
							strcpy(tanfilename->HSCO2resolution,"2P5DE");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L3.resolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L3.resolution_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L3.Version",tanfilename->Version) != 1)
					{
							strcpy(tanfilename->Version,"GLL");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L3.Version_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L3.Version_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L3.Channel",tanfilename->Channel) != 1)
					{
							strcpy(tanfilename->Channel,"MLT");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L3.Channel_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L3.Channel_FAILED!");
							
					}
					if(plist1.GetValue("HSCO2_L3.AreaType",tanfilename->AreaType) != 1)
					{
							strcpy(tanfilename->AreaType,"GBAL");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L3.AreaType_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L3.AreaType_FAILED!");
					}
					if(plist1.GetValue("HSCO2_L3.ProductDec",tanfilename->ProductDec) != 1)
					{
							strcpy(tanfilename->ProductDec,"POAMX");
							log->WriteLogFile("READ_Filenamecfg_HSCO2_L3.ProductDec_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_HSCO2_L3.ProductDec_FAILED!");
					}
					sprintf(tanfilename->IFL_HSCO2_L3_CO2_HDF, "%s_HSCO2_L3_C02_ML_%s_%s_%s_%s_240000_%s_%s.HDF", inputParam->cSatID,tanfilename->HSCO2resolution,tanfilename->AreaType,tanfilename->ProductDec,
							tanfilename->L3month,tanfilename->Version,tanfilename->HSCO2_CalibrationVer);
					sprintf(tanfilename->IFL_HSCO2_L3_CO2_JPG, "%s_HSCO2_L3_CO2_ML_%s_%s_%s_%s_240000_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->HSCO2resolution,tanfilename->AreaType,tanfilename->ProductDec,
							tanfilename->L3month,tanfilename->Version,tanfilename->HSCO2_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->CO2L3HDF_DF, "%s/HSCO2/CO2/%s",inputParam->L3ProductPath,tanfilename->IFL_HSCO2_L3_CO2_HDF);
					sprintf(tanfilename->CO2L3HDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L3_CO2_HDF);
					sprintf(tanfilename->CO2L3JPG_DF, "%s/HSCO2/CO2/%s",inputParam->L3ProductPath,tanfilename->IFL_HSCO2_L3_CO2_JPG);
					sprintf(tanfilename->CO2L3JPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_HSCO2_L3_CO2_JPG);
			}
	}//0 == strcasecmp(inputParam->Detector, "HSCO2") || 0 == strcasecmp(inputParam->Detector, "MULSS")

	/****************************************  form  HSCO2  FILES NAME  END ****************************************************/
	//printf("**605**\n");
	/****************************************  form  CAPI  FILES NAME  START ****************************************************/
	if(0 == strcasecmp(inputParam->Detector, "CAPIX") || 0 == strcasecmp(inputParam->Detector, "MULSS"))
	{
			if(0 != strcasecmp(inputParam->cProductLevel, "L3"))
			{
						//form CAPI L1A filename
						if(plist1.GetValue("CAPI_L1A.resolution",tanfilename->CAPIresolution) != 1)
						{
								strcpy(tanfilename->CAPIresolution,"1K250");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1A.CAPIresolution_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1A.CAPIresolution_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1A.JPGresolution",tanfilename->CAPIJPGresolution) != 1)
						{
								strcpy(tanfilename->CAPIJPGresolution,"1000M");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1A.CAPIJPGresolution_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1A.CAPIJPGresolution_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1A.Version",tanfilename->Version) != 1)
						{
								strcpy(tanfilename->Version,"NUL");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1A.Version_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1A.Version_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1A.Channel",tanfilename->Channel) != 1)
						{
								strcpy(tanfilename->Channel,"MLT");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1A.Channel_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1A.Channel_FAILED!");
						}
						if(0 == strcasecmp(&(tanfilename->Mode[1]), "P"))
						{
							strcpy(tanfilename->DataName,"PIX");
						}
						else
						{
							strcpy(tanfilename->DataName,"SAM");
						}
						sprintf(tanfilename->IFL_CAPI_L1A_HDF, "TAN1_CAPIX_1A_%s_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->DataName,tanfilename->Mode,tanfilename->CAPIresolution,inputParam->orbitnumber,
																tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
						sprintf(tanfilename->IFL_CAPI_L1A_JPG, "%s_CAPIX_1A_%s_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG", inputParam->cSatID,tanfilename->DataName,tanfilename->Mode,tanfilename->CAPIJPGresolution,tanfilename->StartOrbitNum,
								tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer,tanfilename->Channel);
						sprintf(tanfilename->CAPIL1AHDF_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1A_HDF);
						sprintf(tanfilename->CAPIL1AHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1A_HDF);
						sprintf(tanfilename->CAPIL1AJPG_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1A_JPG);
						sprintf(tanfilename->CAPIL1AJPG_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1A_JPG);

						//form CAPI L1B filename
						if(plist1.GetValue("CAPI_L1A.JPGresolution",tanfilename->CAPIJPGresolution) != 1)
						{
								strcpy(tanfilename->CAPIJPGresolution,"1000M");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1A.CAPIJPGresolution_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1A.CAPIJPGresolution_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1B.OBCresolution",tanfilename->CAPIOBCresolution) != 1)
						{
								strcpy(tanfilename->CAPIOBCresolution,"00000");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1B.CAPIOBCresolution_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1B.CAPIOBCresolution_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1B.CALresolution",tanfilename->CAPICALresolution) != 1)
						{
								strcpy(tanfilename->CAPIOBCresolution,"1K250");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1B.CALresolution_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1B.CALresolution_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1B.Version",tanfilename->Version) != 1)
						{
								strcpy(tanfilename->Version,"NUL");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1B.Version_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1B.Version_FAILED!");
						}
						if(plist1.GetValue("CAPI_L1B.Channel",tanfilename->Channel) != 1)
						{
								strcpy(tanfilename->Channel,"MLT");
								log->WriteLogFile("READ_Filenamecfg_CAPI_L1B.Channel_FAILED!\n",-1);
								strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L1B.Channel_FAILED!");
						}
						tempBuff[0] =tanfilename->Mode[0];
							
						tempBuff[1] = '\0';
						if((0 == strcasecmp(tempBuff, "N"))||(0 == strcasecmp(tempBuff, "G"))||(0 == strcasecmp(tempBuff, "T"))||(0 == strcasecmp(tempBuff, "H")))
						{
							strcpy(tanfilename->DataName,"SCI");
							sprintf(tanfilename->IFL_CAPI_L1B_250_HDF, "TAN1_CAPIX_1B_%s_%s_0250M_ORBT_%s_%s_%s_%s_%s.HDF", tanfilename->DataName,tanfilename->Mode,inputParam->orbitnumber,
																			tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->IFL_CAPI_L1B_250_GEO_HDF, "TAN1_CAPIX_1B_%s_%s_GEOQK_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->DataName,tanfilename->Mode,inputParam->orbitnumber,
																				tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->IFL_CAPI_L1B_1KM_HDF, "TAN1_CAPIX_1B_%s_%s_1000M_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->DataName,tanfilename->Mode,inputParam->orbitnumber,
																			tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->IFL_CAPI_L1B_1KM_GEO_HDF, "TAN1_CAPIX_1B_%s_%s_GEO1K_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->DataName,tanfilename->Mode,inputParam->orbitnumber,
																					tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->IFL_CAPI_L1B_JPG, "TAN1_CAPIX_1B_%s_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG",tanfilename->DataName,tanfilename->Mode,tanfilename->CAPIJPGresolution,inputParam->orbitnumber,
										tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer,tanfilename->Channel);
							sprintf(tanfilename->IFL_CAPI_L1B_OBC_HDF, "TAN1_CAPIX_1B_OBC_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->Mode,tanfilename->CAPIOBCresolution,inputParam->orbitnumber,
																					tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->CAPIL1B250HDF_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_250_HDF);
							sprintf(tanfilename->CAPIL1B250HDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_250_HDF);
							sprintf(tanfilename->CAPIL1B250GEOHDF_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_250_GEO_HDF);
							sprintf(tanfilename->CAPIL1B250GEOHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_250_GEO_HDF);
							sprintf(tanfilename->CAPIL1B1KMHDF_DF, "%sCAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_1KM_HDF);
							sprintf(tanfilename->CAPIL1B1KMHDF_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_1KM_HDF);
							sprintf(tanfilename->CAPIL1B1KMGEOHDF_DF, "%sCAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_1KM_GEO_HDF);
							sprintf(tanfilename->CAPIL1B1KMGEOHDF_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_1KM_GEO_HDF);

							sprintf(tanfilename->CAPIL1BOBCHDF_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_OBC_HDF);
							sprintf(tanfilename->CAPIL1BOBCHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_OBC_HDF);
							sprintf(tanfilename->CAPIL1BJPG_DF, "%sCAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_JPG);
							sprintf(tanfilename->CAPIL1BJPG_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_JPG);
							
							//判断L1 数据是否存在												
							if(access(tanfilename->CAPIL1B1KMHDF_DF,F_OK) != 0)  //L1B
							{
								sprintf(strlog,"Error: %s is not exist",tanfilename->CAPIL1B1KMHDF_DF);
								printf("%s!\n",strlog);
								log->WriteLogFile(strlog,-1);
								strcpy(outputParam->cReturnDescription,"Read data fialure");
								strcpy(outputParam->cErrorDescription,strlog);
								outputParam->iReturnCode = READ_INPUT_FILE_L1_ERR;
								return READ_INPUT_FILE_L1_ERR;
							}
							if(access(tanfilename->CAPIL1B1KMGEOHDF_DF,F_OK) != 0)
							{
								sprintf(strlog,"Error: %s is not exist",tanfilename->CAPIL1B1KMGEOHDF_DF);
								printf("%s!\n",strlog);
								log->WriteLogFile(strlog,-1);
								strcpy(outputParam->cReturnDescription,"Read data fialure");
								strcpy(outputParam->cErrorDescription,strlog);
								outputParam->iReturnCode = READ_INPUT_FILE_L1_ERR;
								return READ_INPUT_FILE_L1_ERR;
							}
												
							
							//将L1B 输入文件拷贝到工作目录下
							sprintf(tempBuff, "cp %s %s",  tanfilename->CAPIL1B1KMHDF_DF,tanfilename->CAPIL1B1KMHDF_DF_TEMP);		
							retValue = system(tempBuff);
							if(retValue != 0)
							{
									outputParam->iReturnCode = COPY_PRODUCT_FILE_FAILED;
									//strcpy(outputParam->cReturnDescription,"COPY_INPUT_L1B_PRODUCT_FILE_FAILED");
									strcpy(outputParam->cErrorDescription,"COPY_INPUT_L1B_PRODUCT_FILE_FAILED!");
									//return COPY_PRODUCT_FILE_FAILED;
							}							
						
						
							sprintf(tempBuff, "cp %s %s",  tanfilename->CAPIL1B1KMGEOHDF_DF,tanfilename->CAPIL1B1KMGEOHDF_DF_TEMP);		
							retValue = system(tempBuff);
							if(retValue != 0)
							{
									outputParam->iReturnCode = COPY_PRODUCT_FILE_FAILED;
									//strcpy(outputParam->cReturnDescription,"COPY_INPUT_L1B_PRODUCT_FILE_FAILED");
									strcpy(outputParam->cErrorDescription,"COPY_INPUT_L1B_PRODUCT_FILE_FAILED!");
									//return COPY_PRODUCT_FILE_FAILED;
							}								
						
						
						}
						else
						{
							strcpy(tanfilename->DataName,"CAL");
							sprintf(tanfilename->IFL_CAPI_L1B_CAL_HDF, "TAN1_CAPIX_1B_CAL_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->Mode,tanfilename->CAPICALresolution,inputParam->orbitnumber,
																						tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->Version,tanfilename->CAPI_CalibrationVer);
							sprintf(tanfilename->CAPIL1BCALHDF_DF, "%s/CAPI/%s",inputParam->L1ProductPath,tanfilename->IFL_CAPI_L1B_CAL_HDF);
							sprintf(tanfilename->CAPIL1BCALHDF_DF_TEMP, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L1B_CAL_HDF);
						}
			}//if(0 != strcasecmp(inputParam->cProductLevel, "L3"))
				if(0 == strcasecmp(tanfilename->DataName, "SCI"))
				{
					//form CAPI L2 filename
					if(plist1.GetValue("CAPI_L2.AERresolution",tanfilename->AERresolution) != 1)
					{
							strcpy(tanfilename->AERresolution,"1000M");
							log->WriteLogFile("READ_Filenamecfg_CAPI_L2.AERresolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L2.AERresolution_FAILED!");
					}
					if(plist1.GetValue("CAPI_L2.CLMresolution",tanfilename->CLMresolution) != 1)
					{
							strcpy(tanfilename->CLMresolution,"1000M");
							log->WriteLogFile("READ_Filenamecfg_CAPI_L2.CLMresolution_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L2.CLMresolution_FAILED!");
					}
					if(plist1.GetValue("CAPI_L2.AERVersion",tanfilename->AERVersion) != 1)
					{
							strcpy(tanfilename->AERVersion,"NUL");
							log->WriteLogFile("READ_Filenamecfg_CAPI_L2.AERVersion_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L2.AERVersion_FAILED!");
					}
					if(plist1.GetValue("CAPI_L2.CLMVersion",tanfilename->CLMVersion) != 1)
					{
							strcpy(tanfilename->CLMVersion,"NUL");
							log->WriteLogFile("READ_Filenamecfg_CAPI_L2.CLMVersion_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L2.CLMVersion_FAILED!");
					}
					if(plist1.GetValue("CAPI_L2.Channel",tanfilename->Channel) != 1)
					{
							strcpy(tanfilename->Channel,"MLT");
							log->WriteLogFile("READ_Filenamecfg_CAPI_L2.Channel_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_CAPI_L2.Channel_FAILED!");
					}
					
				
					
					
					sprintf(tanfilename->IFL_CAPI_L2_AER_HDF, "TAN1_CAPIX_L2_AER_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->Mode,tanfilename->AERresolution,inputParam->orbitnumber,
																tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->AERVersion,tanfilename->CAPI_CalibrationVer);
					sprintf(tanfilename->IFL_CAPI_L2_AER_JPG, "TAN1_CAPIX_L2_AER_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG",tanfilename->Mode,tanfilename->AERresolution,inputParam->orbitnumber,
								tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->AERVersion,tanfilename->CAPI_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->IFL_CAPI_L2_CLM_HDF, "TAN1_CAPIX_L2_CLM_%s_%s_ORBT_%s_%s_%s_%s_%s.HDF",tanfilename->Mode,tanfilename->CLMresolution,inputParam->orbitnumber,
									tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->CLMVersion,tanfilename->CAPI_CalibrationVer);
					sprintf(tanfilename->IFL_CAPI_L2_CLM_JPG, "TAN1_CAPIX_L2_CLM_%s_%s_ORBT_%s_%s_%s_%s_%s_%s.JPG",tanfilename->Mode,tanfilename->CLMresolution,inputParam->orbitnumber,
										tanfilename->DataAcquireDate,tanfilename->DataAcquireTime,tanfilename->CLMVersion,tanfilename->CAPI_CalibrationVer,tanfilename->Channel);
					sprintf(tanfilename->AERL2HDF_DF, "%sCAPI/AER/%s",inputParam->L2ProductPath,tanfilename->IFL_CAPI_L2_AER_HDF);
					sprintf(tanfilename->AERL2HDF_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L2_AER_HDF);
					sprintf(tanfilename->AERL2JPG_DF, "%sCAPI/AER/%s",inputParam->L2ProductPath,tanfilename->IFL_CAPI_L2_AER_JPG);
					sprintf(tanfilename->AERL2JPG_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L2_AER_JPG);
					sprintf(tanfilename->CLML2HDF_DF, "%sCAPI/CLM/%s",inputParam->L2ProductPath,tanfilename->IFL_CAPI_L2_CLM_HDF);
					sprintf(tanfilename->CLML2HDF_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L2_CLM_HDF);
					sprintf(tanfilename->CLML2JPG_DF, "%sCAPI/CLM/%s",inputParam->L2ProductPath,tanfilename->IFL_CAPI_L2_CLM_JPG);
					sprintf(tanfilename->CLML2JPG_DF_TEMP, "%s%s",inputParam->WORKDIR,tanfilename->IFL_CAPI_L2_CLM_JPG);

				}
	}//0 == strcasecmp(inputParam->Detector, "CAPIX") || 0 == strcasecmp(inputParam->Detector, "MULSS")

		/****************************************  form  CAPI  FILES NAME  END ****************************************************/
		//printf("**803**\n");
		/****************************************  form  CONFIG FILES NAME START ****************************************************/
		sprintf(tanfilename->HSCO2ARP,"%s/HSCO2/%s", inputParam->ConfigDIR,HSCO2_ARP);
		sprintf(tanfilename->CAPIARP,"%s/CAPI/%s",inputParam->ConfigDIR, CAPI_ARP);
        
		//打印解析XML获取的文件名
		//printf("tanfilename->HSCO2ARP:%s\n",tanfilename->HSCO2ARP);
		//printf("tanfilename->CAPIARP :%s\n",tanfilename->CAPIARP);
		
		
		char month[8];
		if(0 == strcasecmp(inputParam->cProductLevel, "L1")||0 == strcasecmp(inputParam->cProductLevel, "L2"))
		{
					int j=0;
					for(int i=0; i< 6 ; i++)
					{
						month[j] = tanfilename->DataAcquireDate[i];
						j++;
					}
					//定标结果文件路径和文件名(one file per month)
					sprintf(tanfilename->CALResult,"%s/%s%s.HDF",inputParam->ConfigDIR, CAL_Result,month);
					//卫星遥测数据原始路径和文件名(one file per day)
					sprintf(tanfilename->InstrumentDay_DF,"%s/%s%s.dat",inputParam->InputGPSDataPath, Instrument_Day_DF,tanfilename->DataAcquireDate);
					//GPS数据原始路径和文件名(one file per day)
					sprintf(tanfilename->GpsDay_DF,"%s/%s%s.dat",inputParam->InputGPSDataPath, Gps_Day_DF,tanfilename->DataAcquireDate);
					//气象数值预报路径和文件名
					char param[256];
					sprintf(param,"%sPredictTimes",inputParam->NWPType);
					if(plist1.GetValue(param,&tanfilename->NWPredictTimes,1) != 1)
					{
							tanfilename->NWPredictTimes = 2;
							log->WriteLogFile("READ_Filenamecfg_NWPredictTimes_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_NWPredictTimes_FAILED!");
					}
					sprintf(param,"%sph",inputParam->NWPType);
					if(plist1.GetValue(param,tanfilename->cNWPph) != 1)
					{
							strcpy(tanfilename->cNWPph,"00,12");
							log->WriteLogFile("READ_Filenamecfg_NWPph(predict hour)_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_NWPph(predict hour)_FAILED!");
					}
					sprintf(param,"%sprd",inputParam->NWPType);
					if(plist1.GetValue(param,&tanfilename->NWPprd,1) != 1)
					{
							tanfilename->NWPprd =3;
							log->WriteLogFile("READ_Filenamecfg_NWPprd(predict period)_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_NWPprd(predict period)_FAILED!");
					}
					sprintf(param,"%sname",inputParam->NWPType);
					if(plist1.GetValue(param,tanfilename->NWPname) != 1)
					{
							strcpy(tanfilename->NWPname,"gmf.639.*.grb2");  //T639 file name example
							log->WriteLogFile("READ_Filenamecfg_NWPname(filename)_FAILED!\n",-1);
							strcpy(outputParam->cErrorDescription,"READ_Filenamecfg_NWPname(filename)_FAILED!");
					}
					int taillength;
					char *tailchar;
					iLength = strlen(tanfilename->NWPname);
					tailchar = strrchr(tanfilename->NWPname, '*');
					taillength = strlen(tailchar);
					for(int i=0;i<taillength-1;i++)
					{
						tailchar[i]=tailchar[i+1];
					}
					tailchar[taillength-1]='\0';
					j=0;
					for(int i=0; i< iLength-taillength ; i++)
					{
							tanfilename->IFL_NWP[j] = tanfilename->NWPname[i];
							j++;
					}
					int Recievedatahour,predicthour,timetopredict,temphour;
					char tempchar[8],spredicthour[8],stimetopredict[8];
					// data acquire hour string(tempchar), int(Recievedatahour)
					tempchar[0]= tanfilename->DataAcquireTime[0];
					tempchar[1]= tanfilename->DataAcquireTime[1];
					tempchar[2]='\0';
					Recievedatahour = atoi(tempchar);
					//解析cNWPph string(spredicthour) to int(predicthour)
					int strLength = strlen(tanfilename->cNWPph);

					int count = 0;
					j=0;
					for(int i=0; i< strLength+1; i++)
					{
							if(tanfilename->cNWPph[i]=='[' || tanfilename->cNWPph[i] == ']')
							{
								continue;
							}
							if(tanfilename->cNWPph[i] != ',' && i != strLength)
							{
										tempchar[j] = tanfilename->cNWPph[i];
										j++;
							}
							else
							{
										tempchar[j] = '\0';
										temphour = atoi(tempchar);
										if(temphour <= Recievedatahour && count < tanfilename->NWPredictTimes)
										{
											predicthour = temphour;
											strcpy(spredicthour,tempchar);

										}
										else
										{
											break;
										}
										j=0;
										count++;
						}
					}
					count = 0;
					do
					{
						temphour = predicthour+count*tanfilename->NWPprd;

						count++;
					}while(temphour < Recievedatahour && count<8);
					timetopredict = predicthour+(count-1)*tanfilename->NWPprd;
					if(timetopredict<10)
					{
						sprintf(stimetopredict,"00%d",timetopredict);
					}
					else
					{
						sprintf(stimetopredict,"0%d",timetopredict);
					}

					sprintf(tanfilename->IFL_NWP,"%s%s%s%s%s",tanfilename->IFL_NWP, tanfilename->DataAcquireDate,spredicthour,stimetopredict,tailchar);
					sprintf(tanfilename->NWP_DF, "%s%s",inputParam->InputNWPDataPath,tanfilename->IFL_NWP);					
		//			sprintf(tanfilename->NWP_DFtemp, "%s/%s",inputParam->WORKDIR,tanfilename->IFL_NWP);
		
		
		
		}
		
		//printf("**979**\n");
		//打印解析inputxml所获取的T639文件名
		printf("tanfilename->NWP_DF:%s\n",tanfilename->NWP_DF);

			
		/*else if(0 == strcasecmp(inputParam->cProductLevel, "L3"))
		{
			int j=0;
			for(int i=0; i< 6 ; i++)
			{
				month[j] = tanfilename->L3month[i];
				j++;
			}
			//定标结果文件路径和文件名(one file per month)
			sprintf(tanfilename->CALResult,"%s/%s%s.HDF",inputParam->ConfigDIR, CAL_Result,month);
		}
		*/
		
		//判断T639文件是否存在
		/* if(access(tanfilename->NWP_DF,F_OK) != 0)   //T639
		{
			sprintf(strlog,"Error: %s is not exist",tanfilename->NWP_DF);
			printf("%s!\n",strlog);
			log->WriteLogFile(strlog,-1);
			strcpy(outputParam->cReturnDescription,"Read data fialure");
			strcpy(outputParam->cErrorDescription,strlog);
			outputParam->iReturnCode = READ_T639_FILE_ERR;
			return READ_T639_FILE_ERR;
		} */
		
		char PhyFile[256];
		strcpy(PhyFile,tanfilename->NWP_DF);
		int i = 0;
		while(access(PhyFile, F_OK))    //如果文件不存在，向前搜索
		{
			memcpy(strlog ,"Don't find T639 data: " ,sizeof(strlog) );
			strcat(strlog ,PhyFile );
			strcat(strlog ," ,find the data 12 hours before..." );
			printf("%s!\n",strlog);
			log->WriteLogFile(strlog,-1);
			
			prev12h(PhyFile);   //

			i++;
			if(i > 6) 
			{
				memcpy(strlog ,"Don't find T639 data: " ,sizeof(strlog) );
				strcat(strlog ,PhyFile );
				strcat(strlog ," ,process exit!" );
				printf("%s!\n",strlog);
				log->WriteLogFile(strlog,-1);
				strcpy(outputParam->cReturnDescription,"Read T639 data fialure");
			    strcpy(outputParam->cErrorDescription,strlog);
				
				return READ_T639_FILE_ERR;   
			}
		}
		
		strcpy(tanfilename->NWP_DF,PhyFile);
		printf("After find T639 PhyFile = %s\n",PhyFile);
		
	return SUCCESS;	    
}

unsigned int reverseuint(unsigned int a)
{
	union{
		unsigned int i;
		unsigned char c[4];
	}u,r;
	u.i=a;
	r.c[0] = u.c[3];
	r.c[1] = u.c[2];
	r.c[2] = u.c[1];
	r.c[3] = u.c[0];
	return r.i;
}
