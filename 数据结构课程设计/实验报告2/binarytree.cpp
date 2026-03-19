#include<stdio.h>
 

#define OK 1

typedef  int  Status;            //声明函数类型名
typedef  char TElemType;  	     //声明结点元素值得类型

typedef  struct BiTNode
{  //定义二叉链表结点类型 

    TElemType  		data;
    struct BiTNode  *lchild, *rchild; 	//指向左右孩子结点的指针

} BiTNode, *BiTree;

//根据先序遍历的字符序列，创建一棵按二叉链表结构存储的二叉树，指针变量T指向二叉树的根结点。
Status CreateBiTree(BiTree &T)
{
	char ch;
	ch = getchar();
	if(ch =='\n')
	{
		return OK;
	}
	if(ch == '#')
		T = NULL;
	else
	{
		T = new BiTNode;
		T->data = ch;
		CreateBiTree(T->lchild);
		CreateBiTree(T->rchild);
	}
	return OK;
}

//递归先序遍历二叉树T，输出访问的结点字符序列;
Status PreOrderTraverse(BiTree T)
{
	if(T)
	{
        printf("%c ", T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
    return OK;
}

//递归中序遍历二叉树T，输出访问的结点字符序列;
Status InOrderTraverse(BiTree T)
{
	if(T)
	{
		InOrderTraverse(T->lchild);
		printf("%c ",T->data);
		InOrderTraverse(T->rchild);
	}
	return OK;
}

//递归后序序遍历二叉树T，输出访问的结点字符序列;
Status PostOrderTraverse(BiTree T)
{
	if(T)
	{
		PostOrderTraverse(T->lchild);
		PostOrderTraverse(T->rchild);
		printf("%c ",T->data);
	}
	return OK;
}

//应用实例1:返回二叉树T度分别为0,1,2的结点数，其中d为结点度数，如TNodes(T,0)返回度为0（即叶子结点）的结点数;
Status TNodes(BiTree T,int d)
{
	if (!T) return 0;
    
    int degree = 0;
    if (T->lchild) degree++;  // 左子树非空，度数加1
    if (T->rchild) degree++;  // 右子树非空，度数加1
	int count = (degree == d) ? 1 : 0;

    return count + TNodes(T->lchild, d) + TNodes(T->rchild, d);
}

//应用实例2:求二叉树T的高度
Status High(BiTree T)
{
	if(T ==NULL) return 0;
	else
	{
		int m = High(T->lchild);
		int n = High(T->rchild);
		if(m > n) return (m+1);
		else return (n+1);
	}
}

//应用实例3:要求根据给定的字符序列（字符不重复），生成一棵二叉树。在二叉树中，左子树所有结点的字符小于根结点的字符，右子树所有结点的字符大于根结点。左右子树也满足这个条件。
void InsertBST(BiTree &T, TElemType ch)
{
    if(T == NULL)
	{
        T = new BiTNode;
        T->data = ch;
        T->lchild = T->rchild = NULL;
    }
	else
	{
        if (ch < T->data)
		{
            InsertBST(T->lchild, ch);
        }else
		{
            InsertBST(T->rchild, ch);
        }
    }
}


Status CreateBST(BiTree &T, const char *chars)
{
    while (*chars != '\0')
	{
        InsertBST(T, *chars);
        chars++;
    }
    return OK;
}

int main() {
    BiTree T = NULL, T1 = NULL, T2 = NULL;
    printf("请输入先序遍历序列创建二叉树 (空结点用#表示):\n");
    CreateBiTree(T);
    
    printf("先序遍历结果：\n");
    PreOrderTraverse(T);
    
    printf("\n中序遍历结果：\n");
    InOrderTraverse(T);
    
    printf("\n后序遍历结果：\n");
    PostOrderTraverse(T);
    
    int d0 = 0, d1 = 0, d2 = 0;
    d0 = TNodes(T, 0);
    d1 = TNodes(T, 1);
    d2 = TNodes(T, 2);
    printf("\n度为0的结点数: %d\n", d0);
    printf("度为1的结点数: %d\n", d1);
    printf("度为2的结点数: %d\n", d2);
    
    CreateBST(T1, "DBFCAEG");
    CreateBST(T2, "ABCDEFG");
    
    printf("\nT1中序遍历：\n");
    InOrderTraverse(T1);
    printf("\nT1的高度: %d\n", High(T1));
    
    printf("\nT2中序遍历：\n");
    InOrderTraverse(T2);
    printf("\nT2的高度: %d\n", High(T2));
    
    return 0;
}




