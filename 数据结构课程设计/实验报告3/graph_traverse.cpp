#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

#define MAX_VERTEX_NUM 20  // 最大顶点数
#define OK 1

typedef int Status;
typedef enum { DG, DN, UDG, UDN } GraphKind;  // 图的类型
typedef char VertexType;                     // 顶点数据的类型 (字符）
bool visited[MAX_VERTEX_NUM];

// 边(弧)结点数据类型
typedef struct ArcNode
{
    int adjvex;                 // 该边(弧)所依附(指向)的顶点在顶点数组的下标
    struct ArcNode* nextarc;    // 指向下一条边(弧)的指针
    int weight;                 // 边(弧)的权值，无权图其值为0
} ArcNode;

// 顶点数组数据类型
typedef struct VNode
{
    VertexType data;        // 顶点数据
    ArcNode* firstarc;      // 指向第一条依附该顶点的边(弧)的指针
} VNode, AdjList[MAX_VERTEX_NUM];

// 图的邻接表数据类型
typedef struct
{
    AdjList vertices;             // 图的顶点数组
    int vexnum, arcnum;           // 图的顶点数和边(弧)数
    GraphKind kind;               // 图的类型
} ALGraph;


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
        int v1 = arcs[i * 3];   		// 弧尾或边的一个顶点
        int v2 = arcs[i * 3 + 1];  		// 弧头或边的另一个顶点
        int weight = arcs[i * 3 + 2];   // 权值

        ArcNode *node = new ArcNode;
        node->adjvex = v2;
        node->weight = weight;
        node->nextarc = NULL;

        //插入到v1的邻接表中，按序号从小到大插入
        ArcNode **p = &G.vertices[v1].firstarc;
        while (*p && (*p)->adjvex < v2)
        {
            p = &((*p)->nextarc);
        }
        node->nextarc = *p;
        *p = node;

        //如果是无向图，添加对称边(v2, v1)
        if (GKind == UDG || GKind == UDN)
		{
            ArcNode *symNode = new ArcNode;
            symNode->adjvex = v1;
            symNode->weight = weight;
            symNode->nextarc = NULL;

            ArcNode **q = &G.vertices[v2].firstarc;
            while (*q && (*q)->adjvex < v1)
            {
                q = &((*q)->nextarc);
            }
            symNode->nextarc = *q;
            *q = symNode;
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


//深度优先遍历函数
Status DFS(ALGraph G, int v)
{
    cout << G.vertices[v].data << " ";
    visited[v] = true;
	
	ArcNode* p = G.vertices[v].firstarc;
	while(p != NULL)
	{
		int w = p->adjvex;
		if(!visited[w])
		{
			DFS(G,w);
		}
		p = p->nextarc;
	}
	return OK;
}
Status DFSTraverse(ALGraph G)
{
	memset(visited, false, sizeof(visited));
    cout << "深度优先遍历序列：";
    for (int i = 0; i < G.vexnum; i++)
	{
        if (!visited[i])
		{
            DFS(G, i);
        }
    }
    cout << endl;
    return OK;
}


//广度优先遍历函数
Status BFSTraverse(ALGraph G)
{
    memset(visited, false, sizeof(visited));
    queue<int> Q;
    cout << "广度优先遍历序列：";

    for (int i = 0; i < G.vexnum; i++)
	{
        if(!visited[i])
		{	
			cout << G.vertices[i].data << " ";
            visited[i] = true;
            Q.push(i);

            while(!Q.empty())
			{
                int v = Q.front();
                Q.pop();
                for (ArcNode* p = G.vertices[v].firstarc; p; p = p->nextarc) 
				{
                    if (!visited[p->adjvex])
					{
						cout << G.vertices[p->adjvex].data << " ";
                        visited[p->adjvex] = true;
                        Q.push(p->adjvex);
                    }
                }
            }
        }
    }
    cout << endl;
    return OK;
}



int main()
{
    ALGraph G;
    
    //无向图 
    char vexs1[] = {'C0', 'C1', 'C2', 'C3', 'C4', 'C5'};
    int arcs1[7][3] = {{0, 1, 1}, {0, 2, 1}, {1, 3, 1}, {1, 4, 1}, {2, 5, 1}, {3, 4, 1}, {4, 5, 1}};
    CreateALGraph(UDG, G, 6, 7, vexs1, (int *)arcs1);
    OutALGraph(G);
    
    // 深度优先遍历
    DFSTraverse(G);

    // 广度优先遍历
    BFSTraverse(G);

    return 0;
}
    
    
    
    
    
    

