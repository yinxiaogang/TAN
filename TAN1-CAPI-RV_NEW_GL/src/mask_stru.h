#ifndef __MASK_STRU_H__
#define __MASK_STRU_H__

//	云检测掩码结构
//	云掩码是一个6字节整型数组,数据内容按bit位存放
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	unsigned CLOUD_MASK_FLAG	: 1;	//有效性标识
	unsigned UNOBSTRUCTED_FOV_CONFIDENCE_FLAG : 2; //有效象元云检测可信度

	//云检测模式
	unsigned DAY_NIGHT_FLAG		: 1;	//白天夜间标识
	unsigned LAND_WATER_BORDER	: 1;	//水陆边界
	
	//下垫面类型标识
	unsigned SURFACE_TYPE_FLAG	: 5;	//下垫面类型标识

	//单通道云检测结果
	unsigned CLM_CH1			: 2;	//通道1检测结果（可见光）
	unsigned CLM_CH2			: 2;	//通道2检测结果（可见光）
	unsigned CLM_CH3			: 2;	//通道3检测结果（3.7）
	unsigned CLM_CH4			: 2;	//通道4检测结果（11）
	unsigned CLM_CH5			: 2;	//通道5检测结果（12）
	unsigned CLM_CH6			: 2;	//通道6检测结果（1.6）
//	unsigned CLM_CH7			: 2;	//通道7检测结果（近红外，海洋）
//	unsigned CLM_CH8			: 2;	//通道8检测结果（近红外）
//	unsigned CLM_CH9			: 2;	//通道9检测结果（近红外）
	unsigned CLM_CH10			: 2;	//通道10检测结果（卷云）
	unsigned CLM_R2R1			: 2;	//通道R2/R1检测结果（R2/R1）
	unsigned CLM_TBB45			: 2;	//通道T4-T5检测结果（TBB45）
	unsigned CLM_TBB34			: 2;	//通道T3-T4检测结果（TBB34）
	unsigned CLM_TBB35			: 2;	//通道T3-T5检测结果（TBB35）
	//add by Pengfei@20130731
	unsigned SolarFlare			: 1;	//太阳耀斑
}MASK_STRU;


//
//	取值
//
//有效性标识取值
const unsigned char NOT_DETERMINED	= 0x0;	//无效
const unsigned char DETERMINED		= 0x01;	//有效

//有效象元云检测可信度取值 JPEG_FILLED
const unsigned char CLOUDY			= 0x00;	//云
const unsigned char UNCERTAIN		= 0x01;	//不确定
const unsigned char PROBABLY_CLEAR	= 0x02;	//可信的晴空
const unsigned char CONFIDENT_CLEAR = 0x03;	//确信的晴空
const unsigned char FILLED          = 0x3E7;//填充值999

//云掩码
const int CLM_MASK_BYTE_LEN = 6;	//云掩码为6字节
typedef union
{
	unsigned char clm_byte[CLM_MASK_BYTE_LEN];
	MASK_STRU clm_mask;
}CLM_MASK;
#pragma pack(pop)
#endif //__MASK_STRU_H__