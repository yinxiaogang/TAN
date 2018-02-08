/***********************************************************************************************************
Copyright (c) 2008,航天503所
All rights reserved.
文件名：ReadInputXmlFile.h
编写者：汪红强
编写日期：2008年3月05日
简要描述： 输出反馈给PP的outputXML
  
修改记录：
当前版本：1.0.0
**************************************************************************************************************/
#ifndef READINPUTXMLFILE_H_
#define READINPUTXMLFILE_H_

#include "TanIepStruct.h"
#include "TanOutIepStruct.h"
#include "CWriteLog_TAN.h"
#include "StructDefine.h"
#include <bitset>
#include <tanhdf.h>

#include "TanFilename.h"

int ReadInputXmlFile(struct TDPInputxml *inputParam, char *xmlfileName, struct TDPOutputxml *outputParam,struct TanFileName *tanfilename,CWriteLog *log);

int reverseint(int a);

float reversefloat(float a);

short reverseshort(short a);

unsigned int reverseuint(unsigned int a);

long reverseframenum(unsigned char *inputframe);

/* void Destroydata(L1A_CAPI_HDF *l1a_capi_hdf);

//进行帧时间连续性、有效性检查，输出相应质检码
void FtimeQualityCheck(L1A_CAPI_HDF *l1a_capi_hdf,int *actualframes);

//进行帧号连续性检查
void FcountQualityCheck(L1A_CAPI_HDF *l1a_capi_hdf,int *actualframes);

//丢帧统计
void LostscanflagCheck(L1A_CAPI_HDF *l1a_capi_hdf,int *actualframes,struct TAN_CAPI_L1A_FILE_INFO *tanCAPIL1AFileInfo);

//辅助数据包丢帧检查
void LostscanflagAncillaryCheck(L1A_CAPI_HDF *l1a_capi_hdf,int *actualframes,struct TAN_CAPI_L1A_FILE_INFO *tanCAPIL1AFileInfo);

//读取质量码第bit_index位数据
//返回读出位的值
unsigned char Get_bit(unsigned long data, int bit_index);

unsigned char Get_bit(unsigned char data, int bit_index);

void Set_bit(unsigned long *quality, int bit_index, int value);

void Set_bit(unsigned char *quality, int bit_index, int value);

//bit_index 从最左边0位开始数
void Set_Multibit(unsigned long data, unsigned long setdata, int bit_index, int num); //bit_index 从最左边0位开始数 */


#endif /*READINPUTXMLFILE_H_*/
