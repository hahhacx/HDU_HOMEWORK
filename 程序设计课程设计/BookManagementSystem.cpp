#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define MENU_COUNT 14
#include "administrator.c"
#include "struct.c"



void add_Book();//增加书本信息√ 
void delete_Book();//删除书本信息√ 
void update_Book();//修改书本信息√ 
void search_Book();//查询书本信息√ 
void add_reader();//添加读者信息√ 
void delete_reader();//删除读者信息√ 
void update_reader();//修改读者信息√ 
void search_reader();//查询读者信息√ 
void save_reader();//保存读者信息√ 
void browse_reader();//浏览读者信息√ 
void search_borrow_list();//借阅信息查询√ 
void borrow_book(book *p1,reader *p2);//借书√ 
void return_book(book *p1,reader *p2);//还书√ 
void total_borrow1();//图书借阅统计查询√
void total_borrow2();//读者借阅统计查询√ 
int ReadFromFile1();				//读取BookData 
int ReadFromFile2();				//读取ReaderData
int ReadFromFile3();				//读取BorrowingListData
void denglu1();
void denglu2();
void quit();
void show_oneBook(book *p);//展示书本信息√ 
void yemian();

char str1[101];char str2[101];char str3[101];
reader *pr=NULL;
book *pb=NULL;

void menu() { 
    printf(" \t\t    _          _               _           _         _                    _                  \n");
    printf(" \t\t   /  )     __| |  _ __ ___   (_)  _ __   (_)  ___  | |_   _ __    __ _  | |_    ___    _ __ \n");
    printf(" \t\t  / _  )   / _` | | '_ ` _  ) | | | '_  ) | | / __| | __| | '__|  / _` | | __|  / _  ) | '__|\n");
    printf(" \t\t / ___  ) | (_| | | | | | | | | | | | | | | |  \__ ) | |_  | |    | (_| | | |_  | (_) | | |   \n");
    printf(" \t\t/_/   \_ )   \__,_| |_| |_| |_| |_| |_| |_| |_| |___/  \__ | |_|      \__,_|  \__|    \___ ) |_|   \n"); 
                                                                                              
                                                                                              
    printf("          \t\t\t\t          Administrator                           \n");  
    printf("\n");
    printf("\t\t\t\t\t\t  图书管理系统\n");
    printf("\t\t\t\t *************************************************\n");
    printf("\t\t\t\t *          1 -------- 添加图书信息              *\n");
    printf("\t\t\t\t *          2 -------- 删除图书信息              *\n");
    printf("\t\t\t\t *          3 -------- 修改图书信息              *\n");
    printf("\t\t\t\t *          4 -------- 查询图书信息              *\n");
    printf("\t\t\t\t *          5 -------- 添加读者信息              *\n");
    printf("\t\t\t\t *          6 -------- 删除读者信息              *\n");
    printf("\t\t\t\t *          7 -------- 修改读者信息              *\n");
    printf("\t\t\t\t *          8 -------- 查询读者信息              *\n");
    printf("\t\t\t\t *          9 -------- 保存读者信息              *\n");
    printf("\t\t\t\t *         10 -------- 浏览读者信息              *\n");
    printf("\t\t\t\t *         11 -------- 查询借阅信息              *\n");
    printf("\t\t\t\t *         12 -------- 图书借阅统计              *\n");
    printf("\t\t\t\t *         13 -------- 读者借阅统计              *\n");
    printf("\t\t\t\t *         14 -------- 返回登录界面              *\n");
    printf("\t\t\t\t *          0 -------- 退出系统                  *\n");
    printf("\t\t\t\t *************************************************\n");
}
void menu1() { 
    printf("\t\t\t\t    ____                       _               \n");
    printf("\t\t\t\t   |  _  )   ___    __ _    __| |   ___   _ __ \n");
    printf("\t\t\t\t   | |_) |  / _  ) / _` |  / _` |  / _  )| '__|\n");
    printf("\t\t\t\t   |  _ <  |  __ )| (_| | | (_| | |  __ )| |   \n");
    printf("\t\t\t\t   |_| \_)  (___|  (__, _| ( __,_| (___|  |_|   \n");                                         
    printf("          \t\t\t\t               Reader                           \n");  
    printf("\n");
    printf("\t\t\t\t\t\t  图书管理系统\n");
    printf("\t\t\t\t *************************************************\n");
    printf("\t\t\t\t *          1 -------- 查询图书信息              *\n");
    printf("\t\t\t\t *          2 -------- 借书                      *\n");
    printf("\t\t\t\t *          3 -------- 还书                      *\n");
    printf("\t\t\t\t *          4 -------- 图书借阅统计              *\n");
    printf("\t\t\t\t *          5 -------- 读者借阅统计              *\n");
    printf("\t\t\t\t *          6 -------- 返回登录界面              *\n");
    printf("\t\t\t\t *          0 -------- 退出系统                  *\n");
    printf("\t\t\t\t *************************************************\n");
}


//读者系统 
int Read_System()
{
	system("cls");
    system("color F1");
    menu1();
    printf(">请选择要进行的操作(%d_%d):",0,6);
    int choice;
    int right=0;
    scanf("%d",&choice);
    if(choice<0||choice>6)
    {
        printf(">输入有误!请选择(%d-%d)之间的数字!按任意键重试...",0,6);
        fflush(stdin);   //清空输入缓冲区
        getchar();
    }
    while(choice>=0&&choice<=6)
    {
        if(choice<0||choice>6)
        {
            printf(">输入有误!请选择(%d-%d)之间的数字!按任意键重试...",0,6);
            fflush(stdin);   //清空输入缓冲区
            getchar();
        }
        else
        {
            switch(choice)
            {
                case 0:
                    quit();
                    {
                        break;
                    }
                case 1:
                    search_Book();
                    Read_System();
                case 2:
                	if(Reader_num!=0)
                	{
                		for(pr=head2;pr!=NULL;pr=pr->next)
						{
							if(strcmp(pr->number,str2)==0)
							{
								right=1;
								printf("请输入您需要借阅的书籍：");
								char book1[20];
								scanf("%s",book1);
								for(pb=head1;pb!=NULL;pb=pb->next)
								{
									if(strcmp(pb->bookName,book1)==0)
									{
										borrow_book(pb,pr);
										Read_System();
									}
								}
								printf("查询不到该书的借阅信息！");	
							}
						}
						if(right==0)
						{
							printf("管理员暂未录入您的信息！无权限！\n");
							system("pause");
						}
					}
					else
					{
						printf("无任何读者信息！\n");
						system("pause"); 
					}  
                case 3:
                	if(Reader_num!=0)
                	{
                		for(pr=head2;pr!=NULL;pr=pr->next)
						{
							if(strcmp(pr->number,str2)==0)
							{
								right=1;
								printf("请输入您需要归还的书籍：");
								char book1[20];
								scanf("%s",book1);
								for(pb=head1;pb!=NULL;pb=pb->next)
								{
									if(strcmp(pb->bookName,book1)==0)
									{
										return_book(pb,pr);
										Read_System();
									}
								}	
							}
						}
						if(right==0)
						{
							printf("管理员暂未录入您的读者信息！您暂无无权限！"); 
						}
					}
					else
					{
						printf("无任何读者信息！\n");
						system("pause"); 
					}
                case 4:
                    total_borrow1();
                    Read_System();
                case 5:
                    total_borrow2();
                    Read_System();
                case 6:
                    system("cls");
                    printf("请稍候，正在返回登录页面...\n");
                    Sleep(1500);
                    yemian();
                	
            }
        }
        choice = 0;
    }
    return 0;
}





//管理员系统 
int Administrator_System()
{
	system("cls");
    system("color F1");
    int choice;
    menu();
    printf(">请选择要进行的操作(%d_%d):",0,MENU_COUNT);
    scanf("%d",&choice); 
    while(choice>=0&&choice<=MENU_COUNT)
    {
        if(choice<0||choice>MENU_COUNT)
        {
            printf(">输入有误!请选择(%d-%d)之间的数字!按任意键重试...",0,MENU_COUNT);
            fflush(stdin);   //清空输入缓冲区
            getchar();
        }
        else
        {
            switch(choice)
            {
                case 0:
                    quit();
                    {
                    	fflush(stdin);   //清空输入缓冲区
                        break;
                    }
                case 1:
                    add_Book();
                    Administrator_System();
                case 2:
                    delete_Book();
                    Administrator_System();
                case 3:
                    update_Book();
                    Administrator_System();
                case 4:
                    search_Book();
                    Administrator_System();
                case 5:
                    add_reader();
                    Administrator_System();
                case 6:
                    delete_reader();
                    Administrator_System();
                case 7:
                    update_reader();
                    Administrator_System();
                case 8:
                    search_reader();
                    Administrator_System();
                case 9:
                    save_reader();
                    Administrator_System();
                case 10:
                    browse_reader();
                    Administrator_System();
                case 11:
                    search_borrow_list();
                    Administrator_System();
                case 12:
                	total_borrow1();
					Administrator_System();
				case 13:
                	total_borrow2();
					Administrator_System();
				case 14:
					system("cls");
                    printf("请稍候，正在返回登录页面...\n");
                    Sleep(1500);
                    yemian();
            }
        }
        choice = 0;
    }
    return 0;
}




//主页面 
void yemian() 
{
	system("cls");
    printf("\n"); 
    printf("\t\t\t\t\t      图书管理系统\n");
    printf("\t\t\t\t *************************************\n");
    printf("\t\t\t\t *                                   *\n");
    printf("\t\t\t\t *            你的身份是：           *\n");
    printf("\t\t\t\t *                                   *\n"); 
    printf("\t\t\t\t *          1 -------管理员          *\n");
    printf("\t\t\t\t *                                   *\n"); 
    printf("\t\t\t\t *          2 -------读  者          *\n");
    printf("\t\t\t\t *                                   *\n");  
    printf("\t\t\t\t *************************************\n");
    puts("");
    puts("");
    printf("请输入您的身份：");
    int choice12;
    scanf("%d",&choice12);
    if(choice12==1){
        system("cls");
        denglu1();
        printf("请输入您的密码：");
        scanf("%99s",str1);
        if(strcmp(str1,"123456")==0){
            printf("正在登录请稍等");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".\n");
            printf("登录成功！");
            Sleep(500);
            system("cls");
            Administrator_System();
            }
        else{
            printf("密码错误！！！请重新输入！");
            Sleep(500);
            system("cls");
            denglu1();
            printf("请输入您的密码：");
            scanf("%99s",str1);
            if(strcmp(str1,"123456")==0){
            printf("正在登录请稍等");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".\n");
            printf("登录成功！");
            Sleep(500);
            system("cls");
            Administrator_System();
            }
        } 
    }
    else if(choice12==2)
    {
        system("cls");
        denglu2();
        puts("");
        printf("请输入您的编号："); 
        scanf("%99s",str2);                //读者编号
        printf("请输入您的密码：");
        scanf("%99s",str3);                //读者密码 
        if(strcmp(str3,"123456")==0){
            printf("正在登录请稍等");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".\n");
            printf("登录成功！");
            Sleep(500);
            system("cls");
            Read_System();
            }
        else{
            printf("密码错误！！！请重新输入！");
            Sleep(500);
            system("cls");
            denglu2();
            printf("请输入您的编号："); 
            scanf("%99s",str2);                //读者编号
            printf("请输入您的密码：");
            scanf("%99s",str3);                //读者密码
            if(strcmp(str3,"123456")==0)
            {
            printf("正在登录请稍等");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".");
            Sleep(500);
            printf(".\n");
            printf("登录成功！");
            Sleep(500);
            system("cls");
            Read_System();
            }
        } 
    } 
}



int main(){
	ReadFromFile1();
    ReadFromFile2();
    ReadFromFile3();
    yemian();
    return 0;
}


