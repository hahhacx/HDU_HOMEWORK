#include <iostream>
using namespace std;

#define OK 1
#define ERROR 0

typedef int Status;		//声明函数类型名
typedef char TElemType;	//声明结点元素值得类型

typedef struct BiThrNode
{
	//定义线索二叉链表的结点结构和类型
    TElemType data;
    struct BiThrNode *lchild, *rchild;
    int LTag, RTag;
} BiThrNode, *BiThrTree;


Status InitBiThrTree(BiThrTree &T)
{
    char ch;
    cin >> ch;
    if (ch == '#') {
        T = NULL;
    } else {
        T = new BiThrNode;
        T->data = ch;
        T->lchild = T->rchild = NULL;

        T->LTag = 1;
        T->RTag = 1;

        InitBiThrTree(T->lchild);
        if (T->lchild) T->LTag = 0;

        InitBiThrTree(T->rchild);
        if (T->rchild) T->RTag = 0;
    }
    return OK;
}

// 中序线索化的递归辅助函数
Status InOrderThreadingHelper(BiThrTree &Thrt, BiThrTree T, BiThrTree &pre)
{
    if (T) {
        // 递归线索化左子树
        InOrderThreadingHelper(Thrt, T->lchild, pre);

        // 前驱线索化
        if (!T->lchild) {
            T->LTag = 1;
            T->lchild = pre;
        }

        // 后继线索化
        if (pre && !pre->rchild) {
            pre->RTag = 1;
            pre->rchild = T;
        }

        // 更新前驱结点
        pre = T;

        // 递归线索化右子树
        InOrderThreadingHelper(Thrt, T->rchild, pre);
    }
    return OK;
}

// 中序线索化
Status InOrderThreading(BiThrTree &Thrt, BiThrTree T)
{
    Thrt = new BiThrNode;
    Thrt->data = '@';
    Thrt->LTag = 0;
    Thrt->RTag = 1;
    Thrt->lchild = T;
    Thrt->rchild = Thrt;

    BiThrTree pre = Thrt;
    InOrderThreadingHelper(Thrt, T, pre);

    // 完成最后一个结点和头结点的互相连接
    if (pre != Thrt) {
        pre->rchild = Thrt;
        pre->RTag = 1;
        Thrt->rchild = pre;
    }

    return OK;
}

// 按中序遍历输出线索二叉树（递归）
Status InOrderTraverse(BiThrTree T)
{
    if (!T) return OK;

    if (T->LTag == 0) {
        InOrderTraverse(T->lchild);
    }

    cout << "| " << T->LTag << " | ";
    if (T->lchild) {
        cout << (T->lchild->data == '@' ? '@' : T->lchild->data) << " | ";
    } else {
        cout << "^ | ";
    }
    cout << T->data << " | ";
    if (T->rchild) {
        cout << (T->rchild->data == '@' ? '@' : T->rchild->data) << " | ";
    } else {
        cout << "^ | ";
    }
    cout << T->RTag << " |" << endl;

    if (T->RTag == 0) {
        InOrderTraverse(T->rchild);
    }

    return OK;
}

int main() {
    BiThrTree T = NULL;   // 普通二叉树根节点
    BiThrTree Thrt = NULL; // 线索二叉树头结点

    cout << "请输入二叉树的先序序列（用#表示空结点）：";
    InitBiThrTree(T);

    cout << "尚未线索化的二叉树（中序遍历）：\n";
    InOrderTraverse(T);

    InOrderThreading(Thrt, T);

    cout << "线索化后的二叉树（中序遍历）：\n";
    InOrderTraverse(Thrt);

    return 0;
}

