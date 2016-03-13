#include "toolbox.h"

typedef enum 
{
        /// <summary>
        /// 未定义参数
        /// </summary>
        UnDefine,
        /// <summary>
        /// 429参数
        /// </summary>
        T429,
        /// <summary>
        /// 先高位后低位
        /// </summary>
        HL,
        /// <summary>
        /// 先低位后高位
        /// </summary>
        LH,
        /// <summary>
        /// 前16位为高位，后16位为低位，其中16前8位为高位，后8位为低位
        /// </summary>
        HL32HL16,
        /// <summary>
        /// 前16位为高位，后16位为低位，其中16前8位为低位，后8位为高位
        /// </summary>
        HL32LH16,
        /// <summary>
        /// 前16位为低位，后16位为高位，其中16前8位为高位，后8位为低位
        /// </summary>
        LH32HL16,
        /// <summary>
        /// 前16位为低位，后16位为高位，其中16前8位为低位，后8位为高位
        /// </summary>
        LH32LH16
}CollectParamType;

typedef struct{
   int paramId;
   int len;
   int dataType;
}Position;


typedef struct
{ 
    int frameIndex;
    int subFrameLen;
	ListType positionList;
}SubFrame;

void initFrame(void);

ListType getSubFrameList(void);

int getFrameLen(void);
