#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAXSIZE 20  // 文件中记录个数的最大值
typedef int KeyType;  // 定义关键字类型为整数类型

//记录类型
typedef struct {
    KeyType key;   			//学号（记录的关键字）
    const char* name;  		//姓名
    const char* sex;   		//性别
    int age;           		//年龄
} RecordType;

//记录表的类型
typedef struct {
    RecordType r[MAXSIZE + 1];  //r[0]闲置或用作“哨兵”单元
    int length;  				//记录的个数
} RecordTable;

int ccount = 0;  // 比较次数
int mcount = 0;  // 移动次数
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
void InsertSort(RecordTable& RT) {
    int i, j;
    RecordType temp;
    for (i = 2; i <= RT.length; i++) {
        temp = RT.r[i];
        j = i - 1;
        while (j >= 1 && RT.r[j].key > temp.key) {
            RT.r[j + 1] = RT.r[j];
            j--;
            mcount++;  // 移动次数
        }
        RT.r[j + 1] = temp;
        ccount++;  // 比较次数
    }
}
void ShellSort(RecordTable& RT) {
    int i, j, k, gap;
    RecordType temp;
    for (gap = RT.length / 2; gap > 0; gap /= 2) {
        for (i = gap + 1; i <= RT.length; i++) {
            temp = RT.r[i];
            j = i - gap;
            while (j >= 1 && RT.r[j].key > temp.key) {
                RT.r[j + gap] = RT.r[j];
                j -= gap;
                mcount++;  // 移动次数
            }
            RT.r[j + gap] = temp;
            ccount++;  // 比较次数
        }
        // 输出每一趟Shell排序后的记录表
        printf("Gap: %d\n", gap);
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

    // 使用简单插入排序
    printf("\n使用插入排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置移动次数
    InsertSort(RT1);
    OutRecordTable(RT1);
    printf("比较次数: %d, 移动次数: %d\n", ccount, mcount);

    // 恢复待排序记录表
    CreateRecordTable(RT1);

    // 使用Shell排序
    printf("\n使用Shell排序结果：\n");
    ccount = 0;  // 重置比较次数
    mcount = 0;  // 重置移动次数
    ShellSort(RT1);
    OutRecordTable(RT1);
    printf("比较次数: %d, 移动次数: %d\n", ccount, mcount);

    return 0;
}
