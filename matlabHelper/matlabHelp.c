#include "matlabHelp.h"

CAObjHandle OpenMatlab(void)
{
	CAObjHandle hMatlab=0;
	HRESULT stat;
	int result=0;
	stat=MLApp_NewDIMLApp(NULL,1,LOCALE_NEUTRAL,0,&hMatlab);  
	if(stat<0)
	{
	   MessagePopup("hello","can not lauch matlab!");
	   hMatlab=0;
	  
	}else
	{
	   MinMaxMatlab(hMatlab, 0);
	  // MessagePopup("hello","lauch success!"); 
	}
	return hMatlab;
	
}


int CloseMatlab(CAObjHandle hMatlab)
{
	if(hMatlab!=0)
	{
	    MLApp_DIMLAppQuit(hMatlab,NULL);
		return 0;
	}
	return -1;

}

//function [f,fdata]=xq_fft(data,fs)
/*
int AnalyseData(CAObjHandle hMatlab,char* mFilePath,double *data,double sample)
{
	HRESULT stat;
	int result=0;
	if(hMatlab!=0)
	{

	  result=SendMatrix(hMatlab,"data",(double *)data,NULL,1,100);
      if(result!=SUCCESS)
      {
	     MessagePopup("hello","sendMatrix error1!");    
	  }	   
      result=SendMatrix(hMatlab,"fs",((double*)&sample),NULL,1,1);
  	if(result!=SUCCESS)
	{
	   MessagePopup("hello","sendMatrix error2!");    
	}	   
	result=RunMatlabScript(hMatlab,mFilePath);
  	if(result!=SUCCESS)
	{
	   MessagePopup("hello","runScript error2!");    
	}	   
	 GetMatrix(hMatlab,"fdata",);
	return 0;
	}
	return -1;
}*/

/*

function [f,fdata]=xq_fft(data,fs)
%函数用作傅里叶变换，并输出频谱图
%输入
%data——输入数据
%fs——采样频率
%输出
%f——频率
%fdata——输出fft数据
n=length(data);%data长度
f=0:fs/(n-1):fs/2;%生成频率向量
data=data-mean(data);%去均值
% data=hann(n)'.*data;
data=hamming(n)'.*data;%加汉明窗
fdata=1.852*abs(fft(data))/(n/2);%fft并乘以1.852修正
%   plot(f,fdata(1:length(f)));%作频谱图
%fdata=2.8*abs(fft(data))/(n/2);%fft并乘以1.852修正
*/

int vibDataAnalyse(int hMatlab,double *data,int datalength,double sample,double **getData,double **getF)
{
	int i=0,j=0,k=0;
	int result = 0; 
	char message[3000];  
    unsigned    dim1            =   1;
    unsigned    dim2            =   100;
    double      *matrixReal     =   NULL;
    double      *matrixImag     =   NULL;

	dim2=datalength;
	
    result = SendMatrix(hMatlab,"data",(double*)data,NULL,1,datalength);   
    if (result != SUCCESS)
    {
       MessagePopup ("ERROR", "Error in sending Matrix to MATLAB");
       return 0;
    }
	Fmt(message,"%s<fs=%f",sample);
	
    result = RunMatlabCommand(hMatlab,message);
    result = RunMatlabCommand(hMatlab,"n=length(data);");
    result = RunMatlabCommand(hMatlab,"f=0:fs/(n-1):fs/2;");
    result = RunMatlabCommand(hMatlab,"data=data-mean(data);");
    result = RunMatlabCommand(hMatlab,"data=hamming(n)'.*data;");
    result = RunMatlabCommand(hMatlab,"fdata=1.852*abs(fft(data))/(n/2);");
	
	GetMatrix(hMatlab,"fdata",&matrixReal,&matrixImag,&dim1,&dim2);    //获取一维的数组    	
    Fmt (message, "%s<Inverse(matrix):\n");         
    /*for(i=0; i<dim1; ++i)
     {   
                for(j=0;j<dim2;++j)
                {
                    Fmt(message,"%s[a]<%f   ",*(matrixReal+k));
                    k++;
                }
                Fmt(message,"%s[a]<\n");
            }
	
	MessagePopup ("MATRIX IN", message);  	*/	
	*getData=matrixReal;
	*getF=matrixImag;
    //if (matrixReal != NULL) CA_FreeMemory(matrixReal);
   // if (matrixImag != NULL) CA_FreeMemory(matrixImag);
	
	return 0;
	

}

int invertCB (int hMatlab)
{
    /* The matrix to invert */
    double      matrix_r[2][2]    =   {{1,2},{3,4}};
    double      matrix_i[2][2]    =   {{0,0},{0,0}};
    double      *matrixReal     =   NULL;
    double      *matrixImag     =   NULL;
    unsigned    dim1            =   2;
    unsigned    dim2            =   2;
    unsigned    i               =   0;
    unsigned    j               =   0;
    unsigned    k               =   0;
    int         result          =   0;
    char        message[300];
    
            /* Send the matrix to MATLAB */
            result = SendMatrix(hMatlab,"cMatrix",(double*)matrix_r,(double*)matrix_i,2,2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending Matrix to MATLAB");
                return 0;
            }
            /* Invert the matrix in MATLAB */
            result = RunMatlabCommand(hMatlab,"mMatrix=inv(cMatrix);");
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in sending command to MATLAB");
                return 0;
            }
            /* Get the inverted matrix */
            result = GetMatrix(hMatlab,"mMatrix",&matrixReal,&matrixImag,&dim1,&dim2);
            if (result != SUCCESS)
            {
                MessagePopup ("ERROR", "Error in Getting Matrix from MATLAB");
                return 0;
            }           
            /* Display the matrices */
            Fmt (message, "%s<matrix:\n");
            for(i=0; i<dim1; ++i)
            {   
                for(j=0;j<dim2;++j)
                    Fmt (message, "%s[a]<%f   ", matrix_r[i][j]);
                Fmt (message, "%s[a]<\n");
            }
            MessagePopup ("MATRIX", message);
            if(matrixReal == NULL)
            {
                MessagePopup ("ERROR", "NULL pointer returned from MATLAB");
                return 0;
            }
            Fmt (message, "%s<Inverse(matrix):\n");         
            for(i=0; i<dim1; ++i)
            {   
                for(j=0;j<dim2;++j)
                {
                    Fmt(message,"%s[a]<%f   ",*(matrixReal+k));
                    k++;
                }
                Fmt(message,"%s[a]<\n");
            }
            MessagePopup ("INVERTED MATRIX", message);          
            if (matrixReal != NULL) CA_FreeMemory(matrixReal);
            if (matrixImag != NULL) CA_FreeMemory(matrixImag);
    return 0;
}

int stringTest(int hMatlab)
{
   char message[300]; 
   SendString(hMatlab,"hello","nihaoma");
   Fmt(message,"%s<fs=%f",4096.0);
   RunMatlabCommand(hMatlab,message); 
   
   return 0;
}

int scriptTest(int hMatlab,char *filePath,double *wave)
{
    unsigned    dim1            =   1;
    unsigned    dim2            =   100;

    double      *matrixReal     =   NULL;
    double      *matrixImag     =   NULL;
	
    char        message[3000];   	
	int i=0;
	int j=0;
	int k=0;
	for(int i=0;i<100;i++)
	{
	   wave[i]=i;
	}	
	SendMatrix(hMatlab,"data",(double*)wave,NULL,1,100);    //这里表示的是一维数组
	RunMatlabCommand(hMatlab, "fs=1000;");  
    
	 MessagePopup ("INVERTED MATRIX", filePath);
    RunMatlabScript(hMatlab,filePath);
	
	
	return 0;
}

int waveTest(int hMatlab,double *wave)
{
    unsigned    dim1            =   1;
    unsigned    dim2            =   100;

    double      *matrixReal     =   NULL;
    double      *matrixImag     =   NULL;
	
    char        message[3000];   	
	int i=0;
	int j=0;
	int k=0;
	for(int i=0;i<100;i++)
	{
	   wave[i]=i;
	}	
	SendMatrix(hMatlab,"hi",(double*)wave,NULL,1,100);    //这里表示的是一维数组
	
	 MessagePopup ("INVERTED MATRIX", "hello");  		
	 
	
	GetMatrix(hMatlab,"hi",&matrixReal,&matrixImag,&dim1,&dim2);    //获取一维的数组

            Fmt (message, "%s<Inverse(matrix):\n");         
            for(i=0; i<dim1; ++i)
            {   
                for(j=0;j<dim2;++j)
                {
                    Fmt(message,"%s[a]<%f   ",*(matrixReal+k));
                    k++;
                }
                Fmt(message,"%s[a]<\n");
            }
	
	 MessagePopup ("INVERTED MATRIX", message);  		
    if (matrixReal != NULL) CA_FreeMemory(matrixReal);
    if (matrixImag != NULL) CA_FreeMemory(matrixImag);
														  
	return 0;
}


