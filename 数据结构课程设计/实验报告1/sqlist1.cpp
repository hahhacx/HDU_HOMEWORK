#include <stdio.h>
#include <stdlib.h>

#define OK 		1
#define ERROR   0
#define TRUE	1
#define FALSE	0
#define INFEASIBLE 	-1
#define OVERFLOW	-2
#define MAXSIZE    100

#define LIST_INIT_SIZE 10
#define LISTINCREMENT  5

typedef int Status;   //定义函数类型名

typedef int ElemType;//定义ElemType类型

typedef struct {	 
    ElemType *elem;  //线性表数据存储空间基址 
    int length;
    int listsize;
}SqList;// LIST为用户定义的线性表类型




//初始化空线性表
Status InitList(SqList &L){ 
	L.elem = new ElemType[MAXSIZE];
	if(!L.elem) return OVERFLOW;
	L.length = 0;
	L.listsize = LIST_INIT_SIZE;
	return OK;
}

//清空线性表元素 
Status ClearList(SqList &L){ 

  L.length = 0;
  return OK;
} 

//求出线性表长度
int ListLength(SqList L){ 
  return L.length;
}

//向线性表指定位置插入一个新元素
Status ListInsert(SqList &L, int pos, ElemType e){
//pos为插入的（逻辑）位置,e为待插入的数据元素。插入成功则返回1；否则函数返回值为0。
	if(pos < 1 || pos > L.length + 1) return ERROR;
    if(L.length >= L.listsize) 
	{
        ElemType *newElem = (ElemType *)realloc(L.elem, (L.listsize + LISTINCREMENT) * sizeof(ElemType));
        if (!newElem) return OVERFLOW;
        L.elem = newElem;
        L.listsize += LISTINCREMENT;
    }
    for(int i = L.length; i >= pos; i--) 
	{
        L.elem[i] = L.elem[i - 1];
    }
    L.elem[pos - 1] = e;
    L.length++;
    return OK;
} 

//从线性表中删除指定位序的元素
Status ListDelete(SqList &L, int pos, ElemType &item)
{//pos为删除的（逻辑）位置,用item返回被删元素。删除成功则返回1；否则函数返回值为0。
	if(pos < 1 || pos > L.length) return ERROR;
    item = L.elem[pos - 1];
    for(int i = pos; i < L.length; i++) {
        L.elem[i - 1] = L.elem[i];
    }
    L.length--;
    return OK;
}

//获取顺序表中指定位置上的数据元素 
Status GetElem(SqList L,int pos,ElemType &item){
//pos为指定位置,item用于返回找到的数据元素，如果表空，则返回ERROR;
	if(L.length == 0) return ERROR;
	if(pos < 1 || pos > L.length) return ERROR;
    item = L.elem[pos - 1];
    return OK;
}

//从线性表中查找元素，返回第一个与指定值匹配元素位置
int LocateElem(SqList L,ElemType item){
//item为待查找的数据元素
	for(int i = 0; i < L.length; i++) 
	{
        if (L.elem[i] == item) return i + 1;
    }
    return FALSE;

}

//遍历输出线性表
Status TraverseList(SqList L){
	for(int i = 0; i < L.length; i++)
	{
		printf("%d ",L.elem[i]);
	}
	printf("\n");
	return OK;
}

// 集合并运算LA=LA∪LB
Status Union(SqList &La, SqList Lb){
	for(int i = 0; i < Lb.length; i++)
	{
        if(LocateElem(La, Lb.elem[i]) == 0)
		{ 
            ListInsert(La, La.length + 1, Lb.elem[i]);
        }
    }
    return OK;

}

// 集合交运算LA=LA ∩LB
Status Intersection(SqList &La, SqList Lb) {
	SqList temp;
    InitList(temp);
    for(int i = 0; i < La.length; i++)
	{
        if(LocateElem(Lb, La.elem[i]) != 0)
		{
            ListInsert(temp, temp.length + 1, La.elem[i]);
        }
    }
    ClearList(La);
    for(int i = 0; i < temp.length; i++) 
	{
        ListInsert(La, La.length + 1, temp.elem[i]);
    }
    free(temp.elem);
    return OK;
}

// 集合差运算LA=LA-LB
Status Difference(SqList &La, SqList Lb) {
	SqList temp;
    InitList(temp);
    for(int i = 0; i < La.length; i++) 
	{
        if(LocateElem(Lb, La.elem[i]) == 0)
		{
            ListInsert(temp, temp.length + 1, La.elem[i]);
        }
    }
    ClearList(La);
    for(int i = 0; i < temp.length; i++) 
	{
        ListInsert(La, La.length + 1, temp.elem[i]);
    }
    free(temp.elem);
    return OK;
}

// 有序表合并LC=LA+LB
Status MergeList(SqList La, SqList Lb, SqList &Lc) {
	InitList(Lc);
    int i = 0, j = 0;
    while(i < La.length && j < Lb.length)
	{
        if(La.elem[i] < Lb.elem[j])
		{
            ListInsert(Lc, Lc.length + 1, La.elem[i]);
            i++;
        }
		else{
            ListInsert(Lc, Lc.length + 1, Lb.elem[j]);
            j++;
        }
    }
    while(i < La.length)
	{
        ListInsert(Lc, Lc.length + 1, La.elem[i]);
        i++;
    }
    while(j < Lb.length)
	{
        ListInsert(Lc, Lc.length + 1, Lb.elem[j]);
        j++;
    }
    return OK;
}

//去重 
Status Purge(SqList &Lc) {
	if(Lc.length == 0) return OK;
    SqList temp;
    InitList(temp);
    for(int i = 0; i < Lc.length; i++)
	{
        if(LocateElem(temp, Lc.elem[i]) == 0)
		{
            ListInsert(temp, temp.length + 1, Lc.elem[i]);
        }
    }
    ClearList(Lc);
    for(int i = 0; i < temp.length; i++)
	{
        ListInsert(Lc, Lc.length + 1, temp.elem[i]);
    }
    free(temp.elem);
    return OK;

} 

//主函数 
int main(){
	SqList LA,LB,LC;				//定义顺序表变量
	ElemType Array1[]={2,8,27,39,66,77,89};		//顺序表LA的元素 
	ElemType Array2[]={6,18,27,59,65,77,89,120,140};//顺序表LB的元素 
	ElemType item;
	int i;
		
	//0.数据准备
	//0-1初始化线性表LA,LB
	InitList(LA);
	InitList(LB);

	//0-2生成顺序表LA,LB
	for(i = 0; i < 7; i++)
	{
		ListInsert(LA, LA.length + 1, Array1[i]);
	}
	for(i = 0; i < 9; i++)
	{
		ListInsert(LB, LB.length + 1, Array2[i]);
	}

	//0-3输出顺序表表元素
	printf("LA: ");
    TraverseList(LA);
    printf("LB: ");
    TraverseList(LB);
	
	//1. 计算LA=LA∪LB，并输出结果
	Union(LA, LB);
    printf("LA ∪ LB: ");
    TraverseList(LA);

	ClearList(LA); 
	for(i = 0; i < 7; i++)
	{
		ListInsert(LA, LA.length + 1, Array1[i]);
	}
	//2.计算LA=LA ∩LB，并输出结果
	Intersection(LA, LB);
    printf("LA ∩ LB: ");
    TraverseList(LA);
	
	ClearList(LA); 
	for(i = 0; i < 7; i++)
	{
		ListInsert(LA, LA.length + 1, Array1[i]);
	}
	//3.计算LA=LA-LB，并输出结果
	Difference(LA, LB);
    printf("LA - LB: ");
    TraverseList(LA);
	
	ClearList(LA); 
	for(i = 0; i < 7; i++)
	{
		ListInsert(LA, LA.length + 1, Array1[i]);
	}
	//4.有序表合并LC=LA+LB，并输出结果
	MergeList(LA, LB, LC);
    printf("LC : ");
    TraverseList(LC);
	
	//5.去掉LC重复多余的元素 ，并输出结果
	Purge(LC);
    printf("LC (去重后): ");
    TraverseList(LC);
    
    free(LA.elem);
    free(LB.elem);
    free(LC.elem);
    
	return 0;
}

