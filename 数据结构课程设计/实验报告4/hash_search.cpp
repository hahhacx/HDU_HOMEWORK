#include <stdio.h>
#include <stdlib.h>


#define SUCCESS 1			//查找成功
#define UNSUCCESS 0			//查找不成功 
typedef int KeyType;			//关键字的数据类型

//"线性探测再散列"哈希表的类型定义
typedef struct
{
    KeyType *key;     //哈希表的基址（一维数组，数据元素只用一个关键字）
    int count;        //元素个数
    int size;         //哈希表的长度
} HashTable;

//"链地址法"中同义词结点的类型定义
typedef struct KeyNode
{
    KeyType key;        			//数据元素的关键字
    struct KeyNode *next; 			//指向下一个同义词结点的指针
} KeyNode, *KeyLink;

//"链地址法"哈希表的类型定义
typedef struct
{
    KeyLink *head;     				//哈希表的基址（一维指针数组，每个元素是同义词单链表的头指针）
    int count;         				//元素个数
    int size;          				//哈希表的长度（哈希函数“除留余数法”的除数）
} HashLink;

//哈希函数
int Hash(KeyType key)
{
    return key % 13;  				//除留余数法
}

//创建哈希表的函数
void CreateHashTable(HashTable &HT, int HT_Length, KeyType key[], int KeyNum)
{
	HT.key = new KeyType[HT_Length + 1];
	HT.size = HT_Length;
	HT.count = 0;
	
	for(int i = 0; i < HT_Length; i++)
	{
		HT.key[i] = -1;
	}
	
	for(int i = 0; i < KeyNum; i++)
	{
		int index = Hash(key[i]);
		while(HT.key[index] != -1)
		{
			index = (index + 1) % HT_Length;
		}
		HT.key[index] = key[i];
		HT.count++;
	}
}

//哈希查找函数
int SearchHashTable(HashTable HT, KeyType key, int &p, int &c)
{
	int index = Hash(key);
    c = 0;
    p = -1;
    
    while (HT.key[index] != -1)
    {
    	if(HT.key[index] == key)
		{
            p = index;
            c++;
            return SUCCESS;
        }
        index = (index + 1) % HT.size;
        c++;
	}
	c++;
	return UNSUCCESS;
}

//创建哈希表的函数
void CreateHashLink(HashLink &HL, int HL_Length, KeyType key[], int KeyNum)
{
	HL.size = HL_Length;
    HL.count = 0;
    HL.head = (KeyLink *)malloc(sizeof(KeyLink) * HL_Length);

    for(int i = 0; i < HL_Length; i++)
	{
        HL.head[i] = NULL;
    }

    for(int i = 0; i < KeyNum; i++)
	{
        int index = Hash(key[i]);
        KeyLink newNode = (KeyLink)malloc(sizeof(KeyNode));
        newNode->key = key[i];
        newNode->next = HL.head[index];
        HL.head[index] = newNode;
        HL.count++;
    }
}

//哈希查找函数
int SearchHashLink(HashLink HL, KeyType key, KeyLink &p, int &c)
{
	int index = Hash(key);
    p = HL.head[index];
    c = 0;

    while(p != NULL)
	{
        if(p->key == key)
		{
			c++;
            return SUCCESS;
        }
        c++;
        p = p->next;
    }
	c++;
    p = NULL;
    return UNSUCCESS;
}

//输出哈希表数据的函数
void OutHashLink(HashLink HL)
{
    for(int i = 0; i < HL.size; i++)
	{
        printf("Hash[%d]: ", i);
        KeyLink current = HL.head[i];
        while (current != NULL)
		{
            printf("%d -> ", current->key);
            current = current->next;
        }
        printf("^\n");
    }
}

int main()
{

	int i,j,k,total;
	int keys[12]={19,14,23,1,68,20,84,27,55,11,10,79};
	int keys1[13]={26,40,15,29,30,18,32,46,60,74,36,24,38};
	int n=12,n1=13; 
	int HT_Length=16;

	HashTable HT;
	printf("关键字表:\n");
	for(i=0;i<n;i++) printf("%2d ",keys[i]);

	CreateHashTable(HT, HT_Length, keys, n);
	printf("\n\n线性探测再散列哈希表:\n");
	for(i=0;i<HT_Length;i++) printf("%d ",HT.key[i]);
	printf("\n");

	total=0;
	for(i=0;i<n;i++)
	{
		printf("\n查找关键字=%2d",keys[i]);
		SearchHashTable(HT, keys[i], j, k);
		total=total+k;
		printf("  所在哈希表下标=%2d",j);
		printf("  关键字比较次数=%2d",k);
	}
	printf("\n\n查找成功ASL=%f\n",(float)total/n);
	
	total=0;
	for(i=0;i<n1;i++)
	{
		printf("\n查找关键字=%2d",keys1[i]);
		SearchHashTable(HT, keys1[i], j, k);
		total=total+k;
		printf("  所在哈希表下标=%2d",j);
		printf("  关键字比较次数=%2d",k);
	}
	printf("\n\n查找不成功ASL=%f\n",(float)total/n1);

	
	HashLink HL;
	KeyLink p;
	CreateHashLink(HL,13, keys, n);
	printf("\n链地址法哈希表:\n");
	OutHashLink(HL);

	total=0;
	for(i=0;i<n;i++)
	{
		printf("\n查找关键字=%2d",keys[i]);
		SearchHashLink(HL, keys[i], p, k);
		total=total+k;
		printf("  所在哈希表下标=%2d",(keys[i]) % 13);
		printf("  关键字比较次数=%2d",k);

	}
	printf("\n\n查找成功ASL=%f\n",(float)total/n);

	total=0;
	for(i=0;i<n1;i++)
	{
		printf("\n查找关键字=%2d",keys1[i]);
		SearchHashLink(HL, keys1[i], p, k);
		total=total+k;
		printf("  所在哈希表下标=%2d",(keys1[i]) % 13);
		printf("  关键字比较次数=%2d",k);

	}

	printf("\n\n查找不成功ASL=%f\n",(float)total/n1);

	return 1;

}











 
