/***********************************************************************************************************
 Copyright (c) 2008,航天503所
 All rights reserved.
 文件名：AuxProcess.cpp
 编写者：HUYUAN
 编写日期：2008年3月05日
 简要描述： 进行CCD载荷辅助数据处理类，
 目前支持对HJ1AB卫星的CCD、CCD1、CCD2

 修改记录：
 当前版本：1.0.0
 **************************************************************************************************************/
//#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>

#include "AuxProcess.h"
#include "RetrievalDef.h"
#include "WinOrLinux.h"
#include "OppRcErrorCode.h"
#include "timestring.h"
#include "tanhdf.h"
#include "mask_stru.h"
#include "ReadInputXmlFile.h"
#include "JPEG.h"
#include <bitset>
#define FILE_NAME_CFG        "Filename.cfg"
#include "PublicErrorCode.h"


using namespace FY3JPEG;  //ZJPEG所在的名字空间



float colorpalette[64][3] =
{
	{0,         0,       0.5625},
	{0,         0,       0.625 },
	{0,         0,       0.5875},
	{0,         0,       0.75  },
	{0,         0,       0.8125},
	{0,         0,       0.875 },
	{0,         0,       0.9375},
	{0,         0,       1     },
	{0,         0.0625,  1     },
	{0,         0.125,   1     },
	{0,         0.1875,  1     },
	{0,         0.25,  	 1     },
	{0,         0.3125,  1     },
	{0,         0.375,   1     },
	{0,         0.4375,  1     },
	{0,         0.5,  	 1     },
	{0,         0.5625,  1     },
	{0,         0.625,   1     },
	{0,         0.6875,  1     },
	{0,         0.75,  	 1     },
	{0,         0.8125,  1     },
	{0,         0.875,   1     },
	{0,         0.9375,  1     },
	{0,         1,  	 1     },
	{0.0625, 	1,  	 0.9375},
	{0.125, 	1,  	 0.875 },
	{0.1875, 	1,  	 0.8125},
	{0.25,      1,  	 0.75  },
	{0.3125, 	1,  	 0.6875},
	{0.375, 	1,  	 0.625 },
	{0.4375, 	1,  	 0.5625},
	{0.5, 	    1,  	 0.5   },
	{0.5625, 	1,  	 0.4375},
	{0.625, 	1,  	 0.375 },
	{0.6875, 	1,  	 0.3125},
	{0.75, 	    1,  	 0.25  },
	{0.8125, 	1,  	    0.1875},
	{0.875, 	1,  	    0.125 },
	{0.9375, 	1,  	    0.0625},
	{1, 	    1, 	        0    },
	{1, 	    0.9375, 	0    },
	{1, 	    0.875, 	    0    },
	{1, 	    0.8125, 	0    },
	{1, 	    0.75, 	    0    },
	{1, 	    0.6875, 	0    },
	{1, 	    0.625, 	    0    },
	{1, 	    0.5625, 	0    },
	{1, 	    0.5,    	0    },
	{1, 	    0.4375, 	0    },
	{1, 	    0.375,   	0    },
	{1, 	    0.3125, 	0    },
	{1, 	    0.25, 	    0    },
	{1, 	    0.1875, 	0    },
	{1, 	    0.125,   	0    },
	{1, 	    0.0625, 	0    },
	{1, 	    0, 	        0    },
	{0.9375, 	0, 	        0    },
	{0.875, 	0, 	        0    },
	{0.8125, 	0, 	        0    },
	{0.75, 	    0,   	    0    },
	{0.6875, 	0, 	        0    },
	{0.625, 	0, 	        0    },
	{0.5625, 	0,  	    0    },
	{0.5, 	    0, 	        0    }	
};







using namespace std;
using namespace FY3JPEG;  //ZJPEG所在的名字空间
/*******************************************************************
* 函 数 体：write_AerJpeg
* 函数功能：对气溶胶二级产品出浏览图
* 参数意义：1 inputfilename_aer_jpg：气溶胶浏览图文件名
            2 L2_capi_aer_hdf：气溶胶二级产品结构体
            3 L2_CLM_GLOB：

* 修改备注：对出图进行投影，增加边框，添加色标，对跨越经度180（-180）的情况考虑
* 修改时间：20160701
* 修 改 人：李伟

*******************************************************************/
int AuxProcess::write_AerJpeg(char *inputfilename_aer_jpg, L2_CAPIX_AER_HDF &L2_capi_aer_hdf, L2_CAPIX_CLM_HDF &L2_capi_clm_hdf2, L2_CLM_GlobalAttr &L2_CLM_GLOB)
{	
   
	int _SAMPLES = L2_CLM_GLOB.DataPixels[0];
	int _SCANS   = L2_CLM_GLOB.DataLines[0];
	
	float *Depth_IMAGE = new float[_SCANS*_SAMPLES];


	for(int i=0; i<_SCANS; i++) //added by liwei @20170628-拉开数据层次
	{
		for(int j=0; j<_SAMPLES; j++)
		{
			
			Depth_IMAGE[i*_SAMPLES+j] = L2_capi_aer_hdf.Data.Aerosol_Optical_Depth[i*_SAMPLES+j]*100.0;	
		}	
	}
	
	
	//定义最小最大的行列号
	int iMaxRow = 0;
	int iMaxCol = 0;
	
	float fMinLat = 90;
	float fMaxLat = -90;
	float fMinLon = 180;
	float fMaxLon = -180;
	float fDgreeLat = 0.01;
	float fDgreeLon = 0.01;
	
	int LINESIZE = 3;   //边框线条粗细
	
	
	
	//找出经纬度中的最大值和最小值
	fMinLon = 360;
	
	for(int i = 0; i<_SCANS*_SAMPLES; i++)
	{
		float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
		float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
		
		if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 360)
		{
		
			if(fLat <= fMinLat)  //记录纬度中的最小值
				fMinLat = fLat;	
				
			if(fLat >= fMaxLat)  //记录纬度中的最大值
				fMaxLat = fLat;	
			
			if(fLon <= fMinLon)  //记录经度中的最小值
				fMinLon = fLon;	
			
			if(fLon >= fMaxLon)  //记录经度中的最大值
				fMaxLon =fLon;				
		}			
	}
	
	
	/*   //因为此处用的是云检测的经纬度信息，已经对经度进行判断过
	
	//若经度跨越 -180和180度时，将负的经度转化为正值，以180度为基准进行转化
	
	if((fMaxLon-180) <= 0.001)
	{
		for(int i = 0; i<_SCANS*_SAMPLES; i++)
		{
			if(L2_capi_clm_hdf2.Geo.Longitude[i]<0)
			   
		        L2_capi_clm_hdf2.Geo.Longitude[i] = L2_capi_clm_hdf2.Geo.Longitude[i]+360;
		
		}
		
	
		//再重新查找最大最小值
		//找出经纬度中的最大值和最小值
		fMinLon = 360;
		
		for(int i = 0; i<_SCANS*_SAMPLES; i++)
		{
			float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
			float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
			
			if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 360)
			{
			
				if(fLat <= fMinLat)  //记录纬度中的最小值
					fMinLat = fLat;	
					
				if(fLat >= fMaxLat)  //记录纬度中的最大值
					fMaxLat = fLat;	
				
				if(fLon <= fMinLon)  //记录经度中的最小值
					fMinLon = fLon;	
				
				if(fLon >= fMaxLon)  //记录经度中的最大值
					fMaxLon =fLon;				
			}			
		}
		
	} */
	
	//根据经纬度最大最小值按照间隔计算最大行列号
    iMaxRow = ((fMaxLat-fMinLat)/fDgreeLat)+2;   
    iMaxCol = ((fMaxLon-fMinLon)/fDgreeLon)+2;
	
	
	
	
		
	//对经纬度进行投影 added by liwei @20170628
	
	//对经纬度进行投影
	float* CLMAfterProj = new float[iMaxRow*iMaxCol];       //投影后的矩阵
	float* CLMAfterProjadd = new float[iMaxRow*iMaxCol];    //投影到每个位置的像素和
	int* CLMAfterProjPicelnum = new int[iMaxRow*iMaxCol];   //设置一个矩阵，计算每个位置数值的个数
	
	for(int i=0; i<iMaxRow*iMaxCol; i++)    //初始化
	{   
		CLMAfterProj[i] = 9999;
        CLMAfterProjPicelnum[i] = 0;    
        CLMAfterProjadd[i] = 0;		
	}
	
	for(int i = 0; i<_SCANS*_SAMPLES; i++)
	{
		float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
		float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
		
		if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 360)
		{
			//进行投影
			float fRow = (fMaxLat - fLat)/fDgreeLat;
			float fCol = (fLon - fMinLon)/fDgreeLon;
			
			int iRow = int(fRow + 0.5);
			int iCol = int(fCol + 0.5);
		
			//CLMAfterProj[iRow*iMaxCol+iCol] = Depth_IMAGE[i];
			
			CLMAfterProjadd[iRow*iMaxCol+iCol] = CLMAfterProjadd[iRow*iMaxCol+iCol] + Depth_IMAGE[i];
            CLMAfterProjPicelnum[iRow*iMaxCol+iCol]	=  CLMAfterProjPicelnum[iRow*iMaxCol+iCol] + 1;  //计算投影到该格子内的像素个数	
		}			
	}
	
	
	//求每个格点的像素均值
	for(int i=0; i<iMaxRow; i++)
	{
		for(int j=0; j<iMaxCol; j++)
		{
			int index = 0;
			index = i*iMaxCol+j;
			if(CLMAfterProjPicelnum[index] != 0)
			{
				CLMAfterProj[index] = CLMAfterProjadd[index]/CLMAfterProjPicelnum[index];
			}	
		}
	}
	
	delete CLMAfterProjadd; CLMAfterProjadd=NULL;
	delete CLMAfterProjPicelnum; CLMAfterProjPicelnum=NULL;
	










	
	
	
	//设置一个大的变量，将出图数据和色标都放置进去
	int iAlliMaxCol = iMaxCol + 300;   //300为色标增加的列数
	int iAlliMaxRow = iMaxRow + 100;   //300为色标增加的列数   
	float* AllCLMAfterProj = new float[iAlliMaxRow*iAlliMaxCol];
	for(int i=0; i<iAlliMaxRow*iAlliMaxCol; i++)
	{
		AllCLMAfterProj[i] = 9999;	
	}
	
	//把投影后的数据 CLMAfterProj 放置到 AllCLMAfterProj 数据中,并向右移100,向下移 LINESIZE 
	for(int i=0; i<iMaxRow; i++)
	{
		for(int j=0; j<iMaxCol; j++)
		{
			AllCLMAfterProj[(LINESIZE+i)*iAlliMaxCol + j + 100] = CLMAfterProj[i*iMaxCol + j];	
		}
	}
	
	
	
	////////增加边框/////////    //added by liwei @20170630
	/**************
	左边框离左边界 100
	下边框离下边界 100
	**************/
	
	for(int i=0; i<iMaxRow+2*LINESIZE; i++)
	{
		for(int Linesize=0; Linesize<LINESIZE;Linesize++)    
		{
			AllCLMAfterProj[i*iAlliMaxCol+(100-LINESIZE)+Linesize] = -9999;           //左边框
			
			AllCLMAfterProj[i*iAlliMaxCol+(iMaxCol+100) +Linesize] = -9999;           //右边框
		}
	}
	
	
	for(int i=100; i<iMaxCol+100+LINESIZE; i++)
	{
		for(int Linesize=0; Linesize<LINESIZE;Linesize++)
		{
			AllCLMAfterProj[iAlliMaxCol*Linesize + i] = -9999;                //设置上边框
			
			AllCLMAfterProj[((iMaxRow+LINESIZE)+Linesize) * iAlliMaxCol + i] = -9999;         //设置下边框	
		}
	}

			
	delete CLMAfterProj; CLMAfterProj = NULL;
	
	
	
	//绘制色标图
	float nColorTableValue[64];
	for(int i=0; i<64; i++)
	{
		nColorTableValue[i] = 500-i*7.89;		//500为数据中的最大值
	}
	
	int iColorBarHeight = iMaxRow/64;     //色标中每个颜色条的高度（共64种颜色）
	int iColorBarWidth = 50;              //色标宽度
	int left_top_Y = 0;                   //色标和上边界距离
	int left_top_X = iAlliMaxCol-150;     //色标和左边界距离
	
	for(int i = 0; i < 64; i++)  //生成色标
	{
		for(int k = left_top_Y + iColorBarHeight * i; k < left_top_Y + iColorBarHeight * (i + 1); k++)
		{				
			for(int j = left_top_X; j < left_top_X + iColorBarWidth ; j++)	
			{
				AllCLMAfterProj[k * iAlliMaxCol + j] = nColorTableValue[i];				
			}
		}  
	}  
	
	
	
	//出浏览图

	int status = 0;

	JPEGInfo jpginfo;
	
	CPalette *palette = NULL;
   
	CJPEGDataF32 	CHs[3];
	
	char strOutJPG[200]; 
    char strThum[200]; 
    memset(strOutJPG, '\0', 200);
	memset(strThum, '\0', 200);
	strcpy(strOutJPG,inputfilename_aer_jpg);
	strcpy(strThum,inputfilename_aer_jpg);
	
	jpginfo.iHeight = iAlliMaxRow;
	
    jpginfo.iWidth = iAlliMaxCol;
	
    jpginfo.iQuality = 100;
	
	
	CJPEG jpegRGB(jpginfo, COLOR_RGB);
	
	CJPEG::JPEGPoint point;
	
	status = jpegRGB.Create(strOutJPG);
	if(status != 0)
	{
		printf("Creat JPG error!\n");	
	}
	else
	{
		printf("Creat JPG success!\n");	
	}
	
	
	CHs[0].AddFillValue(-999900.0);
	CHs[0].SetData(AllCLMAfterProj);
	CHs[0].SetRange(-6, 500);
	CHs[0].SetSlope(1);	
	CHs[1].SetData(NULL);
	
	palette = jpegRGB.GetPalette();
	/* palette->SetRange(-9999,1000); */
	palette->SetRange(-6,500);
	palette->SetSlope(1);
	
	
	
	
	//把调色板从0-1换算为1-255
	for(int i=0; i<64; i++)
		for(int j=0; j<3; j++)
			colorpalette[i][j] = colorpalette[i][j]*255;
	
	float mixcolor = -5;
	for(int i=0; i<64; i++)
	{
		float leftclor = mixcolor + i*7.89;     //(500+5)/64 = 7.89
		float rightcolor = leftclor + 7.89;
		palette->SetPaletteEntry(leftclor,rightcolor, colorpalette[i][0],colorpalette[i][1],colorpalette[i][2]);
	}
	
	//palette->SetPaletteEntry(-999900,-99990, 169,169,169);	//无效值 灰色
	palette->SetPaletteEntry(-999901,-9999,169,169,169);	//无效值 灰色
	palette->SetPaletteEntry(-9999,-6,255,255,255);	//边框 白色 
	palette->SetPaletteEntry(501, 9999,0,0,0);	        //底板 黑色
	

		
	
	jpegRGB.RGB(CHs);
	
	
	//添加文字
	wchar_t* strLatAnnotation2[12] = {L"5",L"4.5",L"4",L"3.5",L"3",L"2.5",L"2",L"1.5",L"1",L"0.5",L"0",L"-0.5"};
	
	left_top_X = iAlliMaxCol-85;   //文字距离左边界的距离
	
	left_top_Y=0;                  //文字距离上边界的距离
	
	iColorBarHeight = iMaxRow/64;  //每个颜色小块的高度（共64种颜色）   
	iColorBarHeight = iColorBarHeight*5.81;   //每两个字之间的距离（64/11 = 5.81）
	
	FY3JPEG::CJPEG::JPEG_POINT point2={left_top_X,left_top_Y};
	
	for(int i=0;i<12;i++)	
	{
		point2.y=left_top_Y + iColorBarHeight*i ; //+ iColorBarHeight/2.0;
		
		jpegRGB.Annotate(strLatAnnotation2[i], point2, CJPEG::JPEGHorizontal,30,WHITE, 100, "/gpfs3/TanOnLine/include/FZFangSong.ttf");
	}
	
	
	jpegRGB.WriteoutJPEG(); 
	jpegRGB.Dispose();
	
	printf("*********输出AER JPG成功！\n");
	
    delete Depth_IMAGE;	 Depth_IMAGE=NULL;
	delete AllCLMAfterProj; AllCLMAfterProj=NULL;
	return SUCCESS;
}


/*******************************************************************
* 函 数 体： write_ClmJpeg
* 函数功能： 对云检测二级产品出浏览图
* 参数意义：1 inputfilename_clm_jpg ：气溶胶浏览图文件名
            2 L2_capi_clm_hdf2 ：云检测二级产品结构体
            3 L2_CLM_GLOB：
* 修改备注：对出图进行投影，增加边框，添加色标，对跨越经度180（-180）的情况考虑
* 修改时间：20160701
* 修 改 人：李伟

*******************************************************************/

int AuxProcess::write_ClmJpeg(char *inputfilename_clm_jpg,L2_CAPIX_CLM_HDF &L2_capi_clm_hdf2,L2_CLM_GlobalAttr &L2_CLM_GLOB)
{	
    
	int _SAMPLES = L2_CLM_GLOB.DataPixels[0];
	int _SCANS   = L2_CLM_GLOB.DataLines[0];	
	
	//定义最小最大的行列好
	int iMaxRow = 0;
	int iMaxCol = 0;
	
	float fMinLat = 90;
	float fMaxLat = -90;
	float fMinLon = 180;
	float fMaxLon = -180;
	float fDgreeLat = 0.01;
	float fDgreeLon = 0.01;
	
	int LINESIZE = 3;   //Linesize:边框线条粗细
	
	
	
	//找出经纬度中的最大值和最小值
	for(int i = 0; i<_SCANS*_SAMPLES; i++)
	{
		float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
		float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
		
		if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 180)
		{
		
			if(fLat <= fMinLat)  //记录纬度中的最小值
				fMinLat = fLat;	
				
			if(fLat >= fMaxLat)  //记录纬度中的最大值
				fMaxLat = fLat;	
			
			if(fLon <= fMinLon)  //记录经度中的最小值
				fMinLon = fLon;	
			
			if(fLon >= fMaxLon)  //记录经度中的最大值
				fMaxLon =fLon;				
		}			
	}
	
	
	
	//若经度跨越 -180和180度时，将负的经度转化为正值，以180度为基准进行转化
	
	if(fMaxLon-180 <= 0.001)
	{
		for(int i = 0; i<_SCANS*_SAMPLES; i++)
		{
			if(L2_capi_clm_hdf2.Geo.Longitude[i]<0)
			   
		        L2_capi_clm_hdf2.Geo.Longitude[i] = L2_capi_clm_hdf2.Geo.Longitude[i]+360;
		
		}
		
	
		//再重新查找最大最小值
		//找出经纬度中的最大值和最小值
		fMinLon = 360;
		
		for(int i = 0; i<_SCANS*_SAMPLES; i++)
		{
			float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
			float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
			
			if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 360)
			{
			
				if(fLat <= fMinLat)  //记录纬度中的最小值
					fMinLat = fLat;	
					
				if(fLat >= fMaxLat)  //记录纬度中的最大值
					fMaxLat = fLat;	
				
				if(fLon <= fMinLon)  //记录经度中的最小值
					fMinLon = fLon;	
				
				if(fLon >= fMaxLon)  //记录经度中的最大值
					fMaxLon =fLon;				
			}			
		}
		
	}
	
	
	//根据经纬度最大最小值按照间隔为0.01计算最大行列号
    iMaxRow = ((fMaxLat-fMinLat)/fDgreeLat)+2;   
    iMaxCol = ((fMaxLon-fMinLon)/fDgreeLon)+2;
	

		
	//对经纬度进行投影 added by liwei @20170628
	
	//以间隔0.001对经纬度进行投影
	int* CLMAfterProj = new int[iMaxRow*iMaxCol];
	if(CLMAfterProj == NULL)
	{
		printf("Apply memory error!\n");
		return SYSTEM_APPLY_MEMORY_ERR;	
	}
	
	for(int i=0; i<iMaxRow*iMaxCol; i++)
	{
		CLMAfterProj[i] = -9999;	
	}
	
	
	for(int i = 0; i<_SCANS*_SAMPLES; i++)
	{
		float fLat = L2_capi_clm_hdf2.Geo.Latitude[i];
		float fLon = L2_capi_clm_hdf2.Geo.Longitude[i];
		
		if(fLat >= -90 && fLat <= 90 && fLon >= -180 && fLon <= 360)
		{
			//进行投影
			
			float fRow = (fMaxLat - fLat)/fDgreeLat;
			float fCol = (fLon - fMinLon)/fDgreeLon;
			
			
			int iRow = int(fRow + 0.5);
			int iCol = int(fCol + 0.5);
			
	
			CLMAfterProj[iRow*iMaxCol+iCol] = L2_capi_clm_hdf2.Data.Cloud_Mask[i];			
		}			
	}
		
	//云检测快视图
	typedef enum
	{//云检测快视图色标
		JPEG_FILLED = 0,		//0 - 填充
		JPEG_CLOUD,				//1 - 可信度高的云
		JPEG_PROB_CLOUD,		//2 - 可信度底的云
		JPEG_LAND_CLEAR,		//3 - 可信度低的晴空
        JPEG_LAND_PROB_CLEAR,    //4 - 可信度高的晴空
        JPEG_INVALID			//5 - 无效		
	}CLM_JPEG;	
	

	
	//设置一个大的变量，将出图数据和色标都放置进去
	int iAlliMaxCol = iMaxCol + 300;   //300为色标增加的列数
	int iAlliMaxRow = iMaxRow + 100;   //300为色标增加的列数   
	int* AllCLMAfterProj = new int[iAlliMaxRow*iAlliMaxCol];
	for(int i=0; i<iAlliMaxRow*iAlliMaxCol; i++)
	{
		AllCLMAfterProj[i] = -9999;	
	}
	
	//把投影后的数据 CLMAfterProj 放置到 AllCLMAfterProj 数据中,并向右移100,向下移 LINESIZE
	for(int i=0; i<iMaxRow+0; i++)
	{
		for(int j=0; j<iMaxCol; j++)
		{
			AllCLMAfterProj[(i+LINESIZE)*iAlliMaxCol + j + 100] = CLMAfterProj[i*iMaxCol + j];	
		}
	}
	

	
	////////增加边框/////////    //added by liwei @20170630
	/**************
	左边框离左边界 100
	下边框离下边界 100
	
	**************/
	
	for(int i=0; i<iMaxRow+2*LINESIZE; i++)
	{
		for(int Linesize=0; Linesize<LINESIZE;Linesize++)    
		{
			AllCLMAfterProj[i*iAlliMaxCol+(100-LINESIZE)+Linesize] = CLOUDY;           //左边框
			
			AllCLMAfterProj[i*iAlliMaxCol+(iMaxCol+100) +Linesize] = CLOUDY;           //右边框
		}
	}
	
	
	for(int i=100; i<iMaxCol+100+LINESIZE; i++)
	{
		for(int Linesize=0; Linesize<LINESIZE;Linesize++)
		{
			AllCLMAfterProj[iAlliMaxCol*Linesize + i] = CLOUDY;                //设置上边框
			
			AllCLMAfterProj[((iMaxRow+LINESIZE)+Linesize) * iAlliMaxCol + i] = CLOUDY;         //设置下边框	
		}
	}

	delete CLMAfterProj; CLMAfterProj = NULL;
	
	
	
	//绘制色标图
	int iColorBarHeight = iMaxRow/4;
	int iColorBarWidth = 50;
	int left_top_Y = 0;
	int left_top_X = iAlliMaxCol-150;
	
	const int nColorTableValue[4]=
	{
		CLOUDY, //JPEG_CLOUD,
		UNCERTAIN,//JPEG_PROB_CLOUD,
		PROBABLY_CLEAR,//JPEG_LAND_CLEAR,
		CONFIDENT_CLEAR,//JPEG_LAND_PROB_CLEAR,
	};
	
	for(int i = 0; i < 4; i++)  //生成色标
	{
		for(int k = left_top_Y + iColorBarHeight * i; k < left_top_Y + iColorBarHeight * (i + 1); k++)
		{				
			for(int j = left_top_X; j < left_top_X + iColorBarWidth ; j++)	
			{
				AllCLMAfterProj[k * iAlliMaxCol + j] = nColorTableValue[i];				
			}
		}  
	}  
	
	
	for(int i = 0; i < iAlliMaxRow*iAlliMaxCol; i++)
	{
		if (AllCLMAfterProj[i] == FILLED)//NOT_DETERMINED)
		{//像元无效
			AllCLMAfterProj[i] = JPEG_FILLED;
		} 
		else if (AllCLMAfterProj[i] == CLOUDY)
		{//云
			AllCLMAfterProj[i] = JPEG_CLOUD;
		}
		else if (AllCLMAfterProj[i] == UNCERTAIN)
		{//不确定的云
			AllCLMAfterProj[i] = JPEG_PROB_CLOUD;
		}
		else if (AllCLMAfterProj[i] == PROBABLY_CLEAR)
		{//不确定的晴空
			AllCLMAfterProj[i] = JPEG_LAND_CLEAR;
		}
		else if (AllCLMAfterProj[i] == CONFIDENT_CLEAR)
		{//可信度高的晴空
			AllCLMAfterProj[i] = JPEG_LAND_PROB_CLEAR;
		}
		else
		{//填充
			AllCLMAfterProj[i] = JPEG_INVALID;
		}
		
	}
	
	

	
	//出浏览图
	int status = 0;
	
	JPEGInfo jpginfo;

    CPalette *palette = NULL;
		
	CJPEGDataI32  CHs[3];
		
	char strOutJPG[200]; 
    char strThum[200]; 
    memset(strOutJPG, '\0', 200);
	memset(strThum, '\0', 200);
	strcpy(strOutJPG,inputfilename_clm_jpg);
	strcpy(strThum,inputfilename_clm_jpg);
		
	jpginfo.iHeight = iAlliMaxRow;
	
    jpginfo.iWidth = iAlliMaxCol;
	
	
    jpginfo.iQuality = 100;
	
	
	CJPEG jpegRGB(jpginfo, COLOR_RGB);
	
	CJPEG::JPEGPoint point;
	
	status = jpegRGB.Create(strOutJPG);
	if(status != 0)
	{
		printf("Creat JPG error!\n");	
	}
	else
	{
		printf("Creat JPG success!\n");	
	}
	
	
		
	CHs[0].AddFillValue(-9999);	//SDS的填充值
	CHs[0].SetData(AllCLMAfterProj);
	CHs[0].SetRange(0,255);	//SDS的最大值,小值: 							
	CHs[0].SetSlope(1);
	CHs[1].SetData(NULL);
	
	palette = jpegRGB.GetPalette();
	palette->SetRange(0,255);
	palette->SetSlope(1);
	
	palette->SetPaletteEntry(JPEG_CLOUD, 255, 255, 255);          //白色，云
	palette->SetPaletteEntry(JPEG_PROB_CLOUD, 169, 169, 169);     //灰色，可能的云
    palette->SetPaletteEntry(JPEG_LAND_CLEAR, 0, 128, 0);         //浅绿，可能的陆地晴空	
	palette->SetPaletteEntry(JPEG_LAND_PROB_CLEAR, 0, 255, 0);    //绿色，陆地晴空
	palette->SetPaletteEntry(JPEG_FILLED, 0, 0, 0);               //黑色，无效
	palette->SetPaletteEntry(JPEG_INVALID,  0, 0, 0);              //黑色，无效	
	
	
	
	jpegRGB.RGB(CHs);
	
	
	//添加文字
	wchar_t* strLatAnnotation2[4] = {L"0",L"1",L"2",L"3"};
	
	left_top_X = iAlliMaxCol-85;
	
	left_top_Y=0;
	
	FY3JPEG::CJPEG::JPEG_POINT point2={left_top_X,left_top_Y};
	
	for(int i=0;i<4;i++)	
	{
		point2.y=left_top_Y + iColorBarHeight*i + iColorBarHeight/2.0;
		
		jpegRGB.Annotate(strLatAnnotation2[i], point2, CJPEG::JPEGHorizontal,30,WHITE, 100, "/gpfs3/TanOnLine/include/FZFangSong.ttf");
	}
	

	
	jpegRGB.WriteoutJPEG();
	jpegRGB.Dispose();
	
	delete AllCLMAfterProj; AllCLMAfterProj=NULL;
	
	printf("*********输出CLMJPG成功！\n");
	
	
	
   	return SUCCESS;	
	
	
}

int AuxProcess::write_SciJpeg(char *inputfilename_aer_jpg,char *inputfilename_clm_jpg,L2_CAPIX_AER_HDF &L2_capi_aer_hdf2,L2_CAPIX_CLM_HDF &L2_capi_clm_hdf2)
{	
    //jpg of aerosol
	JPEGInfo Info_AER;
	Info_AER.iHeight = 2048;
	Info_AER.iWidth =1800;
	Info_AER.iQuality = 100;
 	CJPEG jpegRGB_aer(Info_AER ,COLOR_RGB);
 	jpegRGB_aer.Create(inputfilename_aer_jpg);
	
 	//R波段
 	CJPEGDataF32 CHs[3];
	CHs[0].AddFillValue(0);
	CHs[0].SetData(L2_capi_aer_hdf2.Geo.Latitude);
	CHs[0].SetRange(0, 255);
	CHs[0].SetSlope(1);

	//G波段
	CHs[1].AddFillValue(0);	
	CHs[1].SetData(L2_capi_aer_hdf2.Data.Aerosol_Optical_Depth);
	CHs[1].SetRange(0, 255);
	CHs[1].SetSlope(1);

	//B波段
	CHs[2].AddFillValue(0);	
	CHs[2].SetData(L2_capi_aer_hdf2.Data.Surface_Reflectance_670);
	CHs[2].SetRange(0, 255);
	CHs[2].SetSlope(1);
	
	jpegRGB_aer.RGB(CHs);	
	jpegRGB_aer.WriteoutJPEG();
	
    //jpg of clm
	/*  JPEGInfo Info_CLM;
	Info_CLM.iHeight = 2048;
	Info_CLM.iWidth =1800;
	Info_CLM.iQuality = 100;
 	CJPEG jpegRGB_clm(Info_CLM ,COLOR_RGB);
 	jpegRGB_clm.Create(inputfilename_clm_jpg);
	
 	//R波段
 	CJPEGDataI32 CHs1[1];
	CHs1[0].AddFillValue(0);
	CHs1[0].SetData(L2_capi_clm_hdf2.Data.Cloud_Mask);
	CHs1[0].SetRange(0, 255);
	CHs1[0].SetSlope(1); */

	/* //G波段
	CHs1[1].AddFillValue(0);	
	CHs1[1].SetData(L2_capi_clm_hdf2.Data.Confidence_Flag);
	CHs1[1].SetRange(0, 255);
	CHs1[1].SetSlope(1);

	//B波段
	
	CHs1[2].AddFillValue(0);	
	CHs1[2].SetData(L2_capi_clm_hdf2.Data.Quality_Flag);
	CHs1[2].SetRange(0, 255);
	CHs1[2].SetSlope(1); 
	
	jpegRGB_clm.RGB(CHs1);	
	jpegRGB_clm.WriteoutJPEG();*/
 
	return SUCCESS;
}


AuxProcess::AuxProcess()
{
	//l1b CAPI time(&time_start)
	l1b_CAPI.NonPolarization.Ref_038_QKM = NULL;
	l1b_CAPI.NonPolarization.Ref_087_QKM = NULL;
	l1b_CAPI.Polarization.Ref_067_I_QKM = NULL;
	l1b_CAPI.Polarization.Ref_067_Q_QKM = NULL;
	l1b_CAPI.Polarization.Ref_067_U_QKM = NULL;
	//Aacillary
	l1b_CAPI.Ancillary.frame_DarkCurrent_QKM = NULL;
	l1b_CAPI.Ancillary.frame_id_QKM = NULL;
	l1b_CAPI.Ancillary.frame_time_second_J2000 = NULL;
	l1b_CAPI.Ancillary.frame_qual_flag_QKM = NULL;
	l1b_CAPI.Ancillary.coef_Cal = NULL;
	l1b_CAPI.Ancillary.OpertionMode = NULL;


   //l2 CLM
	l2_CML.Data.Confidence_Flag = NULL;
	l2_CML.Data.Cloud_Mask = NULL;
	l2_CML.Data.Quality_Flag = NULL;

	l2_CML.Geo.Latitude = NULL;
	l2_CML.Geo.Longitude = NULL;


   //l2 AER
	l2_AER.Data.Aerosol_Optical_Depth = NULL;
	l2_AER.Data.Angstrom_Exponent = NULL;
	l2_AER.Data.Fine_Mode_Fraction = NULL;
	l2_AER.Data.Surface_Reflectance_670 = NULL;
	l2_AER.Data.Surface_Reflectance_870 = NULL;
	l2_AER.Data.Surface_Reflectance_1640 = NULL;
	l2_AER.Data.Quality_Flag = NULL;

	l2_AER.Geo.Latitude = NULL;
	l2_AER.Geo.Longitude = NULL;


//	//time(&time_start);
//	l1b_HSCO2.sounding_geometry.sounding_altitude = NULL;
//	l1b_HSCO2.sounding_geometry.sounding_land_fraction = NULL;
	RetrievalPonitNum = 200;
}

AuxProcess::~AuxProcess()
{
}

//public method 
void AuxProcess::init(RetrievalInputParam* inputXML, RetrievalOutputParam* outputXML,int* Warningcount)
{
	pInputParam = inputXML;
	pOutputParam = outputXML;
	WarnCount = Warningcount;
}

int AuxProcess::ApplyMemory(L2_CAPIX_AER_HDF &L2_aer_memory,L2_CAPIX_CLM_HDF &L2_clm_memory,L2_CLM_GlobalAttr &L2_GlobalAttr)
{
	// L2 AER of CAPI
	int DataPixels_size = L2_GlobalAttr.DataPixels[0];
	int DataLines_size = L2_GlobalAttr.DataLines[0];
	L2_aer_memory.Geo.Latitude                   = NULL;
	L2_aer_memory.Geo.Longitude                  = NULL;
	L2_aer_memory.Data.Aerosol_Optical_Depth     = NULL;
	L2_aer_memory.Data.Surface_Reflectance_670   = NULL;
    if(L2_aer_memory.Geo.Latitude == NULL)
		{
			if( (L2_aer_memory.Geo.Latitude = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}	
	if(L2_aer_memory.Geo.Longitude == NULL)
		{
			if( (L2_aer_memory.Geo.Longitude = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}	
    if(L2_aer_memory.Data.Aerosol_Optical_Depth == NULL)
		{
			if( (L2_aer_memory.Data.Aerosol_Optical_Depth = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}
    if(L2_aer_memory.Data.Surface_Reflectance_670 == NULL)
		{
			if( (L2_aer_memory.Data.Surface_Reflectance_670 = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}

	//L2 CLM of CAPI
    L2_clm_memory.Data.Cloud_Mask                = NULL;
	L2_clm_memory.Data.Confidence_Flag           = NULL;
	L2_clm_memory.Data.Quality_Flag              = NULL;
	L2_clm_memory.Geo.Latitude                   = NULL;
	L2_aer_memory.Geo.Longitude                  = NULL;
	if(L2_clm_memory.Geo.Latitude == NULL)
		{
			if( (L2_clm_memory.Geo.Latitude = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}
	if(L2_aer_memory.Geo.Longitude == NULL)
		{
			if( (L2_clm_memory.Geo.Longitude = (float *)calloc(DataLines_size*DataPixels_size*4,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}		
    if(L2_clm_memory.Data.Cloud_Mask == NULL)
		{
			if( (L2_clm_memory.Data.Cloud_Mask = (int *)calloc(DataLines_size*DataPixels_size*4,sizeof(int))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}
    if(L2_clm_memory.Data.Confidence_Flag == NULL)
		{
			if( (L2_clm_memory.Data.Confidence_Flag = (int *)calloc(DataLines_size*DataPixels_size*4,sizeof(int))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}
    if(L2_clm_memory.Data.Quality_Flag == NULL)
		{
			if( (L2_clm_memory.Data.Quality_Flag = (int *)calloc(DataLines_size*DataPixels_size*4,sizeof(int))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}	
		
    //L1
	if(l1b_CAPI.NonPolarization.Ref_038_QKM == NULL)
	{
		if( (l1b_CAPI.NonPolarization.Ref_038_QKM = (float *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}
	if(l1b_CAPI.NonPolarization.Ref_087_QKM == NULL)
	{
		if( (l1b_CAPI.NonPolarization.Ref_087_QKM = (float *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}
  //Polarization
	if(l1b_CAPI.Polarization.Ref_067_I_QKM == NULL)
	{
		if( (l1b_CAPI.Polarization.Ref_067_I_QKM = (float *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l1b_CAPI.Polarization.Ref_067_Q_QKM == NULL)
	{
		if( (l1b_CAPI.Polarization.Ref_067_Q_QKM = (float *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}


	if(l1b_CAPI.Polarization.Ref_067_U_QKM == NULL)
	{
		if( (l1b_CAPI.Polarization.Ref_067_U_QKM = (float *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}
   //Ancillary
	if(l1b_CAPI.Ancillary.frame_DarkCurrent_QKM == NULL)
	{
		if( (l1b_CAPI.Ancillary.frame_DarkCurrent_QKM = (int *)calloc(FRAME_LEN*L1BSOUNDING_NUM1,sizeof(int))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l1b_CAPI.Ancillary.frame_id_QKM == NULL)
	{
		if( (l1b_CAPI.Ancillary.frame_id_QKM = (int *)calloc(FRAME_LEN,sizeof(int))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l1b_CAPI.Ancillary.frame_time_second_J2000 == NULL)
	{
		if( (l1b_CAPI.Ancillary.frame_time_second_J2000 = (float *)calloc(FRAME_LEN,sizeof(float))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l1b_CAPI.Ancillary.frame_qual_flag_QKM == NULL)
		{
			if( (l1b_CAPI.Ancillary.frame_qual_flag_QKM = (unsigned int *)calloc(FRAME_LEN*L1BSOUNDING_NUM,sizeof(unsigned int))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}


	/////////////////////////////////////l1b/////////////////////////////////////


	/////////////////////////////////////l2////////////////////////////////////
	//RetrievalResults
	//Data
	if(l2_CML.Data.Confidence_Flag == NULL)
	{
		if( (l2_CML.Data.Confidence_Flag = (int *)calloc(RetrievalPonitNum*21,sizeof(int))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l2_CML.Data.Cloud_Mask == NULL)
	{
		if( (l2_CML.Data.Cloud_Mask = (int *)calloc(RetrievalPonitNum,sizeof(int))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	if(l2_CML.Data.Quality_Flag == NULL)
	{
		if( (l2_CML.Data.Quality_Flag = (int *)calloc(RetrievalPonitNum,sizeof(int))) == NULL  )
		{
			return SYSTEM_APPLY_MEMORY_ERR;
		}
	}

	
	//L2 AER DATA

	if(l2_AER.Data.Aerosol_Optical_Depth == NULL)
		{
			if( (l2_AER.Data.Aerosol_Optical_Depth = (float *)calloc(RetrievalPonitNum*21,sizeof(float))) == NULL  )
			{
				return SYSTEM_APPLY_MEMORY_ERR;
			}
		}

	if(l2_AER.Data.Angstrom_Exponent == NULL)
			{
				if( (l2_AER.Data.Angstrom_Exponent = (float *)calloc(RetrievalPonitNum*21,sizeof(float))) == NULL  )
				{
					return SYSTEM_APPLY_MEMORY_ERR;
				}
			}

	if(l2_AER.Data.Fine_Mode_Fraction == NULL)
			{
				if( (l2_AER.Data.Fine_Mode_Fraction = (float *)calloc(RetrievalPonitNum*21,sizeof(float))) == NULL  )
				{
					return SYSTEM_APPLY_MEMORY_ERR;
				}
			}

	if(l2_AER.Data.Surface_Reflectance_670 == NULL)
				{
					if( (l2_AER.Data.Surface_Reflectance_670 = (float *)calloc(RetrievalPonitNum*21,sizeof(float))) == NULL  )
					{
						return SYSTEM_APPLY_MEMORY_ERR;
					}
				}
				

	if(l2_AER.Data.Quality_Flag == NULL)
				{
					if( (l2_AER.Data.Quality_Flag = (int *)calloc(RetrievalPonitNum*21,sizeof(int))) == NULL  )
					{
						return SYSTEM_APPLY_MEMORY_ERR;
					}
				}

	
	/////////////////////////////////////l2////////////////////////////////////

	return SUCCESS;
}



//changed by liwei @ 20170209  把读取L2数据中分开为单独读取云检测和气溶胶的两个函数
int AuxProcess :: read_CAPI_CLM_L2_FILE(char *input_CLM,L2_CAPIX_CLM_HDF &L2_clm_HDF2)
{
	
	int iReturnCode;
	char SDSpath[64];
	
	strcpy(SDSpath,"Data/Cloud_Mask");
	iReturnCode = Readhdf(input_CLM,SDSpath,L2_clm_HDF2.Data.Cloud_Mask,H5T_NATIVE_INT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	strcpy(SDSpath,"Data/Confidence_Flag");	
    iReturnCode = Readhdf(input_CLM,SDSpath,L2_clm_HDF2.Data.Confidence_Flag,H5T_NATIVE_INT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	strcpy(SDSpath,"Data/Quality_Flag");	
     iReturnCode = Readhdf(input_CLM,SDSpath,L2_clm_HDF2.Data.Quality_Flag,H5T_NATIVE_INT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	strcpy(SDSpath,"Geo/Latitude");
     iReturnCode = Readhdf(input_CLM,SDSpath,L2_clm_HDF2.Geo.Latitude,H5T_NATIVE_FLOAT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	strcpy(SDSpath,"Geo/Longitude");	
     iReturnCode = Readhdf(input_CLM,SDSpath,L2_clm_HDF2.Geo.Longitude,H5T_NATIVE_FLOAT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}       
	
	return SUCCESS;
}

//changed by liwei @ 20170209  把读取L2数据中分开为单独读取云检测和气溶胶的两个函数
int AuxProcess :: read_CAPI_AER_L2_FILE(char *input_AER,L2_CAPIX_AER_HDF &L2_aer_HDF2)
{
	int iReturnCode;
	char SDSpath[64];
	strcpy(SDSpath,"Geo/Latitude");
	iReturnCode = Readhdf(input_AER,SDSpath,L2_aer_HDF2.Geo.Latitude,H5T_NATIVE_FLOAT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	printf("read Latitude finish!\n");
	
	// strcpy(SDSpath,"Geo/Longitude");
	// iReturnCode = Readhdf(input_AER,SDSpath,L2_aer_HDF2.Geo.Longitude,H5T_NATIVE_FLOAT);
		// if(iReturnCode != 0)
		// {
			// return READ_HDF_SDS_ERR;
		// }
	// printf("read Longitude finish!\n");
	
	strcpy(SDSpath,"Data/Aerosol_Optical_Depth");
    iReturnCode = Readhdf(input_AER,SDSpath,L2_aer_HDF2.Data.Aerosol_Optical_Depth,H5T_NATIVE_FLOAT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	printf("read Aerosol_Optical_Depth finish!\n");	
	
	strcpy(SDSpath,"Data/Surface_Reflectance_670");
    iReturnCode = Readhdf(input_AER,SDSpath,L2_aer_HDF2.Data.Surface_Reflectance_670,H5T_NATIVE_FLOAT);
		if(iReturnCode != 0)
		{
			return READ_HDF_SDS_ERR;
		}
	printf("read Surface_Reflectance_670 finish!\n");		

	return SUCCESS;
}






int AuxProcess :: read_CAPI_L1B_FILE()
{	
	 int iReturnCode;
    //NonPolarization
	char SDSpath[64];
	strcpy(SDSpath,"Non Polarization/Ref_038_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.NonPolarization.Ref_038_QKM,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
	strcpy(SDSpath,"Non Polarization/Ref_087_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.NonPolarization.Ref_087_QKM,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
//   Polarition
	strcpy(SDSpath,"Polarization/Ref_067_I_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Polarization.Ref_067_I_QKM,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
    strcpy(SDSpath,"Polarization/Ref_067_Q_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Polarization.Ref_067_Q_QKM,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
	strcpy(SDSpath,"Polarization/Ref_067_U_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Polarization.Ref_067_U_QKM,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
//   //Ancillary
	strcpy(SDSpath,"Ancillary/frame_DarkCurrent_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Ancillary.frame_DarkCurrent_QKM,H5T_NATIVE_INT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
	strcpy(SDSpath,"Ancillary/frame_id_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Ancillary.frame_id_QKM,H5T_NATIVE_INT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
	strcpy(SDSpath,"Ancillary/frame_time_second_J2000");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Ancillary.frame_time_second_J2000,H5T_NATIVE_FLOAT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}
	strcpy(SDSpath,"Ancillary/frame_qual_flag_QKM");
	iReturnCode = Readhdf(l1b_CAPIFileName,SDSpath,l1b_CAPI.Ancillary.frame_qual_flag_QKM,H5T_NATIVE_UINT);
	if(iReturnCode != 0)
	{
		return READ_HDF_SDS_ERR;
	}	
	return SUCCESS;
}


//export L2 file
int AuxProcess::Write_CLM_L2_HDF()
{
	int ret;
	int iRank = 2;
	hsize_t hDims0[2] = {RetrievalPonitNum,21};
	hsize_t chunk_size0[2] = {1,20};

	char *stringvalue_shape;
	char *stringvalue_Desc;
	char *stringvalue_longName;
	stringvalue_shape = new char[256];
	stringvalue_Desc = new char[256];
	stringvalue_longName = new char[256];

	char *SDValue[2];
	char *SDValue1;
	SDValue[0] = new char[64];
	SDValue[1] = new char[64];
	SDValue1 = new char[64];

	hid_t FileID;
   FileID = H5Fcreate(l2_CMLFileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	H5Fclose(FileID);

	//Data
	strcpy(SDValue[0],"Data");
	strcpy(SDValue[1],"Geo");


	CreateGroup(l2_CMLFileName,SDValue,2,0);

	delete[] SDValue[0];
	delete[] SDValue[1];

	//L2 CML DATA Cloud_Mask
	iRank = 2;
	hDims0[0] =  RetrievalPonitNum;
	hDims0[1] = 20;
	chunk_size0[0] = 1;
	chunk_size0[1] = 20;
	char units[8];  strcpy(units,"none");
	int valid_range[2] = {0,1};
	int FillValue = -9999;
	char long_name[64];   //= "7778Cloud Mask";
	float Slope = 1.0;
	float Intercept = 1.0;
	char SDSpath[64];
	
	strcpy(long_name,"7778Cloud Mask");
	strcpy(SDSpath,"Data/Cloud_Mask");
	ret = Write_CAPI_hdf(l2_CMLFileName,SDSpath,l2_CML.Data.Cloud_Mask,iRank,hDims0,chunk_size0,H5T_NATIVE_FLOAT,
			units,valid_range,FillValue,long_name,Slope,Intercept);

	//L2 CML DATA Quality_Flag
		valid_range[0] = 0;
		valid_range[1] = 100;
		strcpy(long_name,"Claoud mask confidence flag");
		strcpy(SDSpath,"Data/Quality_Flag");
		//long_name = "Claoud mask confidence flag";
		ret = Write_CAPI_hdf(l2_CMLFileName,SDSpath,l2_CML.Data.Quality_Flag,iRank,hDims0,chunk_size0,H5T_NATIVE_FLOAT,
				units,valid_range,FillValue,long_name,Slope,Intercept);

	//L2 CLM DATA Quality_Flag
		//long_name = "Quality assurance flag";
		strcpy(long_name,"Quality assurance flag");
		strcpy(SDSpath,"Data/Confidence_Flag");
		ret = Write_CAPI_hdf(l2_CMLFileName,SDSpath,l2_CML.Data.Confidence_Flag,iRank,hDims0,chunk_size0,H5T_NATIVE_FLOAT,
				units,valid_range,FillValue,long_name,Slope,Intercept);

	//L2 CML GEO Latitude
		float valid_rangeA[2] = {-90.0,90.0};
		float FillValueA = -9999;
		//long_name = "Latitude";
		strcpy(long_name,"Latitude");
		strcpy(SDSpath,"Geo/Latitude");
		ret = Write_CAPI_hdf(l2_CMLFileName,SDSpath,l2_CML.Geo.Latitude,iRank,hDims0,chunk_size0,H5T_NATIVE_FLOAT,
				units,valid_rangeA,FillValueA,long_name,Slope,Intercept);

	//L2 CML GEO Longitude
		valid_rangeA[0] = -180;
		valid_rangeA[1] = 180;
		//long_name = "Longitude";
		strcpy(long_name,"Longitude");
		strcpy(SDSpath,"Geo/Longitude");
		ret = Write_CAPI_hdf(l2_CMLFileName,SDSpath,l2_CML.Geo.Longitude,iRank,hDims0,chunk_size0,H5T_NATIVE_FLOAT,
				units,valid_rangeA,FillValueA,long_name,Slope,Intercept);

	return SUCCESS;
}

//test by wyg0808
int AuxProcess::ApplyMemory_CLM_Clob(L2_CLM_GlobalAttr &L2_CLM_GLOB3)
{	const int Len_CAPI = 512;
    L2_CLM_GLOB3.SatelliteName=new char[Len_CAPI];
	L2_CLM_GLOB3.SensorName=new char[64];
	L2_CLM_GLOB3.FileName=new char[Len_CAPI];
	L2_CLM_GLOB3.AncillaryDataDescriptors=new char[Len_CAPI];
	L2_CLM_GLOB3.InputPointer=new char[Len_CAPI];
	L2_CLM_GLOB3.DataLevel=new char[Len_CAPI];
	L2_CLM_GLOB3.L2FullPhysicsAlgorithmDescriptor=new char[Len_CAPI];
	L2_CLM_GLOB3.SoftwareVersion=new char[64];
	L2_CLM_GLOB3.SoftwareRevisionDate=new char[Len_CAPI];
	L2_CLM_GLOB3.ObservingBeginningDate=new char[Len_CAPI];
	L2_CLM_GLOB3.ObservingBeginningTime=new char[Len_CAPI];
	L2_CLM_GLOB3.ObservingEndingDate=new char[Len_CAPI];
	L2_CLM_GLOB3.ObservingEndingTime=new char[Len_CAPI];
	L2_CLM_GLOB3.DataCreatingDate=new char[Len_CAPI];
	L2_CLM_GLOB3.DataCreatingTime=new char[Len_CAPI];
	L2_CLM_GLOB3.Resolution=new char[Len_CAPI];
	L2_CLM_GLOB3.UnitResolution=new char[Len_CAPI];
	L2_CLM_GLOB3.AdditionalAnotation=new char[Len_CAPI];
	L2_CLM_GLOB3.HDFVersionId=new char[Len_CAPI];    
	
	return SUCCESS;
}

int AuxProcess::Read_CLM_L1_HDF(char *input_AER_L1b_DF2,L2_CLM_GlobalAttr &L2_CLM_Glob2)
{	    
  
	hid_t hFileID1;	
	hid_t status;
	hFileID1 = H5Fopen(input_AER_L1b_DF2,H5F_ACC_RDWR, H5P_DEFAULT);
	status = H5LTget_attribute_string(hFileID1,"/","Satellite Name",L2_CLM_Glob2.SatelliteName);
	status = H5LTget_attribute_string(hFileID1,"/","Sensor Name",L2_CLM_Glob2.SensorName);
	status = H5LTget_attribute_string(hFileID1,"/","File Name",L2_CLM_Glob2.FileName);
	status = H5LTget_attribute_string(hFileID1,"/","AncillaryDataDescriptors",L2_CLM_Glob2.AncillaryDataDescriptors);
	status = H5LTget_attribute_string(hFileID1,"/","InputPointer",L2_CLM_Glob2.InputPointer);
	status = H5LTget_attribute_string(hFileID1,"/","Data Level",L2_CLM_Glob2.DataLevel);
	status = H5LTget_attribute_int(hFileID1,"/","StartOrbitNumber",L2_CLM_Glob2.StartOrbitNumber);
	status = H5LTget_attribute_int(hFileID1,"/","StopOrbitNumber",L2_CLM_Glob2.StopOrbitNumber);
	status = H5LTget_attribute_int(hFileID1,"/","ActualFrames",L2_CLM_Glob2.ActualFrames);
	status = H5LTget_attribute_int(hFileID1,"/","ExpectedFrames",L2_CLM_Glob2.ExpectedFrames);
	//status = H5LTget_attribute_string(hFileID1,"/","L2FullPhysicsAlgorithmDescriptor",L2_CLM_Glob2.L2FullPhysicsAlgorithmDescriptor);
	status = H5LTget_attribute_string(hFileID1,"/","L1BPhysicsAlgorithmDescriptor",L2_CLM_Glob2.L2FullPhysicsAlgorithmDescriptor);
	status = H5LTget_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_Glob2.SoftwareVersion);
	status = H5LTget_attribute_string(hFileID1,"/","Software Revision Date",L2_CLM_Glob2.SoftwareRevisionDate);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Beginning Date",L2_CLM_Glob2.ObservingBeginningDate);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Beginning Time",L2_CLM_Glob2.ObservingBeginningTime);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Ending Date",L2_CLM_Glob2.ObservingEndingDate);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Ending Time",L2_CLM_Glob2.ObservingEndingTime);
	status = H5LTget_attribute_string(hFileID1,"/","Data Creating Date",L2_CLM_Glob2.DataCreatingDate);
	status = H5LTget_attribute_string(hFileID1,"/","Data Creating Time",L2_CLM_Glob2.DataCreatingTime);
	status = H5LTget_attribute_int(hFileID1,"/","Number Of Data Level",L2_CLM_Glob2.NumberDataLevel);
	status = H5LTget_attribute_string(hFileID1,"/","Resolution",L2_CLM_Glob2.Resolution);
	status = H5LTget_attribute_string(hFileID1,"/","Unit Of Resolution",L2_CLM_Glob2.UnitResolution);
	status = H5LTget_attribute_int(hFileID1,"/","Data Lines",L2_CLM_Glob2.DataLines);
	status = H5LTget_attribute_int(hFileID1,"/","Data Pixels",L2_CLM_Glob2.DataPixels);
	status = H5LTget_attribute_string(hFileID1,"/","Additional Anotation",L2_CLM_Glob2.AdditionalAnotation);
	status = H5LTget_attribute_string(hFileID1,"/","HDFVersionId",L2_CLM_Glob2.HDFVersionId);	
	status = H5LTget_attribute_float(hFileID1,"/","Left-Top Latitude",L2_CLM_Glob2.LeftTopLat);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Top Longitude",L2_CLM_Glob2.LeftTopLon);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Top Latitude",L2_CLM_Glob2.RightTopLat);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Top Longitude",L2_CLM_Glob2.RightTopLon);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Bottom Latitude",L2_CLM_Glob2.LeftBottomLat);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Bottom Longitude",L2_CLM_Glob2.LeftBottomLon);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Bottom Latitude",L2_CLM_Glob2.RightBottomLat);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Bottom Longitude",L2_CLM_Glob2.RightBottomLon);
	
	return SUCCESS;
}
int AuxProcess::Write_CLM_L2_HDF(char *input_AER2,L2_CLM_GlobalAttr &L2_CLM_GLOB6)
{
	//added by liwei @20170617
	strcpy(L2_CLM_GLOB6.DataLevel,"LEVEL 2");
	
	
	hid_t hFileID1;	
	hFileID1 = H5Fopen(input_AER2,H5F_ACC_RDWR, H5P_DEFAULT);	
	hid_t status=0;
	status = H5LTset_attribute_string(hFileID1,"/","Satellite Name",L2_CLM_GLOB6.SatelliteName);
	status = H5LTset_attribute_string(hFileID1,"/","Sensor Name",L2_CLM_GLOB6.SensorName);
	status = H5LTset_attribute_string(hFileID1,"/","File Name",L2_CLM_GLOB6.FileName);
	status = H5LTset_attribute_string(hFileID1,"/","AncillaryDataDescriptors",L2_CLM_GLOB6.AncillaryDataDescriptors);
	status = H5LTset_attribute_string(hFileID1,"/","InputPointer",L2_CLM_GLOB6.InputPointer);
	status = H5LTset_attribute_string(hFileID1,"/","Data Level",L2_CLM_GLOB6.DataLevel);
	status = H5LTset_attribute_string(hFileID1,"/","OrbitNumber",L2_CLM_GLOB6.orbitnumber);
	status = H5LTset_attribute_int(hFileID1,"/","ActualFrames",L2_CLM_GLOB6.ActualFrames,1);
	status = H5LTset_attribute_int(hFileID1,"/","ExpectedFrames",L2_CLM_GLOB6.ExpectedFrames,1);
	status = H5LTset_attribute_string(hFileID1,"/","L2FullPhysicsAlgorith",L2_CLM_GLOB6.L2FullPhysicsAlgorithmDescriptor);
	// status = H5LTset_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_GLOB6.SoftwareVersion);
	// status = H5LTset_attribute_string(hFileID1,"/","Software Revision Date",L2_CLM_GLOB6.SoftwareRevisionDate);
	status = H5LTset_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_GLOB6.VersionofSoftware);
	status = H5LTset_attribute_string(hFileID1,"/","Software Revision Date",L2_CLM_GLOB6.DateofSoftware);
	
	status = H5LTset_attribute_string(hFileID1,"/","Observing Beginning Date",L2_CLM_GLOB6.ObservingBeginningDate);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Beginning Time",L2_CLM_GLOB6.ObservingBeginningTime);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Ending Date",L2_CLM_GLOB6.ObservingEndingDate);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Ending Time",L2_CLM_GLOB6.ObservingEndingTime);
	status = H5LTset_attribute_string(hFileID1,"/","Data Creating Date",L2_CLM_GLOB6.DataCreatingDate);
	status = H5LTset_attribute_string(hFileID1,"/","Data Creating Time",L2_CLM_GLOB6.DataCreatingTime);
	status = H5LTset_attribute_int(hFileID1,"/","Number Of Data Level",L2_CLM_GLOB6.NumberDataLevel,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Top Latitude",L2_CLM_GLOB6.LeftTopLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Top Longitude",L2_CLM_GLOB6.LeftTopLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Top Latitude",L2_CLM_GLOB6.RightTopLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Top Longitude",L2_CLM_GLOB6.RightTopLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Bottom Latitude",L2_CLM_GLOB6.LeftBottomLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Bottom Longitude",L2_CLM_GLOB6.LeftBottomLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Bottom Latitude",L2_CLM_GLOB6.RightBottomLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Bottom Longitude",L2_CLM_GLOB6.RightBottomLon,1);
	status = H5LTset_attribute_string(hFileID1,"/","Resolution",L2_CLM_GLOB6.Resolution);
	status = H5LTset_attribute_string(hFileID1,"/","Unit Of Resolution",L2_CLM_GLOB6.UnitResolution);
	status = H5LTset_attribute_int(hFileID1,"/","Data Lines",L2_CLM_GLOB6.DataLines,1);
	status = H5LTset_attribute_int(hFileID1,"/","Data Pixels",L2_CLM_GLOB6.DataPixels,1);
	status = H5LTset_attribute_string(hFileID1,"/","Additional Anotation",L2_CLM_GLOB6.AdditionalAnotation);
	status = H5LTset_attribute_string(hFileID1,"/","HDFVersionId",L2_CLM_GLOB6.HDFVersionId);
	
	H5Fclose(hFileID1);
	
	return SUCCESS;
}

//从L1读气溶胶全局属性
int AuxProcess::Read_AER_L1_HDF(char *input_AER_L1b_DF2,L2_CLM_GlobalAttr &L2_CLM_Glob2)
{	    
    hid_t hFileID1;	
	hid_t status;
	hFileID1 = H5Fopen(input_AER_L1b_DF2,H5F_ACC_RDWR, H5P_DEFAULT);
	status = H5LTget_attribute_string(hFileID1,"/","Satellite Name",L2_CLM_Glob2.SatelliteName);
	status = H5LTget_attribute_string(hFileID1,"/","Sensor Name",L2_CLM_Glob2.SensorName);
	status = H5LTget_attribute_string(hFileID1,"/","File Name",L2_CLM_Glob2.FileName);
	status = H5LTget_attribute_string(hFileID1,"/","AncillaryDataDescriptors",L2_CLM_Glob2.AncillaryDataDescriptors);
	status = H5LTget_attribute_string(hFileID1,"/","InputPointer",L2_CLM_Glob2.InputPointer);
	status = H5LTget_attribute_string(hFileID1,"/","Data Level",L2_CLM_Glob2.DataLevel);
	status = H5LTget_attribute_int(hFileID1,"/","StartOrbitNumber",L2_CLM_Glob2.StartOrbitNumber);
	status = H5LTget_attribute_int(hFileID1,"/","StopOrbitNumber",L2_CLM_Glob2.StopOrbitNumber);
	status = H5LTget_attribute_int(hFileID1,"/","ActualFrames",L2_CLM_Glob2.ActualFrames);
	status = H5LTget_attribute_int(hFileID1,"/","ExpectedFrames",L2_CLM_Glob2.ExpectedFrames);
	status = H5LTget_attribute_string(hFileID1,"/","L2FullPhysicsAlgorith",L2_CLM_Glob2.L2FullPhysicsAlgorithmDescriptor);
	
	status = H5LTget_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_Glob2.SoftwareVersion);
	status = H5LTget_attribute_string(hFileID1,"/","Software Revision",L2_CLM_Glob2.SoftwareRevisionDate);
	
	status = H5LTget_attribute_string(hFileID1,"/","Observing Beginning Date",L2_CLM_Glob2.ObservingBeginningDate);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Beginning Time",L2_CLM_Glob2.ObservingBeginningTime);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Ending Date",L2_CLM_Glob2.ObservingEndingDate);
	status = H5LTget_attribute_string(hFileID1,"/","Observing Ending Time",L2_CLM_Glob2.ObservingEndingTime);
	status = H5LTget_attribute_string(hFileID1,"/","Data Creating Date",L2_CLM_Glob2.DataCreatingDate);
	status = H5LTget_attribute_string(hFileID1,"/","Data Creating Time",L2_CLM_Glob2.DataCreatingTime);
	status = H5LTget_attribute_int(hFileID1,"/","Number Of Data Level",L2_CLM_Glob2.NumberDataLevel);
	status = H5LTget_attribute_string(hFileID1,"/","Resolution",L2_CLM_Glob2.Resolution);
	status = H5LTget_attribute_string(hFileID1,"/","Unit Of Resolution",L2_CLM_Glob2.UnitResolution);
	status = H5LTget_attribute_int(hFileID1,"/","Data Lines",L2_CLM_Glob2.DataLines);
	status = H5LTget_attribute_int(hFileID1,"/","Data Pixels",L2_CLM_Glob2.DataPixels);
	status = H5LTget_attribute_string(hFileID1,"/","Additional Anotation",L2_CLM_Glob2.AdditionalAnotation);
	status = H5LTget_attribute_string(hFileID1,"/","HDFVersionId",L2_CLM_Glob2.HDFVersionId);	
	status = H5LTget_attribute_float(hFileID1,"/","Left-Top Latitude",L2_CLM_Glob2.LeftTopLat);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Top Longitude",L2_CLM_Glob2.LeftTopLon);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Top Latitude",L2_CLM_Glob2.RightTopLat);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Top Longitude",L2_CLM_Glob2.RightTopLon);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Bottom Latitude",L2_CLM_Glob2.LeftBottomLat);
	status = H5LTget_attribute_float(hFileID1,"/","Left-Bottom Longitude",L2_CLM_Glob2.LeftBottomLon);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Bottom Latitude",L2_CLM_Glob2.RightBottomLat);
	status = H5LTget_attribute_float(hFileID1,"/","Right-Bottom Longitude",L2_CLM_Glob2.RightBottomLon);
	
	return SUCCESS;
}
//读取CFG中的属性值(云检测)
int AuxProcess::Read_CLM_CFG(struct TDPInputxml *inputParam,TanFileName *tanfilename,L2_CLM_GlobalAttr &L2_CLM_GLOB,L2_CAPIX_CLM_HDF &L2_clm_HDF)
{
	//get config filename
	sprintf(tanfilename->Filename_cfg, "%s%sPUBLIC%s%s", inputParam->ConfigDIR,DIR_CHAR,DIR_CHAR, FILE_NAME_CFG);
	
	
	//printf("tanfilename->Filename_cfg = %s\n",tanfilename->Filename_cfg);
	
	
	//打开文件名配置文件 tanfilename->IFL_NWP
	int retValue;
	PList plist1;
	retValue = plist1.MakeList(tanfilename->Filename_cfg);
	
	//文件名称 
	sprintf(L2_CLM_GLOB.FileName, "%s",tanfilename->IFL_CAPI_L2_CLM_HDF);	
	//输入的辅助数据文件名
	sprintf(L2_CLM_GLOB.AncillaryDataDescriptors, "%s",tanfilename->IFL_NWP);
	
	
	//从配置文件读取软件版本号和版本日期 added by liwei @20170627
	
	plist1.GetValue("CAPI_L2.VersionofSoftware",tanfilename->VersionofSoftware);				
	plist1.GetValue("CAPI_L2.DateofSoftware",tanfilename->DateofSoftware);
											
	printf("tanfilename->VersionofSoftware = %s\n",tanfilename->VersionofSoftware);
	printf("tanfilename->DateofSoftware = %s\n",tanfilename->DateofSoftware);
	 
	
	
	//输入的主要数据文件   IFL_CAPI_L1B_1KM_GEO_HDF	
	sprintf(L2_CLM_GLOB.InputPointer, "%s   %s",tanfilename->IFL_CAPI_L1B_1KM_HDF,tanfilename->IFL_CAPI_L1B_1KM_GEO_HDF);
	//数据级别	
	plist1.GetValue("L2_AerosolParameter.DataLevel",L2_CLM_GLOB.DataLevel);
	//数据获取时的轨道号
	sprintf(L2_CLM_GLOB.orbitnumber, "%s",tanfilename->orbitNumber);
	//产品应有帧数
	L2_CLM_GLOB.ExpectedFrames[0] = L2_CLM_GLOB.DataLines[0];
	//产品算法标识和版本
	plist1.GetValue("L2_AerosolParameter.L2FullPhysicsAlgorithmDescriptor",L2_CLM_GLOB.L2FullPhysicsAlgorithmDescriptor);
	//处理软件版本号
	plist1.GetValue("L2_AerosolParameter.SoftwareVersion",L2_CLM_GLOB.SoftwareVersion);
	//处理软件更新日期
	plist1.GetValue("L2_AerosolParameter.SoftwareRevisionDate",L2_CLM_GLOB.SoftwareRevisionDate);
	//数据创建日期
	char temp[16];
	time_t timep;
	struct tm *p;
	time(&timep);
	p = localtime(&timep);
	
	/***********十以内数前面补零，int变char*************/  //add by liwei @20160114
	char temp_mon[5];
	char temp_mday[5];
	char temp_hour[5];
	char temp_min[5];
	char temp_sec[5];
	
	if(p->tm_mon < 10)
	    sprintf(temp_mon,"0%d",1+p->tm_mon);
	else
	    sprintf(temp_mon,"%d",1+p->tm_mon);
		
	if(p->tm_mday < 10)
	    sprintf(temp_mday,"0%d",p->tm_mday);
	else
	    sprintf(temp_mday,"%d",p->tm_mday);
		
	if(p->tm_hour < 10)
	    sprintf(temp_hour,"0%d",p->tm_hour);
	else
	    sprintf(temp_hour,"%d",p->tm_hour);
		
	if(p->tm_min < 10)
	    sprintf(temp_min,"0%d",p->tm_min);
	else
	    sprintf(temp_min,"%d",p->tm_min);
		
	if(p->tm_sec < 10)
	    sprintf(temp_sec,"0%d",p->tm_sec);
	else
	    sprintf(temp_sec,"%d",p->tm_sec);
	/***************************************************/
	

	//sprintf(temp, "%d-%d-%d",(1900+p->tm_year),(1+p->tm_mon),p->tm_mday);
	sprintf(temp, "%d-%s-%s",(1900+p->tm_year),temp_mon,temp_mday);   // 李伟  @2070114
	sprintf(L2_CLM_GLOB.DataCreatingDate, "%s",temp);       
	//数据创建时间(包括时分秒毫秒)
	//sprintf(temp, "%d:%d:%d",p->tm_hour,p->tm_min,p->tm_sec);
	sprintf(temp, "%s:%s:%s",temp_hour,temp_min,temp_sec);    // 李伟  @2070114
	sprintf(L2_CLM_GLOB.DataCreatingTime, "%s",temp);
	
	
	//纬度全局属性        
	 L2_CLM_GLOB.LeftTopLat[0]      = L2_clm_HDF.Geo.Latitude[0];
	 L2_CLM_GLOB.RightTopLat[0]     = L2_clm_HDF.Geo.Latitude[400*1-1];
	 L2_CLM_GLOB.LeftBottomLat[0]   = L2_clm_HDF.Geo.Latitude[400*2456];
	 L2_CLM_GLOB.RightBottomLat[0]  = L2_clm_HDF.Geo.Latitude[400*2457-1];
    //经度全局属性        
	 L2_CLM_GLOB.LeftTopLon[0]      = L2_clm_HDF.Geo.Longitude[0];
	 L2_CLM_GLOB.RightTopLon[0]     = L2_clm_HDF.Geo.Longitude[400*1-1];
	 L2_CLM_GLOB.LeftBottomLon[0]   = L2_clm_HDF.Geo.Longitude[400*2456];
	 L2_CLM_GLOB.RightBottomLon[0]  = L2_clm_HDF.Geo.Longitude[400*2457-1];	 
	//分辨率
	plist1.GetValue("CAPI_L2.clm_resolution",L2_CLM_GLOB.Resolution);
	//分辨率单位
	plist1.GetValue("CAPI_L2.UnitResolution",L2_CLM_GLOB.UnitResolution);	
	//文件的附加说明
	plist1.GetValue("L2_AerosolParameter.AdditionalAnotation",L2_CLM_GLOB.AdditionalAnotation);
	//HDF版本及压缩方式描述
	plist1.GetValue("L2_AerosolParameter.HDFVersionId",L2_CLM_GLOB.HDFVersionId);
	
	return SUCCESS;
}
//读取CFG中的属性值(气溶胶)
int AuxProcess::Read_AER_CFG(struct TDPInputxml *inputParam,TanFileName *tanfilename,L2_CLM_GlobalAttr &L2_CLM_GLOB,L2_CAPIX_AER_HDF &L2_aer_HDF)
{
	//get config filename
	sprintf(tanfilename->Filename_cfg, "%s%sPUBLIC%s%s", inputParam->ConfigDIR,DIR_CHAR,DIR_CHAR, FILE_NAME_CFG);
	//打开文件名配置文件 tanfilename->IFL_NWP
	int retValue;
	PList plist1;
	retValue = plist1.MakeList(tanfilename->Filename_cfg);
	
	sprintf(L2_CLM_GLOB.FileName, "%s",tanfilename->IFL_CAPI_L2_AER_HDF);
	
	
	plist1.GetValue("CAPI_L2.aer_resolution",L2_CLM_GLOB.Resolution);	
	return SUCCESS;
}
//把从L1B读到的全局属性数据写到L2的HDF中
int AuxProcess::Write_AER_L2_HDF(char *input_AER2,L2_CLM_GlobalAttr &L2_CLM_GLOB6)
{
	//added by liwei @20170617
	strcpy(L2_CLM_GLOB6.DataLevel,"LEVEL 2");
	
	hid_t hFileID1;	
	hFileID1 = H5Fopen(input_AER2,H5F_ACC_RDWR, H5P_DEFAULT);	
	hid_t status=0;
	status = H5LTset_attribute_string(hFileID1,"/","Satellite Name",L2_CLM_GLOB6.SatelliteName);
	status = H5LTset_attribute_string(hFileID1,"/","Sensor Name",L2_CLM_GLOB6.SensorName);
	status = H5LTset_attribute_string(hFileID1,"/","File Name",L2_CLM_GLOB6.FileName);
	status = H5LTset_attribute_string(hFileID1,"/","AncillaryDataDescriptors",L2_CLM_GLOB6.AncillaryDataDescriptors);
	status = H5LTset_attribute_string(hFileID1,"/","InputPointer",L2_CLM_GLOB6.InputPointer);
	status = H5LTset_attribute_string(hFileID1,"/","Data Level",L2_CLM_GLOB6.DataLevel);
	status = H5LTset_attribute_string(hFileID1,"/","OrbitNumber",L2_CLM_GLOB6.orbitnumber);
	status = H5LTset_attribute_int(hFileID1,"/","ActualFrames",L2_CLM_GLOB6.ActualFrames,1);
	status = H5LTset_attribute_int(hFileID1,"/","ExpectedFrames",L2_CLM_GLOB6.ExpectedFrames,1);
	status = H5LTset_attribute_string(hFileID1,"/","L2FullPhysicsAlgorith",L2_CLM_GLOB6.L2FullPhysicsAlgorithmDescriptor);
	// status = H5LTset_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_GLOB6.SoftwareVersion);
	// status = H5LTset_attribute_string(hFileID1,"/","Software Revision Date",L2_CLM_GLOB6.SoftwareRevisionDate);
	
	status = H5LTset_attribute_string(hFileID1,"/","Version Of Software",L2_CLM_GLOB6.VersionofSoftware);
	status = H5LTset_attribute_string(hFileID1,"/","Software Revision Date",L2_CLM_GLOB6.DateofSoftware);
	
	status = H5LTset_attribute_string(hFileID1,"/","Observing Beginning Date",L2_CLM_GLOB6.ObservingBeginningDate);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Beginning Time",L2_CLM_GLOB6.ObservingBeginningTime);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Ending Date",L2_CLM_GLOB6.ObservingEndingDate);
	status = H5LTset_attribute_string(hFileID1,"/","Observing Ending Time",L2_CLM_GLOB6.ObservingEndingTime);
	status = H5LTset_attribute_string(hFileID1,"/","Data Creating Date",L2_CLM_GLOB6.DataCreatingDate);
	status = H5LTset_attribute_string(hFileID1,"/","Data Creating Time",L2_CLM_GLOB6.DataCreatingTime);
	status = H5LTset_attribute_int(hFileID1,"/","Number Of Data Level",L2_CLM_GLOB6.NumberDataLevel,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Top Latitude",L2_CLM_GLOB6.LeftTopLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Top Longitude",L2_CLM_GLOB6.LeftTopLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Top Latitude",L2_CLM_GLOB6.RightTopLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Top Longitude",L2_CLM_GLOB6.RightTopLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Bottom Latitude",L2_CLM_GLOB6.LeftBottomLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Left-Bottom Longitude",L2_CLM_GLOB6.LeftBottomLon,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Bottom Latitude",L2_CLM_GLOB6.RightBottomLat,1);
	status = H5LTset_attribute_float(hFileID1,"/","Right-Bottom Longitude",L2_CLM_GLOB6.RightBottomLon,1);
	status = H5LTset_attribute_string(hFileID1,"/","Resolution",L2_CLM_GLOB6.Resolution);
	status = H5LTset_attribute_string(hFileID1,"/","Unit Of Resolution",L2_CLM_GLOB6.UnitResolution);
	status = H5LTset_attribute_int(hFileID1,"/","Data Lines",L2_CLM_GLOB6.DataLines,1);
	status = H5LTset_attribute_int(hFileID1,"/","Data Pixels",L2_CLM_GLOB6.DataPixels,1);
	status = H5LTset_attribute_string(hFileID1,"/","Additional Anotation",L2_CLM_GLOB6.AdditionalAnotation);
	status = H5LTset_attribute_string(hFileID1,"/","HDFVersionId",L2_CLM_GLOB6.HDFVersionId);
	
	H5Fclose(hFileID1);
	
	return SUCCESS;
}


int AuxProcess::WrReportFile(int errorcode, int Warningcount)
{
	return SUCCESS;
}

void AuxProcess::FreeMemory(L2_CAPIX_AER_HDF &L22_capi_aer_hdf,L2_CAPIX_CLM_HDF &L22_capi_clm_hdf,L2_CLM_GlobalAttr &L2_CLM_GLOB12){
	//l1
	if (l1b_CAPI.NonPolarization.Ref_038_QKM != NULL)
	{
		free(l1b_CAPI.NonPolarization.Ref_038_QKM);
		l1b_CAPI.NonPolarization.Ref_038_QKM = NULL;
	}

	if(l1b_CAPI.NonPolarization.Ref_087_QKM != NULL)
	{
		free(l1b_CAPI.NonPolarization.Ref_087_QKM);
		l1b_CAPI.NonPolarization.Ref_087_QKM = NULL;
	}

	if(l1b_CAPI.Polarization.Ref_067_I_QKM != NULL)
	{
		free(l1b_CAPI.Polarization.Ref_067_I_QKM);
		l1b_CAPI.Polarization.Ref_067_I_QKM = NULL;
	}


	if(l1b_CAPI.Polarization.Ref_067_Q_QKM != NULL)
	{
		free(l1b_CAPI.Polarization.Ref_067_Q_QKM);
		l1b_CAPI.Polarization.Ref_067_Q_QKM = NULL;
	}


	if(l1b_CAPI.Polarization.Ref_067_U_QKM != NULL)
	{
		free(l1b_CAPI.Polarization.Ref_067_U_QKM);
		l1b_CAPI.Polarization.Ref_067_U_QKM = NULL;
	}


	if(l1b_CAPI.Ancillary.frame_DarkCurrent_QKM != NULL)
	{
		free(l1b_CAPI.Ancillary.frame_DarkCurrent_QKM);
		l1b_CAPI.Ancillary.frame_DarkCurrent_QKM = NULL;
	}


	if(l1b_CAPI.Ancillary.frame_id_QKM != NULL)
	{
		free(l1b_CAPI.Ancillary.frame_id_QKM);
		l1b_CAPI.Ancillary.frame_id_QKM = NULL;
	}


	if(l1b_CAPI.Ancillary.frame_time_second_J2000 != NULL)
	{
		free(l1b_CAPI.Ancillary.frame_time_second_J2000);
		l1b_CAPI.Ancillary.frame_time_second_J2000 = NULL;
	}



	if(l1b_CAPI.Ancillary.frame_qual_flag_QKM != NULL)
	{
		free(l1b_CAPI.Ancillary.frame_qual_flag_QKM);
		l1b_CAPI.Ancillary.frame_qual_flag_QKM = NULL;
	}


	if(l1b_CAPI.Ancillary.coef_Cal != NULL)
	{
		free(l1b_CAPI.Ancillary.coef_Cal);
		l1b_CAPI.Ancillary.coef_Cal = NULL;
	}


	if(l1b_CAPI.Ancillary.OpertionMode != NULL)
	{
		free(l1b_CAPI.Ancillary.OpertionMode);
		l1b_CAPI.Ancillary.OpertionMode = NULL;
	}



	//l2
	if( l2_CML.Data.Confidence_Flag != NULL)
	{
		free(l2_CML.Data.Confidence_Flag);
		l2_CML.Data.Confidence_Flag  = NULL;
	}

	if( l2_CML.Data.Cloud_Mask != NULL)
	{
		free(l2_CML.Data.Cloud_Mask);
		l2_CML.Data.Cloud_Mask  = NULL;
	}

	if(l2_CML.Data.Quality_Flag != NULL)
	{
		free(l2_CML.Data.Quality_Flag);
		l2_CML.Data.Quality_Flag  = NULL;
	}

	if(l2_CML.Geo.Latitude != NULL)
	{
		free(l2_CML.Geo.Latitude);
		l2_CML.Geo.Latitude  = NULL;
	}

	if(l2_CML.Geo.Longitude != NULL)
	{
		free(l2_CML.Geo.Longitude);
		l2_CML.Geo.Longitude  = NULL;
	}

	//L2 AER DATA
	if(l2_AER.Data.Aerosol_Optical_Depth != NULL)
		{
			free(l2_AER.Data.Aerosol_Optical_Depth);
			l2_AER.Data.Aerosol_Optical_Depth  = NULL;
		}

	if(l2_AER.Data.Angstrom_Exponent != NULL)
			{
				free(l2_AER.Data.Angstrom_Exponent);
				l2_AER.Data.Angstrom_Exponent  = NULL;
			}

	if(l2_AER.Data.Fine_Mode_Fraction != NULL)
				{
					free(l2_AER.Data.Fine_Mode_Fraction);
					l2_AER.Data.Fine_Mode_Fraction  = NULL;
				}

	if(l2_AER.Data.Surface_Reflectance_670 != NULL)
					{
						free(l2_AER.Data.Surface_Reflectance_670);
						l2_AER.Data.Surface_Reflectance_670  = NULL;
					}
					
	if(l2_AER.Data.Surface_Reflectance_870 != NULL)
					{
						free(l2_AER.Data.Surface_Reflectance_870);
						l2_AER.Data.Surface_Reflectance_870  = NULL;
					}	

	if(l2_AER.Data.Surface_Reflectance_1640 != NULL)
					{
						free(l2_AER.Data.Surface_Reflectance_1640);
						l2_AER.Data.Surface_Reflectance_1640  = NULL;
					}				

	if(l2_AER.Data.Quality_Flag != NULL)
						{
							free(l2_AER.Data.Quality_Flag);
							l2_AER.Data.Quality_Flag  = NULL;
						}

	//L2 AER GEO
	if(l2_AER.Geo.Latitude != NULL)
							{
								free(l2_AER.Geo.Latitude);
								l2_AER.Geo.Latitude  = NULL;
							}

	if(l2_AER.Geo.Longitude != NULL)
								{
									free(l2_AER.Geo.Longitude);
									l2_AER.Geo.Longitude  = NULL;
								}
								
	//out jpg
    if(L22_capi_aer_hdf.Geo.Latitude != NULL)
								{
									free(L22_capi_aer_hdf.Geo.Latitude);
									L22_capi_aer_hdf.Geo.Latitude  = NULL;
								}	
	if(L22_capi_aer_hdf.Data.Aerosol_Optical_Depth != NULL)
								{
									free(L22_capi_aer_hdf.Data.Aerosol_Optical_Depth);
									L22_capi_aer_hdf.Data.Aerosol_Optical_Depth  = NULL;
								}						
	if(L22_capi_aer_hdf.Data.Surface_Reflectance_670 != NULL)
								{
									free(L22_capi_aer_hdf.Data.Surface_Reflectance_670);
									L22_capi_aer_hdf.Data.Surface_Reflectance_670  = NULL;
								}	

/*------------------------L2GlobalAttribute-----------------------------------*/
		 delete []L2_CLM_GLOB12.SatelliteName;
		delete []L2_CLM_GLOB12.SensorName;
		delete []L2_CLM_GLOB12.FileName;
		delete []L2_CLM_GLOB12.AncillaryDataDescriptors; 
		
		delete []L2_CLM_GLOB12.InputPointer;
		delete []L2_CLM_GLOB12.DataLevel;
		delete []L2_CLM_GLOB12.L2FullPhysicsAlgorithmDescriptor;
		delete []L2_CLM_GLOB12.SoftwareVersion;
		delete []L2_CLM_GLOB12.SoftwareRevisionDate;
		delete []L2_CLM_GLOB12.ObservingBeginningDate;
		delete []L2_CLM_GLOB12.ObservingBeginningTime;
		delete []L2_CLM_GLOB12.ObservingEndingDate;
		delete []L2_CLM_GLOB12.ObservingEndingTime;
		delete []L2_CLM_GLOB12.DataCreatingDate;
		delete []L2_CLM_GLOB12.DataCreatingTime;
		delete []L2_CLM_GLOB12.Resolution;
		delete []L2_CLM_GLOB12.UnitResolution;
		delete []L2_CLM_GLOB12.AdditionalAnotation;
		delete []L2_CLM_GLOB12.HDFVersionId;								
	
	/////////////////////////////////////l2////////////////////////////////////

};

void AuxProcess::WrWarningMessage(int code, char* descp, int* Warningcount) {

	/*if ((*Warningcount) < 100) {
		rpt.WarningCode[(*Warningcount)] = code;
		strcpy(rpt.WarningDestription[(*Warningcount)], descp);
		(*Warningcount)++;
	}*/
}

void AuxProcess::TimeConvert(time_t timeCode, char *timeString) {
	struct tm *ltime;

	ltime = localtime(&timeCode);
	sprintf(timeString, "%04d %02d %02d %02d:%02d:%02d", ltime->tm_year + 1900,
			ltime->tm_mon + 1, ltime->tm_mday, ltime->tm_hour, ltime->tm_min,
			ltime->tm_sec);

}

