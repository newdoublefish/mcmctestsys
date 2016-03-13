/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：convertTool.h
 // 文件功能描述:
 //
 // 

 // 修改标识：曾衍仁  20150804
 // 修改描述：常用函数
 //-------------------------------------------------------------------------*/
#ifndef __CONVERTTOOL_H__
#define __CONVERTTOOL_H__
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
   extern "C" {
#endif	   
	   
unsigned long String2UnsignedLong(char *str);


unsigned long ByteArray2UnsignedLong(unsigned char *str,int num);

int String2ByteArray(char *str,unsigned char *array);

double String2Double(char *str);

void HexString2UpperCase(char *str);

int OctToHex(int a);

#ifdef __cplusplus
   }
#endif	   

#endif
