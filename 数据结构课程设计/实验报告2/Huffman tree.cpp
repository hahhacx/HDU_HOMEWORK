#include <iostream>
#include <cstring>
#include <cstdlib>
using namespace std;

typedef struct
{//定义二叉链表结点类型 
    int weight;
    int parent, lchild, rchild;
} HTNode, *HTree;		//动态分配数组存储赫夫曼树

typedef char **HTCode;		//动态分配字符串数组存储赫夫曼编码表

//初始化赫夫曼树
void InitHTree(HTree &HT, int *w, int n)
{
    int totalNodes = 2 * n - 1;
    HT = new HTNode[totalNodes + 1];

    for (int i = 0; i < n; ++i)
	{
        HT[i].weight = w[i];
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }

    for (int i = n; i <= totalNodes; ++i)
	{
        HT[i].weight = -1;
        HT[i].parent = -1;
        HT[i].lchild = -1;
        HT[i].rchild = -1;
    }

    cout << "赫夫曼树初始化完成，所有结点信息如下：" << endl;
    for (int i = 0; i < totalNodes; ++i)
	{
        cout << "结点 " << i+1
             << "：权重 = " << HT[i].weight
             << ", 双亲 = " << HT[i].parent
             << ", 左孩子 = " << HT[i].lchild
             << ", 右孩子 = " << HT[i].rchild << endl;
    }
}

//选择两个权重最小且双亲为-1的结点
void Select(HTree HT, int totalNodes, int &s1, int &s2)
{
    s1 = s2 = -1;
    for (int i = 0; i < totalNodes; ++i)
	{
        if (HT[i].parent == -1)//结点未选 
		{
            if (s1 == -1 || HT[i].weight < HT[s1].weight)
			{
                s2 = s1;
                s1 = i;
            } else if (s2 == -1 || HT[i].weight < HT[s2].weight)
			{
                s2 = i;
            }
        }
    }
    //s1 < s2 
}

//构造赫夫曼树HT
void CreateHTree(HTree &HT, int n)
{
    int totalNodes = 2 * n - 1;

    for (int i = n; i < totalNodes; ++i)
	{
        int s1, s2;
        Select(HT, i, s1, s2);

        // 构造新结点
        HT[i].weight = HT[s1].weight + HT[s2].weight;
        HT[i].lchild = s1;
        HT[i].rchild = s2;
        HT[s1].parent = i;
        HT[s2].parent = i;
    }

    cout << "赫夫曼树构造完成，所有结点信息如下：" << endl;
    for (int i = 0; i < totalNodes; ++i)
	{
        cout << "结点 " << i+1
             << "：权重 = " << HT[i].weight
             << ", 双亲 = " << HT[i].parent+1
             << ", 左孩子 = " << HT[i].lchild+1
             << ", 右孩子 = " << HT[i].rchild+1 << endl;
    }
}

//生成赫夫曼编码HC
void HTCoding(HTree HT, HTCode &HC, int n)
{
    HC = new char*[n];			//分配存储n个字符编码的编码表空间 
    char *cd = new char[n];  	//分配临时存放每个字符编码的动态数组空间 
    cd[n - 1] = '\0';  			//编码结束符

    for (int i = 0; i < n; ++i)
	{
        int start = n - 1;
        int c = i;
        int f = HT[c].parent;

        while (f != -1)
		{	//从叶子结点向根节点回溯
			--start; 
            if (HT[f].lchild == c)
			{
                cd[start] = '0';
            } else
			{
                cd[start] = '1';
            }
            c = f;
            f = HT[c].parent;
        }

        HC[i] = new char[n - start];
        strcpy(HC[i], &cd[start]);
    }
    delete[] cd;

    cout << "赫夫曼编码生成完成：" << endl;
    for (int i = 0; i < n; ++i)
	{
        cout << "叶子结点 " << i+1 << "（权重 = " << HT[i].weight << "）：编码 = " << HC[i] << endl;
    }
}

int main()
{
    int n = 8;
    int w[] = {5, 29, 7, 8, 14, 23, 3, 11};

    HTree HT = NULL;  // 赫夫曼树
    HTCode HC = NULL;  // 赫夫曼编码

    //初始化赫夫曼树
    InitHTree(HT, w, n);

    //构造赫夫曼树
    CreateHTree(HT, n);

    //生成赫夫曼编码
    HTCoding(HT, HC, n);

    //释放动态内存
    for (int i = 0; i < n; ++i)
	{
        delete[] HC[i];
    }
    delete[] HC;
    delete[] HT;

    return 0;
}

