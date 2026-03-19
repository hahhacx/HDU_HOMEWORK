#include<iostream>
using namespace std;
#define INFINITY 32767                   //表示最大值∞
#define MAX_VERTEX_NUM  20      		 //最大顶点数
#define OK 1

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的枚举类型
typedef int VRType;
typedef char InfoType;  
typedef struct ArcCell
{ 
	VRType adj  ;          //对无权图用1或0表示；对带权图，则为权值类型。
	InfoType *info;       //该边或弧相关信息的指针（字符串） 

} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef char VertexType;

typedef struct
{
	VertexType vexs[MAX_VERTEX_NUM];    //顶点数组，存放顶点信息
	AdjMatrix   arcs;                   //邻接矩阵
	int vexnum, arcnum;           		//图中顶点总数与弧数
	GraphKind  kind;                 	//图的种类标志

} MGraph;      

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

//输出图G中各顶点的度(有向图分入度和出度)
Status VerDegree(MGraph G)
{
	//有向网 
	if(G.kind == DN)
	{
		for(int i = 0; i < G.vexnum; i++)
		{
        	int inDegree = 0, outDegree = 0;
        	for(int j = 0; j < G.vexnum; j++)
			{
            	if(G.arcs[i][j].adj != INFINITY)
				{
                	outDegree++;
            	}
            	if(G.arcs[j][i].adj != INFINITY)
				{
                	inDegree++;
            	}
        	}
        	printf("有向网顶点 %c: 入度 = %d, 出度 = %d\n", G.vexs[i], inDegree, outDegree);
    	}
	}
	//有向图 
    if(G.kind == DG)
	{
		for(int i = 0; i < G.vexnum; i++)
		{
        	int inDegree = 0, outDegree = 0;
        	for(int j = 0; j < G.vexnum; j++)
			{
            	if(G.arcs[i][j].adj != 0)
				{
                	outDegree++;
            	}
            	if(G.arcs[j][i].adj != 0)
				{
                	inDegree++;
            	}
        	}
        	printf("有向图顶点 %c: 入度 = %d, 出度 = %d\n", G.vexs[i], inDegree, outDegree);
    	}
	}
	//无向网 
	if(G.kind == UDN)
	{
		for(int i = 0; i < G.vexnum; i++)
		{
        	int Degree = 0; 
        	for(int j = 0; j < G.vexnum; j++)
			{
            	if(G.arcs[i][j].adj != INFINITY)
				{
                	Degree++;
            	}
        	}
        	printf("无向网顶点 %c: 度 = %d\n", G.vexs[i], Degree);
    	}
	}
	//无向图 
	if(G.kind == UDG)
	{
		for(int i = 0; i < G.vexnum; i++)
		{
        	int Degree = 0; 
        	for(int j = 0; j < G.vexnum; j++)
			{
            	if(G.arcs[i][j].adj != 0)
				{
                	Degree++;
            	}
        	}
        	printf("无向图顶点 %c: 度 = %d\n", G.vexs[i], Degree);
    	}
	}
    return OK;
}


int main() {
    MGraph G;

    // 测试图1：无向图
    char vexs1[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs1[5][3] = {{0, 1, 1}, {0, 2, 1}, {0, 3, 1}, {1, 2, 1}, {2, 3, 1}};
    CreateMGraph(UDG, G, 4, 5, vexs1, &arcs1[0][0]);
    OutMGraph(G);
    VerDegree(G);

    // 测试图2：有向图
    char vexs2[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs2[6][3] = {{0, 1, 1}, {1, 0, 1}, {1, 2, 1}, {2, 0, 1}, {2, 1, 1}, {3, 1, 1}};
    CreateMGraph(DG, G, 4, 6, vexs2, &arcs2[0][0]);
    OutMGraph(G);
    VerDegree(G);

    // 测试图3：无向网
    char vexs3[] = {'V0', 'V1', 'V2', 'V3'};
    int arcs3[4][3] = {{0, 1, 56}, {0, 2, 34}, {0, 3, 78}, {2, 3, 25}};
    CreateMGraph(UDN, G, 4, 4, vexs3, &arcs3[0][0]);
    OutMGraph(G);
    VerDegree(G);

    // 测试图4：有向网
    char vexs4[] = {'V0', 'V1', 'V2'};
    int arcs4[3][3] = {{0, 2, 50}, {1, 2, 45}, {2, 0, 64}};
    CreateMGraph(DN, G, 3, 3, vexs4, &arcs4[0][0]);
    OutMGraph(G);
    VerDegree(G);

    return 0;
}





 
