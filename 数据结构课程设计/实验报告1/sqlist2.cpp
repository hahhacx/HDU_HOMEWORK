#include <stdio.h>
#include <stdlib.h>

#define OK 		1
#define ERROR   0
typedef int ElemType;  // 声明 ElemType 的类型
typedef int Status;

typedef struct LNode {  // 结点类型定义
    ElemType data;  
    struct LNode *next;
}LNode, *LinkList;



//初始化单链表L
Status InitList(LinkList &L)
{
    L = (LinkList)malloc(sizeof(LNode)); 
    if(L){
        L->next = NULL;
        return OK;
    }
    return ERROR;
}

//清空单链表
Status ClearList(LinkList &L)
{
    LNode *p = L->next;
    while(p){
        LNode *temp = p;
        p = p->next;
        free(temp);
    }
    L->next = NULL;
    return OK;
}

//求单链表长度
int ListLength(LinkList L)
{
    int length = 0;
    LNode *p = L->next; 
    while(p){
        length++;
        p = p->next;
    }
    return length;
}

//获取单链表中第 i 个元素
Status GetElem(LinkList L, int i, ElemType &e)
{
    if(i < 1 || i > ListLength(L)) return ERROR;
    LNode *p = L->next;
    for(int j = 1; j < i; j++)
	{
        p = p->next;
    }
    e = p->data;
    return OK;
}

//查找元素e
LNode *LocateList(LinkList L, ElemType e)
{
    LNode *p = L->next;
    while(p){
        if(p->data == e)return p;
        p = p->next;
    }
    return NULL;
}

//在第i个位置插入元素e
Status ListInsert(LinkList &L, int i, ElemType e)
{
    if (i < 1 || i > ListLength(L) + 1) return ERROR;
    LNode *newNode = (LNode *)malloc(sizeof(LNode));
    newNode->data = e;
    LNode *p = L;
    for(int j = 1; j < i; j++)
	{
        p = p->next;
    }
    newNode->next = p->next;
    p->next = newNode;
    return OK;
}

//删除第i个元素
Status ListDelete(LinkList &L, int i, ElemType &e)
{
    if(i < 1 || i > ListLength(L)) return ERROR;
    LNode *p = L; 
    for(int j = 1; j < i; j++)
	{
        p = p->next;
    }
    LNode *temp = p->next;
    e = temp->data;
    p->next = temp->next;
    free(temp);
    return OK;
}

//遍历输出单链表
Status ListTraverse(LinkList L)
{
    LNode *p = L->next;
    while(p)
	{
        printf("%d->", p->data);
        p = p->next;
    }
    printf("∧\n");
    return OK;
}

//并运算La = La U Lb
Status Union(LinkList &La, LinkList Lb)
{
    LNode *p = Lb->next;
    while (p)
	{
        if(!LocateList(La, p->data))
		{
            ListInsert(La, ListLength(La)+1, p->data);
        }
        p = p->next;
    }
    return OK;
}

//交运算La = La ∩ Lb
Status Intersection(LinkList &La, LinkList Lb)
{
    LNode *p = La->next;
    LinkList temp;
    InitList(temp);
    while(p)
	{
        if(LocateList(Lb, p->data))
		{
            ListInsert(temp, ListLength(temp)+1, p->data);
        }
        p = p->next;
    }
    ClearList(La);
    LNode *tempNode = temp->next;
    while(tempNode)
	{
        ListInsert(La, ListLength(La)+1, tempNode->data);
        tempNode = tempNode->next;
    }
    ClearList(temp);
    return OK;
}

//差运算La = La - Lb
Status Difference(LinkList &La, LinkList Lb)
{
    LNode *p = La->next;
    LinkList temp;
    InitList(temp);
    while(p)
	{
        if(!LocateList(Lb, p->data))
		{
            ListInsert(temp, ListLength(temp)+1, p->data);
        }
        p = p->next;
    }
    ClearList(La);
    LNode *tempNode = temp->next;
    while(tempNode)
	{
        ListInsert(La, ListLength(La)+1, tempNode->data);
        tempNode = tempNode->next;
    }
    ClearList(temp);
    return OK;
}

//合并有序链表
Status MergeList(LinkList La, LinkList Lb, LinkList &Lc)
{
    LNode *p1 = La->next, *p2 = Lb->next;
    InitList(Lc);
    while(p1&&p2)
	{
        if(p1->data <= p2->data)
		{
            ListInsert(Lc, ListLength(Lc)+1, p1->data);
            p1 = p1->next;
        }
		else
		{
            ListInsert(Lc, ListLength(Lc)+1, p2->data);
            p2 = p2->next;
        }
    }
    while(p1)
	{
        ListInsert(Lc, ListLength(Lc)+1, p1->data);
        p1 = p1->next;
    }
    while(p2)
	{
        ListInsert(Lc, ListLength(Lc)+1, p2->data);
        p2 = p2->next;
    }
    return OK;
}

//去重 
Status Purge(LinkList &Lc)
{
    if(Lc->next == NULL) return ERROR;
    LNode *current = Lc->next;
    while(current)
	{
        LNode *n = current;
        while(n->next)
		{
            if(n->next->data == current->data)
			{
                LNode *temp = n->next;
                n->next = temp->next;
                free(temp);
            }
			else
			{
            	n = n->next;
            }
        }
        current = current->next;
    }
    return OK;
}


int main() {
    LinkList LA, LB, LC;
    int i;
    InitList(LA);
    InitList(LB);
    
    int Array1[]={2,8,27,39,66,77,89};
    int Array2[]={6,18,27,59,65,77,89,120,140};
    
    for(i=0; i<7; i++)
	{
        ListInsert(LA,ListLength(LA)+1,Array1[i]);
    }
    
    for(i=0; i<9; i++)
	{
        ListInsert(LB,ListLength(LB)+1,Array2[i]);
    }
    
    printf("LA: ");
    ListTraverse(LA);
    printf("LB: ");
    ListTraverse(LB);
    
    // 集合运算
    Union(LA, LB);
    printf("LA ∪ LB: ");
    ListTraverse(LA);

    ClearList(LA);
    for(i=0; i<7; i++)
	{
        ListInsert(LA,ListLength(LA)+1,Array1[i]);
    }

    Intersection(LA, LB);
    printf("LA ∩ LB: ");
    ListTraverse(LA);

    ClearList(LA);
    for(i=0; i<7; i++)
	{
        ListInsert(LA,ListLength(LA)+1,Array1[i]);
    }

    Difference(LA, LB);
    printf("LA - LB: ");
    ListTraverse(LA);
    
    ClearList(LA);
    for(i=0; i<7; i++)
	{
        ListInsert(LA,ListLength(LA)+1,Array1[i]);
    }

    MergeList(LA, LB, LC);
    printf("LC : ");
    ListTraverse(LC);

    Purge(LC);
    printf("LC (去重后): ");
    ListTraverse(LC);

    
    ClearList(LA);
    ClearList(LB);
    ClearList(LC);

    return 0;
}

