//���ߣ�����
//���䣺bianhaoqiong(AT)163.com
//2012.04.26
//��ӭ�ο���������ע��ԭ����

#include <stdio.h>
#include <stdlib.h>
//�����ջ�ĳ���
#define OPSTACK_LENGTH 5
//������ջ�ĳ���
#define NUMSTACK_LENGTH 100
//���봮����󳤶�
#define MAX_STRING_LENGTH 100

//������ṹ��
struct operatorStruct
{
	//���������
    char name;
	//���ȼ�
    int priority;
	//Ŀ���������������������絥Ŀ�����Ϊ1��˫Ŀ�����2
    int opnum;
};

typedef struct operatorStruct OPERATOR;

//�����ջ
OPERATOR opStack[OPSTACK_LENGTH];
//�����ջ��ָ��
int opStackTop = -1;
//������ջ
double numStack[NUMSTACK_LENGTH];
//������ջ��ָ��
int numStackTop = -1;

//��ȡһ���ַ������������������ȼ�
int getPriority(char name)
{
    if (name == '(' || name == ')')
    {
        return 0;
    }
    if (name == '!')
    {
        return 3;
    }
    if (name == '*' || name == '/')
    {
        return 2;
    }
    if (name == '+' || name == '-')
    {
        return 1;
    }
	return -1;
}
//��ȡһ���ַ���������������Ŀ��
int getOpNum(char name)
{
    if (name == '*' || name == '/' || name == '+' || name == '-')
    {
        return 2;
    }
    if (name == '!')
    {
        return 1;
    }
    if (name == '(' || name == ')')
    {
        return 0;
    }
	return -1;
}

//�����ѹջ
void pushOperator(OPERATOR op)
{
    if (opStackTop < OPSTACK_LENGTH - 1)
    {
        opStack[++opStackTop] = op;
    }

}
//�������ջ
OPERATOR popOperator()
{
    if (opStackTop >= 0)
    {
        return opStack[opStackTop--];
    }else
	{
	    return  opStack[opStackTop]; 
	}	
	

}
//������ѹջ
void pushNumber(double num)
{
    if (numStackTop < NUMSTACK_LENGTH - 1)
    {
        numStack[++numStackTop] = num;
    }

}
//��������ջ
double popNumber()
{
    if (numStackTop >= 0)
    {
        return numStack[numStackTop--];
    }
	return -1.0;
}
//�������ַ����е���0-9��ͷ������һ�������������һ��ת��Ϊ������
//i���ϸ����Ͷ�Ӧ���ַ����ĳ���
double getNumFromString(char *s, int *i)
{
    int j = 0;
    static char numstr[MAX_STRING_LENGTH];
    while ((*s) >= '0' && *s <= '9')
    {
        numstr[j++] = (*s);
        s++;
    }
    if ((*s) == '.')
    {
        numstr[j++] = (*s);
        s++;
        while ((*s) >= '0' && *s <= '9')
        {
            numstr[j++] = (*s);
            s++;
        }
    }
    (*i) = (*i) + j;
    numstr[j] = '\0';
    return atof(numstr);
}

//�Ӳ�����ջ�е������������������һ��˫Ŀ����
double opertate2Num(OPERATOR op)
{
    double num2 = popNumber();
    double num1 = popNumber();
    if (op.name == '+')
    {
        return num1 + num2;
    }
    if (op.name == '-')
    {
        return num1 - num2;
    }
    if (op.name == '*')
    {
        return num1 * num2;
    }
    if (op.name == '/')
    {
        return num1 / num2;
    }
    return -1;
}
//�Ӳ�����ջ�е���һ�������������һ�ε�Ŀ����
double opertate1Num(OPERATOR op)
{
    double num = popNumber();
    if (op.name == '!')
    {
        double result = 1;
        while (num > 1)
        {
            result *= num;
            num--;
        }
        return result;
    }
    return -1;
}
//���һ������
double operate(OPERATOR op)
{
    if (op.opnum == 1)
    {
        return opertate1Num(op);
    }
    else if (op.opnum == 2)
    {
        return opertate2Num(op);
    }
    return -1;
}

double expressCalc(char *string)
{
    //char string[MAX_STRING_LENGTH];//���ʽ�����봮
    int i;
    OPERATOR op, topOp;//opΪ�ӵ�ǰ���봮����ȡ��һ���������topOpΪ�����ջջ���������
	
    topOp.name = '#';
    topOp.priority = 0;
    topOp.opnum = 0;
    pushOperator(topOp);//ѹ��#��Ϊ��ʼ�����
	
    //scanf("%s", string);
    for (i = 0; string[i] != '\0' && string[i] != '=';)
    {
		//�����봮��ȡ��һ���ַ���Ϊ��ʼ�����д���ֱ�����ʽ����
        if (string[i] >= '0' && string[i] <= '9')
        {
			//����ǲ���������������������ȡ������ѹ�������ջ
            pushNumber(getNumFromString(&string[i], &i));
        }
        else
        {
            op.name = string[i];
            op.priority = getPriority(string[i]);
            op.opnum = getOpNum(string[i]);
            topOp = popOperator();
            if (op.name == '(')
            {
				//�����'('������ջ�������������ѹ��ջ�ڣ�������ǰ�������ѹջ
                pushOperator(topOp);
                pushOperator(op);
            }
            else if (op.name == ')')
            {
				//�����')'����������㣬ÿ����������Ϊһ��������ѹ�������ջ��ֱ����'('���������ջ
                while (topOp.name != '(')
                {
                    pushNumber(operate(topOp));
                    topOp = popOperator();
                }
            }
            else
            {
				//�������ͨ�����
                if (topOp.name != '#' && op.priority <= topOp.priority)
                {
					//��������ջ�ǿգ��ҵ�ǰ����������ȼ�����ջ��������������һ�����㣬�����ѹ�������ջ
                    pushNumber(operate(topOp));
                }
                else
                {
					//���򽫴�ջ�������������ѹ��
                    pushOperator(topOp);
                }
				//����ǰ�����ѹջ
                pushOperator(op);
            }
            i++;
        }
    }
	//���ջ��ʣ�������
    while ((topOp = popOperator()).name != '#')
    {
        pushNumber(operate(topOp));
    }
	//������ջ��ʣ�µ����һ������Ϊ���
    //printf("%f\n", popNumber());
    //return 0;
	return popNumber();
}
