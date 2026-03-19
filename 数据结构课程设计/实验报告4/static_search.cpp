#include<iostream>
#include<cstring>
using namespace std;

typedef int KeyType;        //关键字的数据类型

//数据元素（记录）类型定义
typedef struct
{
    KeyType key;      //学号(关键字)
    const char *name; //姓名
    const char *sex;  //性别
    int age;          //年龄
} RecordType;

//静态查找表类型定义
typedef struct
{
    RecordType *Record; //数据元素存储空间的基址
    int length;         //表长度（元素个数）
} SSTable;

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

//顺序查找函数
int SearchSSTable_Seq(SSTable ST, KeyType key, int &c)
{
	c = 0;
	for(int i = 1; i <= ST.length; i++)
	{
		c++;
		if(ST.Record[i].key == key)
		{
			return i;
		}
	}
	return 0;
}

//排序函数
void SortSSTable(SSTable &ST)
{
	for(int i = 1; i <= ST.length - 1; i++)
	{
		for(int j =1; j <= ST.length - i; j++)
		{
			if(ST.Record[j].key > ST.Record[j+1].key)
			{
				swap(ST.Record[j],ST.Record[j+1]);
			}
		}
	}
}

//折半查找函数
int SearchSSTable_Bin(SSTable ST, KeyType key, int &c)
{
	int low = 1, high = ST.length;
    c = 0;
    while(low <= high)
	{
        c++;
        int mid = (low + high) / 2;
        if(ST.Record[mid].key == key)
            return mid;
        else if(ST.Record[mid].key < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main(){

	int i,key;
	int n=11;
	int c;
	int m;

	SSTable ST1;

	//创建静态查找表ST1 
	CreateSSTable(ST1,n);
	printf("学号无序的静态查找表\n");
	TraversSSTable(ST1);

	printf("---顺序查找---");
	printf("\n请输入要查找的学号(输入-1结束)："); 
	scanf("%d",&key); 
	while (key!=-1)
	{
		i=SearchSSTable_Seq(ST1,key,c);
		if(i==0)
			printf("当前要查找的记录不存在！\n");
		else 
		{
			cout << "学号: " << ST1.Record[i].key
			 << "， 姓名: " << ST1.Record[i].name
			 << ",  性别: " << ST1.Record[i].sex
			 << ",  年龄：" << ST1.Record[i].age <<endl; 		//输出找到的记录	
		} 

		printf("查找时比较的次数=%d\n",c);
		printf("\n请输入要查找的学号(输入-1结束)："); //下一次查找 
		scanf("%d",&key); 
	}
	
	int total=0;
	for(i=1;i<=ST1.length;i++)
	{
		key = ST1.Record[i].key;
		i=SearchSSTable_Seq(ST1,key,c);
		total=total+c;
	}
	printf("\n\n顺序查找成功的平均查找长度ASL=%f\n",(float)total/ST1.length);
	printf("\n顺序查找不成功的平均查找长度ASL=%f\n",(float)ST1.length+1);

	//查找表排序 
	SortSSTable(ST1);
	printf("\n学号有序的静态查找表\n");
	TraversSSTable(ST1);

	printf("---折半查找---"); 
	printf("\n请输入要查找的学号(输入-1结束)："); 
	scanf("%d",&key); 
	while (key!=-1)
	{
		i=SearchSSTable_Bin(ST1,key,c);
		if(i==-1)
			printf("当前要查找的记录不存在！\n");
		else
		{
			cout << "学号: " << ST1.Record[i].key
			 << "， 姓名: " << ST1.Record[i].name
			 << ",  性别: " << ST1.Record[i].sex
			 << ",  年龄：" << ST1.Record[i].age <<endl; 		//输出找到的记录	
		} 

		printf("查找时比较的次数=%d\n",c);
		printf("\n请输入要查找的学号(输入-1结束)："); //下一次查找 
		scanf("%d",&key); 
	}
	
	total=0;
	for(i=1;i<=ST1.length;i++)
	{
		key = ST1.Record[i].key;
		m=SearchSSTable_Bin(ST1,key,c);
		total=total+c;
	}

	printf("\n\n折半查找成功的平均查找长度ASL=%f\n",(float)total/ST1.length);

	total=0;
	for(i=0;i<=ST1.length;i++)
	{

		if(i==0) m=SearchSSTable_Bin(ST1,4,c);
		else 
		{
			key = ST1.Record[i].key;
			m=SearchSSTable_Bin(ST1,ST1.Record[i].key+1,c);
		}
		total=total+c;

	}
	printf("\n折半查找不成功的平均查找长度ASL=%f\n",(float)total/(ST1.length+1));
	
	delete[] ST1.Record;
	return 0;
}


