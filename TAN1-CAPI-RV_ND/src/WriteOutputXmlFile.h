/***********************************************************************************************************
Copyright (c) 2016,航天503所
All rights reserved.
文件名：WriteOutputXmlFile.h
编写者：huyuan
编写日期：2016年1月
简要描述： 输出反馈给PP的outputXML
  
修改记录：
当前版本：1.0.0
**************************************************************************************************************/
#ifndef WRITEOUTPUTXMLFILE_H_
#define WRITEOUTPUTXMLFILE_H_
#include "TanOutIepStruct.h"
#include "TanFilename.h"

//写输出XML文件
int WriteOutputXmlFile(struct TDPOutputxml outputParam, char *cFileName,struct TanFileName *tanfilename);


//该模块只针对于CLM， added by liwei @20170623
int WriteCLMOutputXmlFile(struct TDPOutputxml outputParam, char *cFileName,struct TanFileName *tanfilename);


#endif /*WRITEOUTPUTXMLFILE_H_*/
