#include<stdio.h>
#include<string.h>
#define OK 1
#define ERROR 0

typedef struct
{
    char *ch;       //若是非空串，则按串长分配存储区，否则ch为NULL。
    int  length;    //串长度
} HString;           //堆分配串的类型定义


//将字符串常量chars赋给串变量S；
int StrAssign(HString &S,char *chars)
{
	S.length = strlen(chars);
	S.ch = new char[S.length + 1];
	strcpy(S.ch,chars);
	return OK;
}

//比较串S与T的值 ，若S>T,则返回1；若S=T,则返回0；否则返回-1。
int StrCompare(HString S, HString T)
{
	if(strcmp(S.ch, T.ch) > 0)
		return 1;
	else if(strcmp(S.ch, T.ch) < 0)
		return -1;
	else return 0;
}

//返回串S的长度。
int StrLength(HString S)
{
	return S.length;
}

//将串T1和T2连接在一起赋给串变量S。
int Concat(HString &S, HString T1, HString T2)
{
	S.length = T1.length + T2.length;
	S.ch = new char[S.length + 1];
	strcpy(S.ch,T1.ch);
	strcat(S.ch,T2.ch);
	return OK;
}

//用Sub返回串S的第pos个字符起长度为len的子串，1≤ pos ≤StrLength(S)，1≤len≤StrLength(S)-pos+1
int SubString(HString &sub, HString S, int pos, int len)
{
	if (pos < 1 || pos + len - 1 > S.length) return ERROR;
	sub.length = len;
	sub.ch = new char[len + 1];
	strncpy(sub.ch, S.ch + pos - 1, len);
	sub.ch[len] = '\0';
	return OK;
}

//遍历（输出）字符串S中的各个字符。
int StrTraverse(HString S)
{
	for(int i = 0;i < S.length; ++i)
	{
		printf("%c",S.ch[i]);
	}
	printf("\n");
	return OK;
}

//将串T赋给串S。
int StrCopy(HString &S, HString T)
{
	S.length = T.length;
	S.ch = new char[S.length + 1];
	strcpy(S.ch,T.ch);
	return OK;
}

//返回串T在串S中第pos个字符开始出现的位置，若串T不是串S的子串，则返回0。1≤ pos ≤StrLength(S)。利用SubString()和StrCompare()实现。
int Index(HString S, HString T, int pos)
{
	int n = StrLength(S), m = StrLength(T);
    if(pos < 1 || pos > n) return ERROR;
    HString sub;
    for(int i = pos; i <= n - m + 1; ++i)
	{
        SubString(sub, S, i, m);
        if (StrCompare(sub, T) == 0) return i;
    }
    return 0;
}

//将S串中所有的子串T1用串T2代替。利用SubString()、StrCompare()和Concat()实现。
int Replace(HString &S, HString T1, HString T2)
{
	int pos = Index(S, T1, 1);
    while(pos)
	{
        HString left, right, temp;
        SubString(left, S, 1, pos - 1);
        SubString(right, S, pos + T1.length, S.length - pos - T1.length + 1);
        Concat(temp, left, T2);
        Concat(S, temp, right);
        pos = Index(S, T1, pos + T2.length);
    }
    return OK;
}

//在串S的第pos个字符之前插入串T。1≤ pos ≤StrLength(S)+1。利用SubString()和Concat()实现。
int StrInsert (HString &S, int pos, HString T)
{
	if(pos < 1 || pos > StrLength(S) + 1) return ERROR;
	HString left, right, temp;
    SubString(left, S, 1, pos - 1);
    SubString(right, S, pos, S.length - pos + 1);
    Concat(temp, left, T);
    Concat(S, temp, right);
    return OK;
}

//删除串S中从第pos个字符开始连续的len个字符。1≤ pos ≤StrLength(S)-len+1，利用SubString()和Concat()实现。
int StrDelete(HString &S, int pos, int len)
{
	if(pos < 1 || pos > StrLength(S) - len + 1) return ERROR;
	HString left, right, temp;
    SubString(left, S, 1, pos - 1);
    SubString(right, S, pos + len, S.length - pos - len + 1);
    Concat(S, left, right);
    return OK;
}



int main()
{
    HString T1, T2, T3, S;

	//数据准备
    StrAssign(T1, "Hello");
    StrAssign(T2, "World");
    StrAssign(T3, "Everyone");
	
	//输出字符串字符
    printf("T1: ");
    StrTraverse(T1);
    printf("T2: ");
    StrTraverse(T2);
    printf("T3: ");
    StrTraverse(T3);

	//利用StrCopy()将串T1赋给串S，并输出S的字符。
    StrCopy(S, T1);
    printf("将串T1赋给串S后: ");
    StrTraverse(S);

    //利用StrInsert()将串T2插入到S的尾部，并输出S的字符。
    StrInsert(S, S.length + 1, T2);
    printf("将串T2插入到串S尾部后: ");
    StrTraverse(S);

    //利用StrReplace()将串S中的T1替换成T3，并输出S的字符。
    Replace(S, T1, T3);
    printf("串S中的T1替换成T3后: ");
    StrTraverse(S);

    //利用Index()查找串T2在S中的位置，并输出位置结果。
    int pos = Index(S, T2, 1);
    printf("串T2在S中的位置: %d\n", pos);

    //利用StrDelete()删除S中子串T2，并输出删除后S的结果。
    StrDelete(S, pos, StrLength(T2));
    printf("将串S删除字串T2后: ");
    StrTraverse(S);

    //释放内存
    delete[] T1.ch;
    delete[] T2.ch;
    delete[] T3.ch;
    delete[] S.ch;

    return 0;
}


