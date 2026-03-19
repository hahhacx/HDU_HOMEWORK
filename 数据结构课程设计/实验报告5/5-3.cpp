#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAXSIZE 20  		//文件中记录个数的最大值
typedef int KeyType;  		//定义关键字类型为整数类型

// 记录类型
typedef struct {
    KeyType key;   				//学号（记录的关键字）
    const char* name;  			//姓名
    const char* sex;   			//性别
    int age;           			//年龄
} RecordType;

// 记录表的类型
typedef struct {
    RecordType r[MAXSIZE + 1];  // r[0]闲置或用作“哨兵”单元
    int length;  // 记录的个数
} RecordTable;

int ccount = 0;  // 比较次数
int mcount = 0;  // 交换次数
typedef enum { OK = 1, ERROR = 0 } Status;

Status CreateRecordTable(RecordTable& RT) {
    int keys[] = { 56, 19, 80, 5, 21, 64, 88, 13, 37, 75, 92 };
    const char* names[] = { "Zhang", "Wang", "Zhou", "Huang", "Zheng", "Li", "Liu", "Qian", "Sun", "Zhao", "Chen" };
    const char* sexs[] = { "F", "F", "F", "F", "M", "M", "M", "M", "M", "M", "M" };
    int ages[] = { 19, 18, 19, 18, 19, 20, 20, 19, 18, 19, 18 };

    int i, n = 11;
    for (i = 1; i <= n; i++) {
        RT.r[i].key = keys[i - 1];
        RT.r[i].name = names[i - 1];
        RT.r[i].sex = sexs[i - 1];
        RT.r[i].age = ages[i - 1];
    }
    RT.length = n;
    return OK;
}
Status OutRecordTable(RecordTable RT) {
    int i;
    printf("学号 姓名 性别 年龄\n");
    for (i = 1; i <= RT.length; i++) {
        printf("%2d  %-5s  %-1s  %2d\n", RT.r[i].key, RT.r[i].name, RT.r[i].sex, RT.r[i].age);
    }
    return OK;
}
void SelectSort(RecordTable& RT) {
    int i, j, minIndex;
    RecordType temp;
    for (i = 1; i <= RT.length - 1; i++) {
        minIndex = i;
        for (j = i + 1; j <= RT.length; j++) {
            ccount++;  // 比较次数
            if (RT.r[j].key < RT.r[minIndex].key) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            // 交换
            temp = RT.r[i];
            RT.r[i] = RT.r[minIndex];
            RT.r[minIndex] = temp;
            mcount++;  // 交换次数
        }
    }
}
void Heapify(RecordTable& RT, int n, int i) {
    int largest = i;
    int left = 2 * i;
    int right = 2 * i + 1;
    RecordType temp;

    // 比较左子节点与当前节点
    if (left <= n && RT.r[left].key > RT.r[largest].key) {
        ccount++;  // 比较次数
        largest = left;
    }

    // 比较右子节点与当前节点
    if (right <= n && RT.r[right].key > RT.r[largest].key) {
        ccount++;  // 比较次数
        largest = right;
    }

    // 如果最大的节点不是当前节点，交换并继续调整
    if (largest != i) {
        temp = RT.r[i];
        RT.r[i] = RT.r[largest];
        RT.r[largest] = temp;
        mcount++;  // 交换次数

        Heapify(RT, n, largest);
    }
}

void BuildHeap(RecordTable& RT) {
    for (int i = RT.length / 2; i >= 1; i--) {
        Heapify(RT, RT.length, i);
    }
}

void HeapSort(RecordTable& RT) {
    // 建立初始大顶堆
    BuildHeap(RT);

    // 输出初始大顶堆
    printf("初始大顶堆：\n");
    OutRecordTable(RT);

    // 进行堆排序
    for (int i = RT.length; i >= 2; i--) {
        // 交换堆顶与堆尾
        RecordType temp = RT.r[1];
        RT.r[1] = RT.r[i];
        RT.r[i] = temp;
        mcount++;  // 交换次数

        // 调整堆
        Heapify(RT, i - 1, 1);

        // 输出每次调整后的堆
        printf("调整后的堆：\n");
        OutRecordTable(RT);
    }
}
int main() {
    RecordTable RT1;

    // 创建待排序记录表
    CreateRecordTable(RT1);

    // 输出原始记录
    printf("原始记录表：\n");
    OutRecordTable(RT1);

    // 使用简单选择排序
    printf("\n使用简单选择排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置交换次数
    SelectSort(RT1);
    OutRecordTable(RT1);
    printf("比较次数: %d, 交换次数: %d\n", ccount, mcount);

    // 恢复待排序记录表
    CreateRecordTable(RT1);

    // 使用堆排序
    printf("\n使用堆排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置交换次数
    HeapSort(RT1);
    OutRecordTable(RT1);
    printf("比较次数: %d, 交换次数: %d\n", ccount, mcount);

    return 0;
}
