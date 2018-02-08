//#include <stdlib.h>
//#include <stdio.h>
//#include <GetShmKey.h>
////#include <ZPPT.h>
//#include <signal.h>
//#include <iostream>
//#include <memory.h>
//#include <string.h>
//#include <WriteLogNew.h>
//#include <COMMZ.h>
//#include <ERROR.h>
//#include <JPEG.h>
//#include <hdf5.h>
//#include <zhdf.h>
//#include <FY3L1bHeader.h>
//#include <SDSMAXSCANS.h>
//
//#include "ParamDll.h"
////#include "LocalMem.h"
//#include "LocalZPPT.h"
//#include "CommFunc.h"
//#include "cloudmask.h"
//
//
//#define NUM_LINES     2000
//#define NUM_PIXELS    2048
//#define MASK_DIM      6
//#define MAX_FILE_PATH 255
//
//using namespace FY3JPEG;
//
//PList plist;
//bool g_StandAlone = false;
//int iReturn = 0;
//
////逻辑名物理名
//char in_phyName[3][MAX_FILE_PATH];
//char out_phyName[2][MAX_FILE_PATH];
//
//char phyName_jpg[MAX_FILE_PATH];
//char phyName_thumb[MAX_FILE_PATH];
//
////char strILogicName[3][MAX_FILE_PATH]={"IFL_MERSI_GEO1KM","IFL_MCMG_H5",ARRAYEND};
////char strOLogicName[2][MAX_FILE_PATH]={"IFL_IMAGE",ARRAYEND};
//
//unsigned char* landsea;
//unsigned char* landsea_shadow;
//unsigned char* cloudmask_array;
//unsigned char* cloudmask_test;
//unsigned char* cloudmask_shadow;
//unsigned char* cloudmask_test_shadow;
//unsigned char  cloudmask_tmp;
//
//int main(int argc, char* argv[])
//{
//	int testdata1 = 1;
//	char *ptest = (char*)&testdata1;
//	if(*ptest == 1)
//	{
//		printf("little\n");
//	}
//	else
//	{
//		printf("big\n");
//	}
//
//	memset(in_phyName, 0, 3 * MAX_FILE_PATH);
//    memset(out_phyName,0, 2 * MAX_FILE_PATH);
//
//	strcpy(in_phyName[0],argv[1]);
//	strcpy(in_phyName[1],argv[2]);
//	strcpy(out_phyName[0],argv[3]);
//	//初始化过程，逻辑名物理名转换
//
//	/* if(argc == 1)
//	{
//		printf("程序参数小于2个，使用 standalone 模式\n");
//        if(plist.MakeList("MCMGJPEG.standalone") == 1)
//        {
//            g_StandAlone = true;
//        }
//        else
//        {
//            printf("Need MCMGJPEG.standalone!\n");
//            exit(0);
//        }
//	}
//	if(g_StandAlone)
//	{
//		printf("MCMG running in standalone mode!\n");
//		iReturn = Z503::ZPPTInit("MCMGJPEG.standalone");
//		if(iReturn != 0)
//		{
//			printf("ZPPTInit()错误，直接退出.\n");
//			return -1;
//		}
//	}
//	else
//	{
//		printf("MCMG running not in standalone mode!\n");
//		iReturn = ZLDSInit(argv[1],GETPRONAME);    //日志服务初始化了
//        if(iReturn != 0)
//		{
//            ZLDSWriteLogForProduct('E', "MCMGJPEG进程初始化，日志服务初始化失败!",5);
//            printf("日志公共服务初始化失败\n");
//			return -1;
//        }
//        ZLDSWriteLogForProduct('N', "MCMGJPEG进程初始化，日志服务初始化成功!",5);
//		iReturn = ZPPTInit(argv[1]);
//        if(iReturn != 0)
//        {
//            printf("ZPPTInit()错误,直接退出.\n");
//			ZLDSWriteLogForProduct('E', "文件名转换公共服务初始化失败，程序退出\n",5);
//			return -1;
//        }
//	}
//
//	if(g_StandAlone)
//	{
//		printf("111111----------------%d\n",iReturn);
//		 iReturn = Z503::ZPPTGetInputDataName((char*)(strILogicName[0]),(char*)(in_phyName[0]),1);
//		iReturn = Z503::ZPPTGetInputDataName((char*)(strILogicName[1]),(char*)(in_phyName[1]),1);
//		printf("222222----------------%d\n",iReturn);
//		printf("%s\n",in_phyName[0]);
//		printf("%s\n",in_phyName[1]);
//		iReturn += Z503::ZPPTGetOutputDataName((char*)(strOLogicName[0]), (char*)(out_phyName[0]),1);
//		printf("-------------------%d\n",iReturn);
//		printf("%s\n",out_phyName[0]);
//		if(iReturn != 0)
//		{
//			return -1;
//		}
//
//	}
//	else
//	{
//		iReturn = ZPPTGetInputDataName((char*)strILogicName,(char*)in_phyName);
//		if(iReturn != 0)
//        {
//			ZLDSWriteLogForProduct('E', "取得输入文件物理名失败，程序退出\n",5);
//			return -1;
//        }
//		ZLDSWriteLogForProduct('N', "取得输入文件物理名成功.",5);
//
//		iReturn = ZPPTGetOutputDataName((char*)strOLogicName, (char*)out_phyName);
//		if(iReturn != 0)
//		{
//			ZLDSWriteLogForProduct('E', "取得输出文件物理名失败，程序退出\n",5);
//			return -1;
//		}
//		ZLDSWriteLogForProduct('N', "取得输出文件物理名成功.",5);
//	} */
//
//	strcpy(phyName_jpg, out_phyName[0]);
//    strcat(phyName_jpg, ".ORIGI.JPG");
//    strcpy(phyName_thumb, out_phyName[0]);
//    strcat(phyName_thumb, ".THUMB.JPG");
//
//	//为储存输出结果的cloudmask_array数组分配空间
//    cloudmask_array = (unsigned char*)malloc(sizeof(unsigned char)*NUM_PIXELS*NUM_LINES*MASK_DIM);
//    cloudmask_test  = (unsigned char*)malloc(sizeof(unsigned char)*NUM_PIXELS*NUM_LINES);
//	//为读入的L1数据集分配空间
//	landsea = (unsigned char*)malloc(sizeof(unsigned char)*NUM_PIXELS*NUM_LINES);
//
//	char	strUnits[20];
//	char	strLongName[200];
////	char*  strLongName=0;
//	char	strBandName[20];
////	char*	strBandName=0;
//	int		iValidRange[2];
//	int		iFillValue;
//	float	fIntercept;
//	float	  fSlope;
//	int		status = 0;
//
//	status = ZHDFReadSDSDataUChar(in_phyName[0], "Geolocation/LandSeaMask",landsea,
//									strUnits, iValidRange, &iFillValue,&fIntercept,
//									&fSlope, strLongName,strBandName);
//	if(status != 0)
//	{
//		printf("读取L1数据失败，退出程序.\n");
//		return -1;
//	}
//
//	status = ZHDFReadSDSDataUChar(in_phyName[1], "Cloud_Mask",cloudmask_array,
//									strUnits, iValidRange, &iFillValue,&fIntercept,
//									&fSlope, strLongName,strBandName);
//	if(status != 0)
//	{
//		printf("读取云检测数据集失败，退出程序.\n");
//		return -1;
//	}
//	cloudmask_shadow = cloudmask_array;          //将cloudmask得到的结果：cloudmask_array的首地址赋给cloudmask_shadow
//	landsea_shadow = landsea;                    //将内存中的landsea数据的首地址赋给landsea_shadow
//	cloudmask_test_shadow = cloudmask_test;      //将存放处理后结果的数组cloudmask_tset的首地址赋给cloudmask_test_shadow
//	for(int i = 0; i < NUM_PIXELS*NUM_LINES; i++)
//	{
//		cloudmask_tmp = *cloudmask_shadow;
//		unsigned char   cloudmask_tmp_shadow;
//		cloudmask_tmp_shadow = cloudmask_tmp;
//		if(cloudmask_tmp == 0)
//		{
//			*cloudmask_test_shadow = 7;
//		}
//		else if(cloudmask_tmp != 0)
//		{
//			cloudmask_tmp_shadow = cloudmask_tmp_shadow&1;
//			if(cloudmask_tmp_shadow==0)
//			{
//				*cloudmask_test_shadow = 7;      //cloudmask的第一位是0，代表计算没有决定这个点，所以数据无效
//			}
//			else if(cloudmask_tmp_shadow==1)
//			{
//				//cloudmask的第一位是1，代表这个点的计算是有效的，执行下面的语句
//				cloudmask_tmp = (cloudmask_tmp & 6)/2;   //将cloudmask数组中的一个数据的第2，3位提取出来赋给cloudmask_test数组中的相应数据
//				if(cloudmask_tmp == 3)
//				{
//					//如果提取出来的数据是3，代表这个点是晴空，这个时候要判断这个点是水，还是陆地。
//					if((*landsea_shadow == 1) || (*landsea_shadow) == 4)     //这个点是陆地且晴空
//						*cloudmask_test_shadow = 3;
//					else if((*landsea_shadow==0)
//						||(*landsea_shadow==3)
//						||(*landsea_shadow==5)
//						||(*landsea_shadow==6)
//						||(*landsea_shadow==7))      //这个点是水面，下面要判断这里是不是太阳耀斑
//					{
//						cloudmask_tmp = *cloudmask_shadow;
//						cloudmask_tmp = (cloudmask_tmp>>4)&1;
//                        //unsigned char temp = cloudmask_tmp&16;
//						if(cloudmask_tmp == 1)
//						{
//							//这个点不是太阳耀斑区
//							*cloudmask_test_shadow = 4;
//                            //countNSG++;
//						}
//						else if(cloudmask_tmp == 0)
//						{         //这个点是太阳耀斑区
//							*cloudmask_test_shadow = 5;
//                            //countSG++;
//						}
//					}
//				}
//				else if(cloudmask_tmp == 2)
//				{                //这个点是可能晴空
//					if((*landsea_shadow == 1) || (*landsea_shadow) == 4)	//这个点是陆地且也许晴空
//						*cloudmask_test_shadow = 2;
//					else if((*landsea_shadow==0)
//						||(*landsea_shadow==3)
//						||(*landsea_shadow==5)
//						||(*landsea_shadow==6)
//						||(*landsea_shadow==7))		//这个点是水面且也许晴空
//						*cloudmask_test_shadow = 6;
//				}
//				else
//					*cloudmask_test_shadow = cloudmask_tmp;		//如果landsea_tmp中的值不是3，则这个点不是晴空，直接将cloudmask_tmp赋给*cloudmask_test
//			}
//		}
//		if(*landsea_shadow==2)
//		{
//			*cloudmask_test_shadow = 8;             //这个点是海岸线
//		}
//
//		cloudmask_test_shadow++;
//		//tempcount++;
//		cloudmask_shadow++;
//		//cloudmask_shadow +=6;
//		landsea_shadow++;
//    }
//
//	//生成浏览图
//	JPEGInfo	info;
//    CPalette *palette = NULL;
//	CJPEGDataU8 CHs[3];
//
//	info.iHeight = 2000;
//	info.iWidth = 2048;
//	info.iQuality = 100;
//
//	CJPEG jpegRGB(info, COLOR_RGB);
//
//	printf("phyName_jpg = %s\n",phyName_jpg);
//
//	if(jpegRGB.Create(phyName_jpg) != 0)
//	{
//		printf("Create Jpg error!!!\n");
//		return -1;
//	}
//
//    /* short *JPGData = new short[2000*2048];*/
//    /* for(int i =0;i<2000*2048;i++)
//    {
//	    printf("___%d",cloudmask_test[i]);
//	}
//	 */
//	CHs[0].AddFillValue(255);
//	CHs[0].SetData(cloudmask_test);
//	CHs[0].SetRange(0,255);
//	CHs[0].SetSlope(1);
//	CHs[1].SetData(NULL);
//	CHs[2].SetData(NULL);
//
//	palette = jpegRGB.GetPalette();
//	palette->SetRange(0, 255);
//
//	palette->SetPaletteEntry(0,1, 255,255,255);          //生成白色，这个地方是云
//	palette->SetPaletteEntry(1,2, 169, 169, 169);        //生成灰色，这个地方不确定
//	palette->SetPaletteEntry(2,3, 0, 255, 0);            //生成亮绿，这个地方陆地也许晴空
//	palette->SetPaletteEntry(3,4, 0, 128, 0);            //生成绿色，这个地方陆地晴空
//	palette->SetPaletteEntry(4,5, 25, 25, 112);          //生成深蓝，这个地方水面晴空
//	palette->SetPaletteEntry(5,6, 255, 0, 0);            //生成红色，这个地方水面太阳耀斑
//	palette->SetPaletteEntry(6,7, 65, 105, 225);         //生成淡蓝，这个地方水面也许晴空
//	palette->SetPaletteEntry(7,8, 0, 0, 0);              //生成黑色，这个地方数据无效
//	palette->SetPaletteEntry(8,9, 255, 215, 0);          //生成黄色，这个是海岸线
//
//
//    jpegRGB.RGB(CHs);
//	//ZLDSWriteLogForProduct('N', "生成快视图。",5);
//
//    // printf("\n*************添加色标**********************\n");
//
//	CJPEG::JPEGRect rect = {0, 0, 0, 0};
//	rect.right = 300;
//	rect.bottom = 100;
//	const char logobmp[45] = "/COSWORK/PARM/FY3C/VIRR/logo_fy3_casc_8.bmp";
//	jpegRGB.Composite(logobmp, rect, 50, CJPEG::ES);
//
//	CJPEG::JPEGRect white_rect = {1760, 80, 0, 0};
//	rect.right = 300;
//	rect.bottom = 100;
//	const char white_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_white_sebiao.bmp";
//	jpegRGB.Composite(white_logobmp, white_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect gray_rect = {1760, 130, 0, 0};
//	const char gray_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_gray_sebiao.bmp";
//	jpegRGB.Composite(gray_logobmp, gray_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect green_rect = {1760, 180, 0, 0};
//	const char green_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_green_sebiao.bmp";
//	jpegRGB.Composite(green_logobmp, green_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect brightgreen_rect = {1760, 230, 0, 0};
//	const char brightgreen_logobmp[70] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_brightgreen_sebiao.bmp";
//	jpegRGB.Composite(brightgreen_logobmp, brightgreen_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect deepblue_rect = {1760, 280, 0, 0};
//	const char deepblue_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_deepblue_sebiao.bmp";
//	jpegRGB.Composite(deepblue_logobmp, deepblue_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect lightblue_rect = {1760, 330, 0, 0};
//	const char lightblue_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_lightblue_sebiao.bmp";
//	jpegRGB.Composite(lightblue_logobmp, lightblue_rect, 100, CJPEG::MANUAL);
//	CJPEG::JPEGRect black_rect = {1760, 380, 0, 0};
//	const char black_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_black_sebiao.bmp";
//	jpegRGB.Composite(black_logobmp, black_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect red_rect = {1760, 430, 0, 0};
//	const char red_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_red_sebiao.bmp";
//	jpegRGB.Composite(red_logobmp, red_rect, 100, CJPEG::MANUAL);
//
//	CJPEG::JPEGRect yellow_rect = {1760, 480, 0, 0};
//	const char yellow_logobmp[60] = "/COSWORK/PARM/FY3C/MERSI/logo_fy3_MCMG_yellow_sebiao.bmp";
//	jpegRGB.Composite(yellow_logobmp, yellow_rect, 100, CJPEG::MANUAL);
//
//	// printf("\n*************添加色标值**********************\n");
//	CJPEG::JPEGPoint point;
//	point.x = 1795;
//	point.y = 50;
//	jpegRGB.Annotate(L"MERSI Cloud Mask", point, CJPEG::JPEGHorizontal, 28,BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//	point.x = 1830;
//	point.y = 110;
//	jpegRGB.Annotate(L"Cloudy", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 160;
//	jpegRGB.Annotate(L"Uncertain", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 210;
//	jpegRGB.Annotate(L"Land Clear", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 260;
//	jpegRGB.Annotate(L"Poss Land Clear", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 310;
//	jpegRGB.Annotate(L"Water Clear", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 360;
//	jpegRGB.Annotate(L"Poss Water Clear", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 410;
//	jpegRGB.Annotate(L"Invalid", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 460;
//	jpegRGB.Annotate(L"Sun Glint", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//
//	point.x = 1830;
//	point.y = 510;
//	jpegRGB.Annotate(L"Coastline", point, CJPEG::JPEGHorizontal, 25, BLACK, 70,"/COSWORK/PARM/FY3C/MERSI/FZFangSong.ttf");
//	jpegRGB.WriteoutJPEG();
//	CJPEG *jpegT1 = jpegRGB.Thumbnail(phyName_thumb);
//	jpegT1->WriteoutJPEG();
//	printf("jpg completed.\n");
//	jpegRGB.Dispose();
//	delete jpegT1;
//
//	if(cloudmask_array != NULL)
//	{
//		free(cloudmask_array);
//		cloudmask_array = NULL;
//	}
//	if(landsea != NULL)
//	{
//		free(landsea);
//		landsea = NULL;
//	}
//	printf("JPEG Finished\n");
//    return 0;
//	}
