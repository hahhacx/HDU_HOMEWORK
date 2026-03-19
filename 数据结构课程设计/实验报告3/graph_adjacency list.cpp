#include<stdio.h>
#include<iostream>
using namespace std;

#define MAX_VERTEX_NUM  20//最大顶点数
#define OK 1
typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的类型
typedef char VertexType; 				 //顶点数据的类型(字符） 

//边(弧)结点数据类型 
typedef struct ArcNode
{
	int	adjvex ;       				//该边(弧)所依附(指向)的顶点在顶点数组的下标
	struct ArcNode *nextarc;        //指向下一条边(弧)的指针
	int	weight;        				//边(弧)的权值，无权图其值为0
}ArcNode;

//顶点数组数据类型 
typedef struct VNode
{
	VertexType  data;           //顶点数据 
	ArcNode  *firstarc;         //指向第一条依附该顶点的边(弧)的指针
}VNode, AdjList[MAX_VERTEX_NUM];

//图的邻接表数据类型 
typedef  struct
{
	AdjList	vertices;                   //图的顶点数组 
	int	vexnum, arcnum;     			//图的顶点数和边(弧)数 
	GraphKind kind;                     //图的类型 
}ALGraph;


//创建4种不同类型图的邻接表存储的通用函数
Status CreateALGraph(GraphKind GKind, ALGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
    G.vexnum = vexnum;
    G.arcnum = arcnum;
    G.kind = GKind;

    //初始化
    for(int i = 0; i < vexnum; i++)
	{
        G.vertices[i].data = vexs[i];
        G.vertices[i].firstarc = NULL;
    }

    
    for(int i = 0; i < arcnum; i++)
	{
        int v1 = arcs[i * 3];   		//弧尾或边的一个顶点
        int v2 = arcs[i * 3 + 1];  		//弧头或边的另一个顶点
        int weight = arcs[i * 3 + 2];   //权值

        ArcNode *node = new ArcNode;
        node->adjvex = v2;
        node->weight = weight;
        node->nextarc = G.vertices[v1].firstarc;
        G.vertices[v1].firstarc = node;

        //如果是无向图或网，添加对称边(v2, v1)
        if (GKind == UDG || GKind == UDN)
		{
            ArcNode *symNode = new ArcNode;
            symNode->adjvex = v1;
            symNode->weight = weight;
            symNode->nextarc = G.vertices[v2].firstarc;
            G.vertices[v2].firstarc = symNode;
        }
    }
    return OK;
}

//输出图G的顶点数组元素和邻接表结构数据，对于有权图（网），权值加括号表示。
Status OutALGraph(ALGraph G)
{
    printf("图的类型: ");
    switch (G.kind)
	{
        case DG: printf("有向图\n"); break;
        case DN: printf("有向网\n"); break;
        case UDG: printf("无向图\n"); break;
        case UDN: printf("无向网\n"); break;
    }
    printf("邻接表:\n");
    for (int i = 0; i < G.vexnum; i++)
	{
        printf("%c: ", G.vertices[i].data);
        ArcNode *arc = G.vertices[i].firstarc;
        while (arc != NULL)
		{
            printf("-> %c", G.vertices[arc->adjvex].data);
            if (G.kind == DN || G.kind == UDN)
			{
                printf("(%d)", arc->weight);
            }
            arc = arc->nextarc;
        }
        printf("\n");
    }
    return OK;
}


//输出图G中各顶点的度（有向图分入度和出度）
Status VerDegree(ALGraph G)
{
    int inDegree[MAX_VERTEX_NUM] = {0};   // 入度数组
    int outDegree[MAX_VERTEX_NUM] = {0};  // 出度数组

    for(int i = 0; i < G.vexnum; i++)
	{
        ArcNode *arc = G.vertices[i].firstarc;
        while (arc != NULL)
		{
            outDegree[i]++;
            inDegree[arc->adjvex]++;
            arc = arc->nextarc;
        }
    }

    for(int i = 0; i < G.vexnum; i++)
	{
        if (G.kind == DG || G.kind == DN)
		{
            printf("顶点 %c: 入度 = %d, 出度 = %d\n", G.vertices[i].data, inDegree[i], outDegree[i]);
        } else
		{
            printf("顶点 %c: 度数 = %d\n", G.vertices[i].data, outDegree[i]);
        }
    }
    return OK; 
}

int main() {
    ALGraph G;
    
    // 测试图1：无向图
    char vexs1[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs1[5][3] = {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {2, 1, 1}, {2, 3, 1}};
    CreateALGraph(UDG, G, 4, 5, vexs1, (int *)arcs1);
    OutALGraph(G);
    VerDegree(G);

    // 测试图2：有向图
    char vexs2[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs2[6][3] = {{0, 1, 1}, {1, 0, 1}, {1, 2, 1}, {2, 0, 1}, {2, 1, 1}, {3, 1, 1}};
    CreateALGraph(DG, G, 4, 6, vexs2, (int *)arcs2);
    OutALGraph(G);
    VerDegree(G);

	// 测试图3：无向网 
    char vexs3[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs3[4][3] = {{0, 1, 56}, {0, 2, 34}, {0, 3, 78}, {2, 3, 25}};
    CreateALGraph(UDN, G, 4, 4, vexs3, (int *)arcs3);
    OutALGraph(G);
    VerDegree(G);
    
    // 测试图4：有向网 
    char vexs4[] = {'V0', 'V1', 'V2'};
    int arcs4[3][3] = {{0, 2, 50}, {1, 2, 45}, {2, 0, 64}};
    CreateALGraph(DN, G, 3, 3, vexs4, (int *)arcs4);
    OutALGraph(G);
    VerDegree(G);

    return 0;
}



