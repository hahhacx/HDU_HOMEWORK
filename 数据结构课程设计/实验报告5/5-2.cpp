#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAXSIZE 20  		//文件中记录个数的最大值
typedef int KeyType;  		//定义关键字类型为整数类型

// 记录类型
typedef struct {
    KeyType key;   			//学号（记录的关键字）
    const char* name;  		//姓名
    const char* sex;   		//性别
    int age;           		//年龄
} RecordType;

// 记录表的类型
typedef struct {
    RecordType r[MAXSIZE + 1];  // r[0]闲置或用作“哨兵”单元
    int length;  // 记录的个数
} RecordTable;

int ccount = 0;  // 比较次数
int mcount = 0;  // 交换次数
int pcount = 0;  // 划分次数
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
void BubbleSort(RecordTable& RT) {
    int i, j;
    RecordType temp;
    for (i = 1; i <= RT.length - 1; i++) {
        for (j = 1; j <= RT.length - i; j++) {
            ccount++;  // 比较次数
            if (RT.r[j].key > RT.r[j + 1].key) {
                // 交换
                temp = RT.r[j];
                RT.r[j] = RT.r[j + 1];
                RT.r[j + 1] = temp;
                mcount++;  // 交换次数
            }
        }
    }
}
void QuickSort(RecordTable& RT, int low, int high) {
    if (low >= high) return;

    int i = low, j = high;
    RecordType pivot = RT.r[low];  // 选择第一个元素为枢轴
    while (i < j) {
        // 从右侧找到比枢轴小的记录
        while (i < j && RT.r[j].key >= pivot.key) {
            j--;
            ccount++;  // 比较次数
        }
        // 从左侧找到比枢轴大的记录
        while (i < j && RT.r[i].key <= pivot.key) {
            i++;
            ccount++;  // 比较次数
        }
        // 交换两者
        if (i < j) {
            RecordType temp = RT.r[i];
            RT.r[i] = RT.r[j];
            RT.r[j] = temp;
            mcount++;  // 交换次数
        }
    }
    // 将枢轴放到正确的位置
    RT.r[low] = RT.r[i];
    RT.r[i] = pivot;
    mcount++;  // 交换次数

    pcount++;  // 划分次数
    // 输出每次划分后的记录
    printf("划分 #%d:\n", pcount);
    OutRecordTable(RT);

    // 递归处理左右子表
    QuickSort(RT, low, i - 1);
    QuickSort(RT, i + 1, high);
}
int main() {
    RecordTable RT1;

    // 创建待排序记录表
    CreateRecordTable(RT1);

    // 输出原始记录
    printf("原始记录表：\n");
    OutRecordTable(RT1);

    // 使用冒泡排序
    printf("\n使用冒泡排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置交换次数
    BubbleSort(RT1);
    OutRecordTable(RT1);
    printf("比较次数: %d, 交换次数: %d\n", ccount, mcount);

    // 恢复待排序记录表
    CreateRecordTable(RT1);

    // 使用快速排序
    printf("\n使用快速排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置交换次数
    pcount = 0;  // 重置划分次数
    QuickSort(RT1, 1, RT1.length);
    OutRecordTable(RT1);
    printf("比较次数: %d, 交换次数: %d\n", ccount, mcount);

    return 0;
}
