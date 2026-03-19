#include <iostream>
#include <cstring>

#define INFINITY 32767				//表示最大值∞
#define MAX_VERTEX_NUM 20			//最大顶点数
#define MAX_EDGE_NUM 50				//最大边数
#define OK 1

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的枚举类型
typedef int VRType;
typedef char InfoType;

typedef struct ArcCell
{
    VRType adj;         //对无权图用1或0表示；对带权图，则为权值类型。
    InfoType *info;     //该边或弧相关信息的指针（字符串） 
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef char VertexType;
typedef struct
{
    VertexType vexs[MAX_VERTEX_NUM];  //顶点数组，存放顶点信息
    AdjMatrix arcs;                   //邻接矩阵
    int vexnum, arcnum;               //图中顶点总数与弧数
    GraphKind  kind;                  //图的种类标志
} MGraph;

typedef struct
{
    int start, end;   // 边的两个顶点索引
    VRType weight;    // 权值
} Edge;


//创建4种不同类型图的邻接矩阵存储的通用函数
Status CreateMGraph(GraphKind GKind, MGraph &G, int vexnum, int arcnum, char *vexs, int *arcs)
{
    G.vexnum = vexnum;
    G.arcnum = arcnum;
    G.kind = GKind;

    //初始化 
    for(int i = 0; i < vexnum; i++)
	{
        for(int j = 0; j < vexnum; j++)
		{
            if (GKind == DN || GKind == UDN)//有向网和无向网 
                G.arcs[i][j].adj = INFINITY;  
            else							//有向图和无向图 
                G.arcs[i][j].adj = 0;
            G.arcs[i][j].info = NULL;
        }
    }

    //设置顶点信息
    for(int i = 0; i < vexnum; i++)
	{
        G.vexs[i] = vexs[i];
    }

    //设置边或弧
    for(int i = 0; i < arcnum; i++)
	{
        int v1 = arcs[i * 3];   		//边或弧的起点
        int v2 = arcs[i * 3 + 1];   	//边或弧的终点
        int weight = arcs[i * 3 + 2];  	//权重

        G.arcs[v1][v2].adj = weight;
        if(GKind == UDG || GKind == UDN)
		{
            G.arcs[v2][v1].adj = weight;  //无向图或无向网为对称
        }
    }
    return OK;
}

//输出图G的顶点数组元素和邻接矩阵
Status OutMGraph(MGraph G)
{
    printf("图的类型: ");
    switch (G.kind) {
        case DG: printf("有向图\n"); break;
        case DN: printf("有向网\n"); break;
        case UDG: printf("无向图\n"); break;
        case UDN: printf("无向网\n"); break;
    }
    printf("顶点数组元素: ");
    for (int i = 0; i < G.vexnum; i++) {
        printf("%c ", G.vexs[i]);
    }
    printf("\n邻接矩阵:\n");
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = 0; j < G.vexnum; j++) {
            if (G.arcs[i][j].adj == INFINITY)
                printf("∞ ");
            else
                printf("%d ", G.arcs[i][j].adj);
        }
        printf("\n");
    }
    return OK; 
}

//Prim算法,要求输出每一条最小生成边（例如：（A,B,10））和辅助数组closedge[]的变化过程。
Status MiniSpanTree_Prim(MGraph G, VertexType u)
{
    int uIndex = -1;
    for (int i = 0; i < G.vexnum; i++)
	{
        if (G.vexs[i] == u) {
            uIndex = i;
            break;
        }
    }
    if (uIndex == -1) {
        printf("起始顶点不存在！\n");
        return OK;
    }

    int closedge[MAX_VERTEX_NUM];
    VRType minCost[MAX_VERTEX_NUM];
    bool visited[MAX_VERTEX_NUM] = {false};			//存储过的点集U 

    for (int i = 0; i < G.vexnum; i++)
	{
        closedge[i] = uIndex;
        minCost[i] = G.arcs[uIndex][i].adj;
    }
    visited[uIndex] = true;
    minCost[uIndex] = 0; // 起始顶点自身的权重设为0

    printf("Prim算法最小生成树:\n");
    for (int i = 1; i < G.vexnum; i++)
	{
        VRType minWeight = INFINITY;
        int minIndex = -1;
        for (int j = 0; j < G.vexnum; j++) {
            if (!visited[j] && minCost[j] < minWeight)
			{
                minWeight = minCost[j];
                minIndex = j;
            }
        }
		printf("closedge数组: ");
        for (int j = 0; j < G.vexnum; j++) {
            if (visited[j])
                printf("0 ");
            else if (minCost[j] == INFINITY)
                printf("∞ ");
            else
                printf("%d ", minCost[j]);
        }
        printf("\n");
        if (minIndex == -1) break;
        visited[minIndex] = true;
        printf("(%c, %c, %d)\n", G.vexs[closedge[minIndex]], G.vexs[minIndex], minCost[minIndex]);

        

        for (int j = 0; j < G.vexnum; j++)
		{
            if (!visited[j] && G.arcs[minIndex][j].adj < minCost[j])
			{
                minCost[j] = G.arcs[minIndex][j].adj;
                closedge[j] = minIndex;
            }
        }
    }
    return OK;
}

//Kruskal算法,要求输出排序后的edge数组，然后输出每一条最小生成边和连通分量数组cnvx[]的变化过程。
Status MiniSpanTree_Kruskal(MGraph G)
{
    Edge edges[MAX_EDGE_NUM];				//起始点存的都是下标 
    int Vexset[MAX_VERTEX_NUM];				//顶点所在的连通分量 
    int edgeCount = 0;

    for (int i = 0; i < G.vexnum; i++)
	{
        for (int j = i + 1; j < G.vexnum; j++)
		{
            if (G.arcs[i][j].adj != INFINITY)
			{
                edges[edgeCount++] = {i, j, G.arcs[i][j].adj};
            }
        }
    }

    for (int i = 0; i < edgeCount - 1; i++)
	{
        for (int j = 0; j < edgeCount - i - 1; j++)
		{
            if (edges[j].weight > edges[j + 1].weight)
			{
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }

    printf("排序后的边:\n");
    for (int i = 0; i < edgeCount; i++) {
        printf("(%c, %c, %d)\n", G.vexs[edges[i].start], G.vexs[edges[i].end], edges[i].weight);
    }

    for (int i = 0; i < G.vexnum; i++) Vexset[i] = i;

    printf("Kruskal算法最小生成树:\n");
    for (int i = 0; i < edgeCount; i++)
	{
        int uRoot = edges[i].start, vRoot = edges[i].end;
        while (Vexset[uRoot] != uRoot) uRoot = Vexset[uRoot];
        while (Vexset[vRoot] != vRoot) vRoot = Vexset[vRoot];

        if (uRoot != vRoot)
		{
			printf("连通分量数组: ");
            for (int j = 0; j < G.vexnum; j++)
			{
                int root = j;
                while (Vexset[root] != root) root = Vexset[root];
                printf("%d ", root);
            }
            printf("\n");
            printf("(%c, %c, %d)\n", G.vexs[edges[i].start], G.vexs[edges[i].end], edges[i].weight);
            Vexset[uRoot] = vRoot;

        }
    }
    return OK;
}

int main()
{
	MGraph G;
	char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};
    int arcs[10][3] = {{0, 1, 10}, {0, 2, 12}, {0, 4, 15}, {1, 2, 7}, {1, 3, 5}, {1, 5, 6}, {2, 4, 12}, {2, 5, 8}, {3, 5, 6}, {4, 5, 10}};
	CreateMGraph(UDN, G, 6, 10, vexs, &arcs[0][0]);
    OutMGraph(G);

    printf("\nPrim算法:\n");
    MiniSpanTree_Prim(G, 'A');

    printf("\nKruskal算法:\n");
    MiniSpanTree_Kruskal(G);

    return 0;
}



