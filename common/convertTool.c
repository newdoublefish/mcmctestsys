/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include <ansi_c.h>
#include "windows.h" 

int OctToHex(int a)
{
	int ret=0;
	char temp[10]={0};
	sprintf(temp,"%d",a);
	for(int i=0;i<strlen(temp);i++)
	{
	   ret=ret*8+temp[i]-'0';
	}	
	return ret;
}




int StrToInt(char *str)
{
	int var=0;
	int long t;
	long len =strlen(str);
	if(var >8) 
		return -1;
	for(;*str;str++)
	{
	    if(*str>='A' && *str <='F')
			t=*str-55;
		else
			
		   t=*str-48;
		
		var<<=4;
		var|=t;
		
	
	}
	return var;
}




//把字符串转换成无符号长整形数，如char value="12345",转换后unsinged long value=12345;
//String2UnsignedLong
unsigned long String2UnsignedLong(char *str)
{
	unsigned long var=0;
	unsigned long t;
	long len =strlen(str);
	if(var >8) 
		return -1;
	for(;*str;str++)
	{
		if(*str>='a' && *str <='f')
		{
		   t=*str-87;
		}else if(*str>='A' && *str <='F')
		{	
			t=*str-55;
		}else if(*str>='0' && *str <='9')
		{
		    t=*str-48;
		}else{
		    var=0;
			continue;
		}	
		var<<=4;
		var|=t;
		
	
	}
	return var;
}

//将字符创转成字节数组，如把“0xAABBCCDD”,转换成一个数组unsigned char data[4];data[0]=0xaa,data[1]=0xbb,data[2]=0xcc,data[4]=0xdd;
//String2ByteArray
int String2ByteArray(char *str,unsigned char *array)
{
	//unsigned long t; 
	int len=0;
	for(;*str;str+=2)
	{
		char *tempStr;
		unsigned long temp=0;
		unsigned long temp2=0; 
		
		tempStr=str;
	    if(*tempStr>='A' && *tempStr <='F')
		{	
			temp=*tempStr-55;
		}else if(*tempStr>='a' && *tempStr <='f')
	    {  
			temp=*tempStr-87;
		}else if(*tempStr>='0' && *tempStr <='9')
		{
		    temp=*tempStr-48;
		}else{
		    continue;
		}
		
		tempStr=str+1;
	    if(*tempStr>='A' && *tempStr <='F')
		{	
			temp2=*tempStr-55;
		}else if(*tempStr>='a' && *tempStr <='f')
	    {  
			temp2=*tempStr-87;
		}else if(*tempStr>='0' && *tempStr <='9')
		{
		    temp2=*tempStr-48;
		}else{
		    continue;
		}		
		
		temp<<=4;
		temp|=temp2;
		*array=(unsigned char)(temp & 0xff);
		array++;
		len++;
	
	}
	return len;
}

//将字节数组转换成无符号长整形值 unsigned char data[4]={0xaa,0xbb,0xcc};转换后的值为unsigned long value=0xccbbaa;
//ByteArray2UnsignedLong
unsigned long ByteArray2UnsignedLong(unsigned char *str,int num)
{
     unsigned long var=0;
	 
	 for(int i=0;i<num;i++)
     {
	    var|=((unsigned long)str[i] & 0xff)<<(8*i);
	 }
	 return var;

}

//将doubleStr转换为double值，比如char *value="3.15",转换后为double value=3.15
//String2Double
double String2Double(char *str)
{
   int dInt,i,ln,f;
   double dDec;
   
   if(strlen(str)==1 && *str=='0')
   {
	   dDec=0;
	   return dDec;
   }	   

   
   f=*str=='-'?-1:1;
   while(*str<'1' && *str!='.' || *str>'9') str++;
   ln=strlen(str);
   
   for(dDec=dInt=i=0;str[i];i++)
   {
	   if(str[i]=='.')
	   {
	      for(ln--;ln>i;ln--)
		  {
		     if(str[ln]>='0' && str[ln]<='9')
			 {
			 	 //(dDec/=10)+=(str[ln]-'0')/10.0; 
				 dDec=dDec/10;
				 dDec+=(str[ln]-'0')/10.0;
			 }	 
		         //(dDec/=10)+=((str[ln]='0')/10.0); 		 
		  }
		  break;
	   }
	   if(str[i]>='0' && str[i]<='9')
	   {
	      //(dInt*=10)+=str[i]-'0';
		   dInt=dInt*10;
		   dInt+=str[i]-'0';
	   }	   
   }
   
   dDec+=dInt;
   return f==1?dDec:-dDec;
}

//将16进制数字的字符串中的小写转换成大小
void HexString2UpperCase(char *str)
{
    for(int i=0;i<strlen(str);i++)
	{
		
	    if(*(str+i)<='f' && *(str+i)>='a')
		{
			 *(str+i)+=('A'-'a');
		}	 
	}
}

//将8进制转换成十进制  
int Oct2Hex(int a)
{
	int ret=0;
	char temp[10]={0};
	sprintf(temp,"%d",a);
	for(int i=0;i<strlen(temp);i++)
	{
	   ret=ret*8+temp[i]-'0';
	}	
	return ret;
}
