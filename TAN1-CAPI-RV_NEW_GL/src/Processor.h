#ifndef PROCESSER_H_
#define PROCESSER_H_

#include <stdint.h>
#include "RetrievalStruct.h"
#include "StructDefine.h"
#include "AuxProcess.h"

class Processor {
private:

	int rawimgsize;
	int singlewidth;
	int singlesize;

	struct RetrievalInputParam *PROCESSin;
	struct RetrievalOutputParam *PROCESSout;
	AuxProcess* RW;

	int RetrievalPonitNum;

	int* WarnCount;
	int WarnCode;
	char WarnDescp[512];

public:
	int processimgsize;

public:
	Processor();
	virtual ~Processor();

	// 处理函数集
	int init(AuxProcess* RandW, RetrievalInputParam* inputXML,RetrievalOutputParam* outputXML, int* Warningcount);
	int accelerate();

private:
	int SearchLeftValidPixel(int ip, int dpCount, int *dpList);
	int SearchRightValidPixel(int ip, int dpCount, int *dpList, int totalNum);
	void clean();

};

#endif /*PROCESSER_H_*/
