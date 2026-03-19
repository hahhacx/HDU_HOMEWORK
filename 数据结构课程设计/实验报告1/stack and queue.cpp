#include <stdio.h>
#include <stdlib.h>


typedef int Status;         //声明函数类型名
#define OK 1
#define ERROR 0
#define OVERFLOW -2
#define STACK_INIT_SIZE    10
#define STACKINCREMENT  5


typedef int SElemType;  //声明SElemType的类型
typedef struct  { 

        SElemType *base;  //顺序栈数据存储空间基址 
        SElemType *top;    //顺序栈栈顶指针 
        int stacksize;

}SqStack;  // SqStack为用户定义的顺序栈类型



//顺序栈基本操作函数的实现
//利用malloc函数申请存储空间，构造一个空的顺序栈S，S的初始大小为STACK_INIT_SIZE。
Status InitStack(SqStack &S)
{
	S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
	if (!S.base) exit(OVERFLOW);
	S.top = S.base;
	S.stacksize = STACK_INIT_SIZE;
	return OK;
}

//将顺序栈数据清空。只需做S.top=S.base赋值操作即可。 
Status ClearStack(SqStack &S)
{
	S.top = S.base;
	return OK;
}

//返回L中数据元素个数。直接返回S.top-S.base的值即可。
Status StackLength(SqStack S)
{
	return S.top - S.base;
}

//用e返回栈顶元素的值。 
Status GetTop(SqStack S, SElemType &e)
{
	if (S.top != S.base)
	{
		e = *(S.top - 1);
		return OK;
	}
	return ERROR;
}

//将数据e进栈。
Status Push(SqStack &S, SElemType e)
{
	if (S.top - S.base >= S.stacksize)
	{
		S.base = (SElemType *)realloc(S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S.base) return ERROR;
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
	}
	*S.top = e;
	S.top++;
	return OK; 
}

//将栈顶元素出栈，用e返回其值。
Status Pop(SqStack &S, SElemType &e)
{
	if (S.top == S.base) return ERROR;
	--S.top;
	e = *S.top;
	return OK;
}



#define MAXQSIZE 10    //队列的最大长度
typedef int QElemType;  //声明SElemType的类型
typedef struct {

       QElemType *base;    //初始化队列时动态分配存储空间的地址(数组名)
       int front;                 //头元素的下标变量
       int rear;                   //尾元素的下标变量

}SqQueue;   // SqQueue为用户定义的循环队列类型


//循环队列基本操作函数的实现
//利用malloc函数申请存储空间，构造一个空的循环队列Q，Q的大小为MAXQSIZE。
Status InitQueue(SqQueue &Q)
{
	Q.base = (QElemType*)malloc(MAXQSIZE * sizeof(QElemType));
	if (!Q.base) exit(OVERFLOW);
	Q.front = Q.rear = 0;
	return OK;
}

//将循环队列数据清空。 
Status ClearQueue(SqQueue &Q)
{
	Q.front = Q.rear = 0;
	return OK;
}

//返回Q中数据元素个数。
Status QueueLength(SqQueue Q)
{
	return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}

//数据e入队。
Status EnQueue(SqQueue &Q, QElemType e)
{
	if ((Q.rear + 1) % MAXQSIZE == Q.front) return ERROR;
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % MAXQSIZE;
	return OK;
}

//数据出队，用e返回其值。
Status DeQueue(SqQueue &Q, QElemType &e)
{
	if (Q.rear == Q.front) return ERROR;
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % MAXQSIZE;
	return OK;
}

//判队空。
Status QueueEmpty(SqQueue Q)
{
	return Q.rear == Q.front;
}

//判队满。
Status QueueFull(SqQueue Q)
{
	return (Q.rear + 1) % MAXQSIZE == Q.front;
}

//利用顺序栈的基本操作函数，编制一个将十进制整数m转换成n进制(n=2或8)整数的数制转换函数Conversion(m,n) ，并在函数中输出转换结果。 
Status Conversion(int m,int n)
{
	int k = m;
	SqStack S;
	InitStack(S);
	while (m > 0)
	{
		Push(S, m % n);
        m /= n;
	} 
	printf("%d 转换为 %d 进制后结果为: ",k,n);
    while (StackLength(S) > 0) {
        SElemType e;
        Pop(S, e);
        printf("%d", e);
    }
    printf("\n");
    return OK; 
}

//编一个函数QueueReverse(Q)，利用栈将将队列中的元素逆序重排，即队头元素变成队尾元素。
Status QueueReverse(SqQueue &Q)
{
	SqStack S;
    InitStack(S);
    QElemType e;
    while (DeQueue(Q, e) != 0) {
        Push(S, e);
    }
    while (StackLength(S) > 0) {
        Pop(S, e);
        EnQueue(Q, e);
    }
    return OK;
}

int main()
{
	//数制转换
	int m;
    printf("请输入一个十进制整数: ");
    scanf("%d", &m);
    //2进制 
    Conversion(m, 2);
    //8进制 
    Conversion(m, 8);
    
    
    //连续入队9个整数后，显示队列Q的front和rear的值
    SqQueue Q;
    InitQueue(Q);
    for (int i = 1; i <= 9; i++) {
        EnQueue(Q, i);
    }
    printf("连续入队9个整数后, 队列头序号: %d, 队列尾序号: %d\n", Q.front, Q.rear);

    //若再入队1个整数，显示此时队列的状态信息
    if (EnQueue(Q, 10) == 0) {
        printf("队列空间已被占满，无法加入新元素\n");
    }

    //连续出队5个整数，再入队2个整数，显示此时的队列Q的长度与front和rear的值
    for (int i = 0; i < 5; i++) {
        DeQueue(Q, m);
    }
    EnQueue(Q, 11);
    EnQueue(Q, 12);
    printf("连续出队5个整数，再入队2个整数后, 队列长度: %d, 队列头序号: %d, 队列尾序号: %d\n", QueueLength(Q), Q.front, Q.rear);

    //调用函数QueueReverse(Q)，将队列元素逆序重排，然后将队列元素出队并输出。
    QueueReverse(Q);
    printf("队列元素逆序重排后:");
    while (!QueueEmpty(Q)) {
        DeQueue(Q, m);
        printf("%d ", m);
    }
    printf("\n");

    return 0;
	
}





