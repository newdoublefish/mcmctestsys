#include "toolbox.h"

typedef enum 
{
        /// <summary>
        /// δ�������
        /// </summary>
        UnDefine,
        /// <summary>
        /// 429����
        /// </summary>
        T429,
        /// <summary>
        /// �ȸ�λ���λ
        /// </summary>
        HL,
        /// <summary>
        /// �ȵ�λ���λ
        /// </summary>
        LH,
        /// <summary>
        /// ǰ16λΪ��λ����16λΪ��λ������16ǰ8λΪ��λ����8λΪ��λ
        /// </summary>
        HL32HL16,
        /// <summary>
        /// ǰ16λΪ��λ����16λΪ��λ������16ǰ8λΪ��λ����8λΪ��λ
        /// </summary>
        HL32LH16,
        /// <summary>
        /// ǰ16λΪ��λ����16λΪ��λ������16ǰ8λΪ��λ����8λΪ��λ
        /// </summary>
        LH32HL16,
        /// <summary>
        /// ǰ16λΪ��λ����16λΪ��λ������16ǰ8λΪ��λ����8λΪ��λ
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
