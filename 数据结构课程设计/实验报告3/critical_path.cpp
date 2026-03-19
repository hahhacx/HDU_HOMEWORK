#include <iostream>
#include <cstring>
using namespace std; 

#define MAX_VERTEX_NUM 20
#define INFINITY 32767
#define OK 1
#define ERROR 0

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的枚举类型
typedef int VRType;
typedef char VertexType;

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
typedef struct {
    AdjList vertices;					//图的顶点数组 
    int vexnum, arcnum;					//图的顶点数和边(弧)数 
    GraphKind kind;                     //图的类型 
} ALGraph;

int ve[MAX_VERTEX_NUM]; // 最早发生时间
int vl[MAX_VERTEX_NUM]; // 最晚发生时间
int stack[MAX_VERTEX_NUM];
int top = -1;

// 查找顶点索引
int LocateVex(ALGraph G, VertexType v) {
    for (int i = 0; i < G.vexnum; i++) {
        if (G.vertices[i].data == v) return i;
    }
    return -1;
}

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
        node->nextarc = G.vertices[v1].firstarc;
        G.vertices[v1].firstarc = node;

        //如果是无向图，添加对称边(v2, v1)
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

//拓扑排序
Status TopologicalSort(ALGraph G, int* topoOrder)
{
    int indegree[MAX_VERTEX_NUM] = {0};
    int stack[MAX_VERTEX_NUM], top = -1;

    //计算各顶点的入度
    for (int i = 0; i < G.vexnum; i++)
	{
        for (ArcNode* arc = G.vertices[i].firstarc; arc; arc = arc->nextarc)
		{
            indegree[arc->adjvex]++;
        }
    }

    //入度为 0 的顶点入栈
    for (int i = 0; i < G.vexnum; i++)
	{
        if (indegree[i] == 0)
		{
            stack[++top] = i;
        }
    }

    int count = 0;					//对输出顶点计数 
    while (top != -1)
	{
        int u = stack[top--];
        topoOrder[count++] = u;

        for (ArcNode* arc = G.vertices[u].firstarc; arc; arc = arc->nextarc) {
            if (--indegree[arc->adjvex] == 0)
			{
                stack[++top] = arc->adjvex;
            }
        }
    }

	if (count < G.vexnum)
	{
		return ERROR;
	} 
    else return OK;
}

// 关键路径算法
Status CriticalPath(ALGraph G)
{
    int topoOrder[MAX_VERTEX_NUM];
    if (!TopologicalSort(G, topoOrder)) {
        cout << "图有环，无法求关键路径！\n";
        return ERROR;
    }

    //初始化 ve
    for (int i = 0; i < G.vexnum; i++) {
        ve[i] = 0;
    }

    //计算 ve (最早发生时间)
    for (int i = 0; i < G.vexnum; i++) {
        int u = topoOrder[i];
        for (ArcNode* arc = G.vertices[u].firstarc; arc; arc = arc->nextarc) {
            if (ve[u] + arc->weight > ve[arc->adjvex]) {
                ve[arc->adjvex] = ve[u] + arc->weight;
            }
        }
    }

    //初始化 vl
    for (int i = 0; i < G.vexnum; i++) {
        vl[i] = ve[G.vexnum - 1];
    }

    //计算 vl (最晚发生时间)
    for (int i = G.vexnum - 1; i >= 0; i--) {
        int u = topoOrder[i];
        for (ArcNode* arc = G.vertices[u].firstarc; arc; arc = arc->nextarc) {
            if (vl[arc->adjvex] - arc->weight < vl[u]) {
                vl[u] = vl[arc->adjvex] - arc->weight;
            }
        }
    }

    //输出关键路径
    cout << "关键路径: \n";
    for (int u = 0; u < G.vexnum; u++) {
        for (ArcNode* arc = G.vertices[u].firstarc; arc; arc = arc->nextarc) {
            int ee = ve[u];
            int el = vl[arc->adjvex] - arc->weight;
            if (ee == el) {
                cout << G.vertices[u].data << " -> " << G.vertices[arc->adjvex].data << " (权值: " << arc->weight << ")\n";
            }
        }
    }
}

int main() {
    ALGraph G;
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'};
    int arcs[15][3] = {
        {0, 1, 8}, {0, 2, 6}, {0, 4, 7}, {1, 3, 3}, {2, 3, 10}, {2, 6, 9}, {3, 5, 4}, {3, 8, 19}, {4, 6, 9}, {4, 7, 13}, {5, 9, 14}, {6, 7, 2}, {6, 8, 8}, {7, 8, 12}, {8, 9, 10}
    };

    CreateALGraph(DN, G, 10, 15, vexs, (int *)arcs);
    OutALGraph(G);
    CriticalPath(G);
    return 0;
}

