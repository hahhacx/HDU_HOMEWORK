#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define OK 1
#define FALSE 0

typedef int KeyType; // 关键字数据类型

//数据元素（记录）类型定义
typedef struct
{
    KeyType key;      //学号(关键字)
    char *name; //姓名
    char *sex;  //性别
    int age;          //年龄
} RecordType;

//静态查找表类型定义
typedef struct
{
    RecordType *Record; //数据元素存储空间的基址
    int length;         //表长度（元素个数）
} SSTable;

//动态查找表BST的类型定义
typedef  struct BSTNode
{   
	RecordType  data;
	struct BSTNode *lchild, *rchild; //指向左右孩子结点的指针
}BSTNode, *BSTree;

//创建静态查找表
void CreateSSTable(SSTable &ST, int n)
{
	ST.Record = new RecordType[n+1];
	ST.length = n ;
	
	ST.Record[1] = {56, "Zhang", "F", 19};
    ST.Record[2] = {19, "Wang", "F", 20};
    ST.Record[3] = {80, "Zhou", "F", 19};
    ST.Record[4] = {5, "Huang", "M", 20};
    ST.Record[5] = {21, "Zheng", "M", 20};
    ST.Record[6] = {64, "Li", "M", 19};
    ST.Record[7] = {88, "Liu", "F", 18};
    ST.Record[8] = {13, "Qian", "F", 19};
    ST.Record[9] = {37, "Sun", "M", 20};
    ST.Record[10] = {75, "Zhao", "M", 20};
    ST.Record[11] = {92, "Chen", "M", 20};
}

//遍历函数 
void TraversSSTable(SSTable ST)
{
	for(int i = 1; i <= ST.length; i++)
	{
		cout << "学号: " << ST.Record[i].key
			 << "， 姓名: " << ST.Record[i].name
			 << ",  性别: " << ST.Record[i].sex
			 << ",  年龄：" << ST.Record[i].age <<endl;
	}
}

//插入函数
int InsertDSTable_BST(BSTree &DT, RecordType e)
{
	if(DT == NULL)
	{
		DT = new BSTNode;
		if(DT==NULL) return FALSE;
		DT->data = e;
		DT->lchild = DT->rchild = NULL;
		return OK;
	}
	if(e.key < DT->data.key)
		return InsertDSTable_BST(DT->lchild,e);
	if(e.key > DT->data.key)
		return InsertDSTable_BST(DT->rchild,e);
	return FALSE;
}

//创建动态查找表
void CreateDSTable_BST(BSTree &DT, SSTable &ST)
{
	for(int i = 1;i <= ST.length; i++)
	{
		InsertDSTable_BST(DT,ST.Record[i]);
	}
}

//中序遍历函数
void TraversDSTable_InOrder(BSTree DT)
{
	if(DT!=NULL)
	{
		TraversDSTable_InOrder(DT->lchild);
		printf("学号：%d 姓名：%s 性别：%s 年龄：%d\n", DT->data.key, DT->data.name, DT->data.sex, DT->data.age);
        TraversDSTable_InOrder(DT->rchild);
	} 
}

//查找函数
BSTree SearchDSTable_BST(BSTree DT, KeyType key, int *count)
{
	if(DT == NULL) return NULL;
	(*count)++;
	if(key < DT->data.key)
		return SearchDSTable_BST(DT->lchild, key, count);
	else if(key > DT->data.key)
		return SearchDSTable_BST(DT->rchild, key, count);
	return DT;
}

//删除函数
int DeleteDSTable_BST(BSTree &DT, KeyType key)
{
	if(DT == NULL) return FALSE;
	if(key < DT->data.key)
		return DeleteDSTable_BST(DT->lchild,key);
	else if(key > DT->data.key)
		return DeleteDSTable_BST(DT->rchild,key);
	
	BSTree temp;
    if(DT->lchild == NULL)
	{
        temp = DT;
        DT = DT->rchild;
        free(temp);
    }
	else if (DT->rchild == NULL)
	{
        temp = DT;
        DT = DT->lchild;
        free(temp);
    }
	else
	{
        //找到右子树中的最小节点替代
        temp = DT->rchild;
        while (temp->lchild != NULL)
		{
            temp = temp->lchild;
        }
        DT->data = temp->data;
        return DeleteDSTable_BST(DT->rchild, temp->data.key);
    }
    return OK;	
		
}

int main()
{
	int i,key;
	int n=11;
	SSTable ST1;
	BSTree DT1 = NULL,p;
	int count;

	//创建静态查找表ST1 
	CreateSSTable(ST1,n);
	printf("静态查找表\n");
	TraversSSTable(ST1);

	//创建动态查找表DT1 
	CreateDSTable_BST(DT1,ST1);
	printf("\n动态查找表(中序遍历)\n");
	TraversDSTable_InOrder(DT1);

	//查找动态查找表元素 
	printf("\n---元素查找---\n");
	printf("\n请输入要查找记录的学号(输入-1结束)："); 
	scanf("%d",&key); 
	while (key!=-1)
	{
		count=0;
		p=SearchDSTable_BST(DT1,key,&count);
		if(!p)
			printf("当前要查找的记录不存在！\n");
		else 
			cout << "学号: " << p->data.key
			 << "， 姓名: " << p->data.name
			 << ",  性别: " << p->data.sex
			 << ",  年龄：" << p->data.age <<endl; 		//输出找到的记录
		printf("查找时比较的次数=%d\n",count);
		printf("\n请输入要查找的学号(输入-1结束)："); //下一次查找 
		scanf("%d",&key); 
	} 



	int total=0;
	for(i=1;i<=ST1.length;i++)
	{

		count=0;
		p=SearchDSTable_BST(DT1, ST1.Record[i].key,&count);
		total=total+count;
	}
	printf("\n\n查找成功的平均查找长度ASL=%f\n",(float)total/ST1.length);

	

	total=0;
	for(i=0;i<=ST1.length;i++)
	{
		count=0;
		if(i==0) SearchDSTable_BST(DT1,4,&count);
		else SearchDSTable_BST(DT1,ST1.Record[i].key+1,&count);
		total=total+count;

	}
	printf("\n查找不成功的平均查找长度ASL=%f\n",(float)total/(ST1.length+1));

	

	//构造动态查找表（插入新元素 ） 
	printf("\n---元素插入---\n");
	RecordType e;
	printf("\n请输入要插入的新记录(输入-1结束)\n学号 姓名 性别 年龄\n");

	while(1)
	{
		printf("学号：");
        scanf("%d", &e.key);
        if (e.key == -1) break;
        getchar();  // 清除换行符
        e.name = (char *)malloc(100 * sizeof(char));
        e.sex = (char *)malloc(10 * sizeof(char));
        printf("姓名：");
        scanf("%s", e.name); 
        getchar();  // 清除换行符
        printf("性别：");
        scanf("%s", e.sex);
        getchar();  // 清除换行符
        printf("年龄：");
        scanf("%d", &e.age);
        if (InsertDSTable_BST(DT1, e)) {
            printf("\n插入元素后的动态查找表(中序遍历):\n");
            TraversDSTable_InOrder(DT1);
        } else {
            printf("当前要插入的记录已存在，禁止插入！\n");
        }
        free(e.name);
        free(e.sex);
    }

	// 删除元素
    printf("\n---元素删除---\n");
    printf("\n请输入要删除记录的学号(输入-1结束)：");
    while (1) {
        scanf("%d", &key);
        if (key == -1) break;
        if (DeleteDSTable_BST(DT1, key)) {
            printf("\n删除元素后的动态查找表(中序遍历):\n");
            TraversDSTable_InOrder(DT1);
        } else {
            printf("当前要删除的记录不存在！\n");
        }
        printf("\n请输入要删除记录的学号(输入-1结束)：");
    }

    return 0;
    
}


