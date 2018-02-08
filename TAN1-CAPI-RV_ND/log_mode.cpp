具体写日志如下：

//returnCode标记调用函数整型变量，0：表示调用函数成功，非零：表示调用函数异常
int returnCode; 
//申明一个日志变量DPPSQAlog                    
CWriteLog DPPSQAlog;
//WrPieceMessage: remenber a piece of warning message, before write log file   
DPPSQAlog.WrPieceMessage("******TAN1-CAPIX-DPPS-QA module begin**\n", 0);
						 ....
						 ....
DPPSQAlog.WrPieceMessage("No file of log!\n",1);    
returnCode = function(Tpye &inputParam,Tpye &CAPI_TAN1,....);
  if( returnCode>0 )
	{
		printf("ReadInputXmlFile function failed!\n");
		DPPSQAlog.WrPieceMessage(" ReadInputXmlFile function failed!",1);
		//WrOldMessage(): write history warning message to log file
		DPPSQAlog.WrOldMessage();                           
		return  returnCode;
	}
DPPSQAlog.WrOldMessage();
//WriteLogFile: when mode<0 means warning, mode=0 means normal, mode>0 means error
DPPSQAlog.WriteLogFile("Read inputxml file over!",0);  
DPPSQAlog.WriteLogFile("Read inputxml no Moth!",-1);	
	
（注：WrPieceMessage、DPPSQAlog.WrOldMessage和WriteLogFile函数具体声明定义见CWriteLog_TAN.cpp和CWriteLog_TAN.h）
		