#ifndef __MASK_STRU_H__
#define __MASK_STRU_H__

//	�Ƽ������ṹ
//	��������һ��6�ֽ���������,�������ݰ�bitλ���
#pragma pack(push)
#pragma pack(1)
typedef struct
{
	unsigned CLOUD_MASK_FLAG	: 1;	//��Ч�Ա�ʶ
	unsigned UNOBSTRUCTED_FOV_CONFIDENCE_FLAG : 2; //��Ч��Ԫ�Ƽ����Ŷ�

	//�Ƽ��ģʽ
	unsigned DAY_NIGHT_FLAG		: 1;	//����ҹ���ʶ
	unsigned LAND_WATER_BORDER	: 1;	//ˮ½�߽�
	
	//�µ������ͱ�ʶ
	unsigned SURFACE_TYPE_FLAG	: 5;	//�µ������ͱ�ʶ

	//��ͨ���Ƽ����
	unsigned CLM_CH1			: 2;	//ͨ��1��������ɼ��⣩
	unsigned CLM_CH2			: 2;	//ͨ��2��������ɼ��⣩
	unsigned CLM_CH3			: 2;	//ͨ��3�������3.7��
	unsigned CLM_CH4			: 2;	//ͨ��4�������11��
	unsigned CLM_CH5			: 2;	//ͨ��5�������12��
	unsigned CLM_CH6			: 2;	//ͨ��6�������1.6��
//	unsigned CLM_CH7			: 2;	//ͨ��7������������⣬����
//	unsigned CLM_CH8			: 2;	//ͨ��8������������⣩
//	unsigned CLM_CH9			: 2;	//ͨ��9������������⣩
	unsigned CLM_CH10			: 2;	//ͨ��10����������ƣ�
	unsigned CLM_R2R1			: 2;	//ͨ��R2/R1�������R2/R1��
	unsigned CLM_TBB45			: 2;	//ͨ��T4-T5�������TBB45��
	unsigned CLM_TBB34			: 2;	//ͨ��T3-T4�������TBB34��
	unsigned CLM_TBB35			: 2;	//ͨ��T3-T5�������TBB35��
	//add by Pengfei@20130731
	unsigned SolarFlare			: 1;	//̫��ҫ��
}MASK_STRU;


//
//	ȡֵ
//
//��Ч�Ա�ʶȡֵ
const unsigned char NOT_DETERMINED	= 0x0;	//��Ч
const unsigned char DETERMINED		= 0x01;	//��Ч

//��Ч��Ԫ�Ƽ����Ŷ�ȡֵ JPEG_FILLED
const unsigned char CLOUDY			= 0x00;	//��
const unsigned char UNCERTAIN		= 0x01;	//��ȷ��
const unsigned char PROBABLY_CLEAR	= 0x02;	//���ŵ����
const unsigned char CONFIDENT_CLEAR = 0x03;	//ȷ�ŵ����
const unsigned char FILLED          = 0x3E7;//���ֵ999

//������
const int CLM_MASK_BYTE_LEN = 6;	//������Ϊ6�ֽ�
typedef union
{
	unsigned char clm_byte[CLM_MASK_BYTE_LEN];
	MASK_STRU clm_mask;
}CLM_MASK;
#pragma pack(pop)
#endif //__MASK_STRU_H__