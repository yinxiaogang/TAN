#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

#include "Processor.h"
#include "RetrievalDef.h"
#include "WinOrLinux.h"
#include "ParamDll_TAN.h"
#include "timestring.h"
#include "PublicErrorCode.h"


//extern void parity_check(unsigned char* pimage, int Image_Width, int Image_High);

Processor::Processor() {

}

Processor::~Processor() {
	clean();
}

int Processor::init(AuxProcess* RandW, RetrievalInputParam* inputXML,RetrievalOutputParam* outputXML, int* Warningcount)
{

	PROCESSin = inputXML;
	PROCESSout = outputXML;
	RW = RandW;

	WarnCount = Warningcount;

	RetrievalPonitNum = RW->RetrievalPonitNum;
	return SUCCESS;
}

int Processor::accelerate()
{
	int i,ret;


	/////////////////////////////////////l2 test///////////////////////////////////
	//RetrievalResults
	for(i=0;i<RetrievalPonitNum*21;i++)
	{
		RW->l2_CML.Data.Confidence_Flag[i] = 1.0;
	}


		/////////////////////////////////////l2////////////////////////////////////


	return ret;
}

void Processor::clean()
{

}


