#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "struct.c"

int Book_num=0;					//书本数量 
int Reader_num=0;				//读者数量 
struct book *head1 = NULL;		//书本头指针 
struct reader *head2 = NULL;	//读者头指针 
int borrowListNumber = 0;		//借阅表内借阅数量 
struct borlist *head3 = NULL;	//借阅表头指针 

int ReadFromFile1()
{
	FILE *fp;
	int i;
	book *p = NULL;
	if((fp=fopen("BookData.txt","r+"))==NULL)//如果文件不存在
	{
		fp=fopen("BookData.txt","w");//就新建一个文件
		fprintf(fp,"Book_num:%d",Book_num);//在文件中写入当前书籍的数量
	}
	fscanf(fp,"Book_num:%d",&Book_num);//读取当前书籍的数量 存入临时变量中
	if(Book_num!=0) 
	{
		for(i=0; i<Book_num; i++) //读取每一本数的信息
		{
			p = (book*)malloc(sizeof(book));
			fscanf(fp,"\nbookNumber:%d",&p->bookNumber);
			fscanf(fp,"\nbookName:%s",p->bookName);
			fscanf(fp,"\nauthor:%s",p->author);
			fscanf(fp,"\ntypenum:%d",&p->typenum);
			fscanf(fp,"\npublishCompany:%s",p->publishCompany);
			fscanf(fp,"\npublishTime:%s",p->publishTime);
			fscanf(fp,"\nquantity:%d",&p->quantity);
			fscanf(fp,"\nprice:%f",&p->price);
			fscanf(fp,"\nlendNumber:%d",&p->lendNumber);
			p->next = head1;
			head1 = p;//更新头指针
		}
	}
	fclose(fp);
	return 0;
}

int ReadFromFile2()
{
	FILE *fp;
	int i;
	reader *p = NULL;
	if((fp=fopen("ReaderData.txt","r+"))==NULL)//如果文件不存在
	{
		fp=fopen("ReaderData.txt","w");//就新建一个文件
		fprintf(fp,"Reader_num:%d",Reader_num);
	}
	fscanf(fp,"Reader_num:%d",&Reader_num);
	if(Reader_num!=0) 
	{
		for(i=0; i<Reader_num; i++)
		{
			p = (reader*)malloc(sizeof(reader));
			fscanf(fp,"\nnumber:%s",p->number);
			fscanf(fp,"\nloanNumber:%d",&p->loanNumber);
			fscanf(fp,"\nname:%s",p->name);
			fscanf(fp,"\nmaxLoanAmount:%d",&p->maxLoanAmount);
			fscanf(fp,"\nborrowNum:%d",&p->borrowNum);
			fscanf(fp,"\nRlend:%d",&p->Rlend);
			p->next = head2;
			head2 = p;//更新头指针
		}	
	}
	fclose(fp);
	return 0;
}

int ReadFromFile3()
{
	FILE *fp;
	int i;
	borlist *p = NULL;
	if((fp=fopen("BorrowingListData.txt","r+"))==NULL)//如果文件不存在
	{
		fp=fopen("BorrowingListData.txt","w");//就新建一个文件
		fprintf(fp,"borrowListNumber:%d",borrowListNumber);//在文件中写入当前书籍的数量
	}
	fscanf(fp,"borrowListNumber:%d",&borrowListNumber);//读取当前书籍的数量 存入临时变量中
	if(borrowListNumber!=0) 
	{
		for(i=0; i<borrowListNumber; i++) //读取每一本数的信息
		{
			p = (borlist*)malloc(sizeof(borlist));
			fscanf(fp,"\nnumber:%s",p->Rnumber);					//读者编号 
			fscanf(fp,"\nname:%s",p->Rname);						//读者姓名 
			fscanf(fp,"\nbookName:%s",p->Bname);					//书名
			fscanf(fp,"\nauthor:%s",p->Bauthor); 					//书本作者 
			fscanf(fp,"\nborrowDate:%s",p->borrowDate);				//借书日期 
			fscanf(fp,"\nreturnDate:%s",p->returnDate);				//还书日期
			p->next = head3;
			head3 = p;//更新头指针
		}
	}
	fclose(fp);
	return 0;
}


void denglu1() { 
    printf("\n"); 
    printf("\t\t\t\t\t      图书管理系统\n");
    printf("\t\t\t\t *************************************\n");
    printf("\t\t\t\t *                                   *\n");
    printf("\t\t\t\t *                                   *\n"); 
    printf("\t\t\t\t *        请输入你的管理员密码       *\n");
    printf("\t\t\t\t *                                   *\n"); 
    printf("\t\t\t\t *                                   *\n");  
    printf("\t\t\t\t *************************************\n");
}
void denglu2() { 
    printf("\n"); 
    printf("\t\t\t\t\t           图书管理系统\n");
    printf("\t\t\t\t ************************************************\n");
    printf("\t\t\t\t *                                              *\n");
    printf("\t\t\t\t *                                              *\n"); 
    printf("\t\t\t\t *           请输入你的读者编号以及密码         *\n");
    printf("\t\t\t\t *                                              *\n"); 
    printf("\t\t\t\t *                                              *\n");  
    printf("\t\t\t\t ************************************************\n");
}


/*

 实现退出程序功能，在退出之前将建立好的链表中每个节点的信息都
 存到文件中
 
*/ 
void quit()
{
	//保存书本信息 
	book *p1 = NULL;
	FILE *fpw;
	system("cls");
	fpw = fopen("BookData.txt","w+");		//以读写方式打开，数据增加到末尾 
	fprintf(fpw,"Book_num:%d",Book_num);	//在文件中写入当前书籍的数量
	for(p1=head1; p1!=NULL; p1=p1->next)
	{
		fprintf(fpw,"\nbookNumber:%d",p1->bookNumber);
		fprintf(fpw,"\nbookName:%s",p1->bookName);
		fprintf(fpw,"\nauthor:%s",p1->author);
		fprintf(fpw,"\ntypenum:%d",p1->typenum);
		fprintf(fpw,"\npublishCompany:%s",p1->publishCompany);
		fprintf(fpw,"\npublishTime:%s",p1->publishTime);
		fprintf(fpw,"\nquantity:%d",p1->quantity);
		fprintf(fpw,"\nprice:%f",p1->price);
		fprintf(fpw,"\nlendNumber:%d",p1->lendNumber);
		fprintf(fpw,"\n");
	}
	fclose(fpw);
	//保存读者信息 
	reader *p2 = NULL;
    system("cls");
    fpw = fopen("ReaderData.txt","w+");
    fprintf(fpw,"Reader_num:%d\n",Reader_num);
    for(p2=head2; p2!=NULL; p2=p2->next)
    {
        fprintf(fpw,"number:%s\n",p2->number);
        fprintf(fpw,"loanNumber:%d\n",p2->loanNumber);
        fprintf(fpw,"name:%s\n",p2->name);
        fprintf(fpw,"maxLoanAmount:%d\n",p2->maxLoanAmount);
        fprintf(fpw,"borrowNum:%d\n",p2->borrowNum);
        fprintf(fpw,"Rlend:%d\n",p2->Rlend);
        fprintf(fpw,"\n");
    }
    fclose(fpw);
	//保存借阅表信息 
	borlist *p3 = NULL; 
	system("cls");
	fpw = fopen("BorrowingListData.txt","r+");		//以读写方式打开，数据增加到末尾 
	fprintf(fpw,"borrowListNumber:%d\n",borrowListNumber);	//在文件中写入当前书籍的数量
	for(p3=head3;p3!=NULL;p3=p3->next)
	{
		fprintf(fpw,"\nnumber:%s",p3->Rnumber);					//读者编号 
		fprintf(fpw,"\nname:%s",p3->Rname);						//读者姓名 
		fprintf(fpw,"\nbookName:%s",p3->Bname);					//书名
		fprintf(fpw,"\nauthor:%s",p3->Bauthor);					//书本作者 
		fprintf(fpw,"\nborrowDate:%s",p3->borrowDate);			//借书日期 
		fprintf(fpw,"\nreturnDate:%s",p3->returnDate);			//还书日期 
		fprintf(fpw,"\n");
	}
	
	printf("\n");
	printf("感谢使用本图书信息管理系统!\n") ;
	printf("\n");
	fclose(fpw);
	exit(0);
}



//保存读者信息 
void save_reader()
{
    reader *p = NULL;
    FILE *fpw;
    system("cls");
    fpw = fopen("ReaderData.txt","w");
    fprintf(fpw,"Reader_num:%d\n",Reader_num);
    for(p=head2; p!=NULL; p=p->next)
    {
        fprintf(fpw,"number:%s\n",p->number);
        fprintf(fpw,"loanNumber:%d\n",p->loanNumber);
        fprintf(fpw,"name:%s\n",p->name);
        fprintf(fpw,"maxLoanAmount:%d\n",p->maxLoanAmount);
        fprintf(fpw,"borrowNum:%d\n",p->borrowNum);
        fprintf(fpw,"Rlend:%d\n",p->Rlend);
        fprintf(fpw,"\n");
    }
    fclose(fpw);
}


//展示书本信息 
void show_oneBook(book *p)
{
	printf("编号：%d\n",p->bookNumber);
	printf("书名：%s\n",p->bookName);
	printf("作者名：%s\n",p->author);
	printf("分类号：%d\n",p->typenum);
	printf("出版单位：%s\n",p->publishCompany);
	printf("出版时间：%s\n",p->publishTime);
	printf("书本价格：%.2f\n",p->price);
	printf("库存数量：%d\n",p->quantity);
}

//浏览读者信息 
void browse_reader()
{
    reader *p = NULL;
    system("cls");
    for(p=head2; p!=NULL; p=p->next)
    {
        printf("编号：%s\n",p->number);
        printf("借阅号：%d\n",p->loanNumber);
        printf("姓名：%s\n",p->name);
        printf("最大借阅额度：%d\n",p->maxLoanAmount);
        printf("已借阅数量：%d\n",p->borrowNum);
        printf("总借阅次数:%d\n",p->Rlend);
        puts("");
    }
    printf("以上为所有读者的信息\n");
    system("pause");
}

//添加书本信息 
void add_Book()
{
	book *p = NULL;
	FILE *fpw;
	p = (book*)malloc(sizeof(book));
	system("cls");
	printf(">请输入编号：");
	scanf("%d",&p->bookNumber);
	printf(">请输入书名：");
	scanf("%s",p->bookName);
	printf(">请输入作者名：");
	scanf("%s",p->author);
	printf(">请输入分类号：");
	scanf("%d",&p->typenum);
	printf(">请输入出版单位：");
	scanf("%s",p->publishCompany);
	printf(">请输入出版时间：");
	scanf("%s",p->publishTime);
	printf(">请输入书本价格：");
	scanf("%f",&p->price);
	printf(">请输入书本库存量：");
	scanf("%d",&p->quantity);
	p->lendNumber=0; 
	show_oneBook(p);
	int x;
	printf(">确认录入此图书？ 是（1）or 否（0）\n");
	scanf("%d",&x);
	switch(x)
	{
		case 1:
			{
				p->next = head1;
				head1 = p;
				Book_num++;
				printf("正在添加请稍等");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".\n");
				printf("录入成功！按任意键继续...\n");
				fflush(stdin);
				getchar();
				break;
			}
		case 0:
			{
				printf("取消录入！按任意键继续...\n");
				fflush(stdin);
				getchar();
				break;
			}
		default:
			{
				printf("输入错误！请重新输入！\n") ;
				fflush(stdin);
				getchar();
				add_Book();
				return;
			}
	}

}


//添加读者信息 
void add_reader()
{
    reader *p = NULL;
    FILE *fpw;
    p = (reader*)malloc(sizeof(reader));
    system("cls");
    printf(">请输入读者编号：");
    scanf("%99s",p->number);
    printf(">请输入读者借阅号："); 
    scanf("%d",&p->loanNumber);
    printf(">请输入读者姓名：");
    scanf("%99s",p->name);
    printf(">请输入该读者最大借阅额度：");
    scanf("%d",&p->maxLoanAmount);
    p->borrowNum=0;
    p->Rlend=0;
    int x;
    printf(">确认录入此读者信息？ 是（1）or 否（0）\n");
    scanf("%d",&x);
    switch(x)
    {
        case 1:
            {
                p->next = head2;
                head2 = p;
                Reader_num++;
                printf("正在添加请稍等");
                Sleep(500);
                printf(".");
                Sleep(500);
                printf(".");
                Sleep(500);
                printf(".\n");
                printf("录入成功！按任意键继续...\n");
                fflush(stdin);
                getchar();
                break;
            }
        case 0:
            {
                printf("取消录入！按任意键继续...\n");
                fflush(stdin);
                getchar();
                break;
            }
        default:
            {
                printf("输入错误！请重新输入！\n") ;
                fflush(stdin);
                getchar();
                add_reader();
                return;
            }
    }

}

//查询书本信息    1、按图书名称查询    2、按作者名称查询 
void search_Book()
{
	system("cls");
	puts(">按书名查找请按 1\n>按作者名查找请按 2\n>返回请按3");
	int x,found=0;
	char search[20];
	scanf("%d",&x);
	system("cls");
	book *p=head1;
	switch(x)
	{
		case 1:
			{
				puts(">请输入要查找的书名:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->bookName)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						show_oneBook(p);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 2:
			{
				puts(">请输入要查找的作者名:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->author)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						show_oneBook(p);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 3:
			{
				found = 3;
				break;
			}

		default:
			{
				printf("输入错误！请重新搜索！\n") ;
				found = 2;
				break;
			}
	}
	if(found==1)	puts("已找到以上图书，按任意键继续...");
	if(found==0) 	puts("你要查找的图书信息不存在!");
	if(found==2)	search_Book();
	if(found==3)    return;
	fflush(stdin);
	getchar();
} 

//查询读者信息 
void search_reader()
{
	system("cls");
	puts(">按编号查找请按 1\n>按姓名查找请按 2\n>返回请按3");
	int x,found=0;
	char search[101];
	scanf("%d",&x);
	system("cls");
	reader *p=head2;
	switch(x)
	{
		case 1:
			{
				puts(">请输入要查找的读者编号:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->number)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						printf("读者姓名：%s\n",p->name);
						printf("读者编号：%s\n",p->number);
						printf("读者借阅号：%d\n",p->loanNumber);
						printf("读者最大借阅额度：%d\n",p->maxLoanAmount);
						printf("读者已借阅数量：%d\n",p->borrowNum);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 2:
			{
				puts(">请输入要查找的姓名:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->name)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						printf("读者姓名：%s\n",p->name);
						printf("读者编号：%s\n",p->number);
						printf("读者借阅号：%d\n",p->loanNumber);
						printf("读者最大借阅额度：%d\n",p->maxLoanAmount);
						printf("读者已借阅数量：%d\n",p->borrowNum);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 3:
			{
				found = 3;
				break;
			}

		default:
			{
				printf("输入错误！请重新搜索！\n") ;
				found = 2;
				break;
			}
	}
	if(found==1)	puts("已找到以上读者，按任意键继续...");
	if(found==0) 	puts("你要查找的读者信息不存在!");
	if(found==2)	search_reader();
	if(found==3)    return;
	fflush(stdin);
	getchar();
}


//删除书本信息 
void delete_Book()
{
	system("cls");
	int found=0;
	char name[20];
	book *p=head1,*pold;//两个临时变量 pold表示前一个节点 
	puts(">请输入要删除的书名：");
	scanf("%s",name);
	while(p!=NULL)
	{
		if(strcmp(name,p->bookName)==0)//如果找到这本书了
		{
			found = 1;//标记为找到了
			show_oneBook(p);//展示一下这本书
			puts("");
			puts(">您确定要删除这本书吗 是（1）or 否（0）");
			int choice;
			scanf("%d",&choice);
			if(choice)
			{
				if(p==head1)//如果要删掉的是第一本书 
				{
					head1 = head1->next;
					free(p);
					p = head1; 
				}
				else
				{
					pold->next = p->next;
					free(p);
					p = pold->next;
				}
				printf("正在删除请稍等");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".\n");
				printf(">删除成功！\n\n");
				Book_num--;
				
			}
			else break;
		}
		else//如果还没找到这本书，就找下一个节点
		{
			pold = p;
			p = p->next;
		}
	}
	if(found==0)	puts("删除失败，未找到此图书！");
	puts("按任意键继续...");
	fflush(stdin);
	getchar();

}

//删除读者信息 
void delete_reader()
{
	system("cls");
	int found=0;
	char number1[20];
	reader *p=head2,*pold;//两个临时变量 pold表示前一个节点 
	puts(">请输入要删除的读者编号：");
	scanf("%s",number1);
	while(p!=NULL)
	{
		if(strcmp(number1,p->number)==0)//如果找到这本位读者了
		{
			found = 1;//标记为找到了
			printf("读者姓名：%s\n",p->name);
			printf("读者编号：%s\n",p->number);
			puts("");
			puts(">您确定要删除这位读者的信息吗 是（1）or 否（0）");
			int choice;
			scanf("%d",&choice);
			if(choice)
			{
				if(p==head2)//如果要删掉的是第一位读者 
				{
					head2 = head2->next;
					free(p);
					p = head2; 
				}
				else
				{
					pold->next = p->next;
					free(p);
					p = pold->next;
				}
				printf("正在删除请稍等");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".\n");
				printf(">删除成功！\n\n");
				Reader_num--;
				
			}
			else break;
		}
		else//如果还没找到这位读者，就找下一个节点
		{
			pold = p;
			p = p->next;
		}
	}
	if(found==0)	puts("删除失败，未找到此位读者！");
	puts("按任意键继续...");
	fflush(stdin);
	getchar();

}

//修改书本信息 
void update_Book()
{
	system("cls");
	book *p = head1;
	char name[20];
	int x,found=0;
	printf(">请输入要修改的图书的书名：");
	scanf("%s",name);
	while(p!=NULL)
	{
		if(strcmp(name,p->bookName)==0)
		{
			found = 1;
			show_oneBook(p);
			printf(">是否确定修改？ 是（1）or 否（0）");
			scanf("%d",&x);
			if(x)
			{
				system("cls");
				printf("**********请重新输入信息*********\n");
				printf(">请输入编号：");
				scanf("%d",&p->bookNumber);
				printf(">请输入书名：");
				scanf("%s",p->bookName);
				printf(">请输入作者名：");
				scanf("%s",p->author);
				printf(">请输入分类号：");
				scanf("%d",&p->typenum);
				printf(">请输入出版单位：");
				scanf("%s",p->publishCompany);
				printf(">请输入出版时间：");
				scanf("%s",p->publishTime);
				printf(">请输入书本价格：");
				scanf("%f",&p->price);
				printf("正在更新请稍等");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".");
				Sleep(500);
				printf(".\n");
				printf("\n>提醒：修改成功!\n");
			}
		}
		p = p->next;
	}
	if(found==0)	printf(">提醒：没有您要修改的信息！\n");
	puts("按任意键继续...");
	fflush(stdin);
	getchar();
}

//修改读者信息 
void update_reader()
{
    system("cls");
    reader *p = head2;
    char number1[20];
    int x,found=0;
    printf(">请输入要修改的读者编号：");
    scanf("%s",number1);
    while(p!=NULL)
    {
        if(strcmp(number1,p->number)==0)
        {
            found = 1;
            printf("读者姓名：%s\n",p->name);
            printf("读者编号：%s\n",p->number);
            printf(">是否确定修改？ 是（1）or 否（0）");
            scanf("%d",&x);
            if(x)
            {
                system("cls");
                printf("**********请重新输入信息*********\n");
                printf(">请输入读者编号：");
                scanf("%99s",p->number);
                printf(">请输入读者借阅号："); 
                scanf("%d",&p->loanNumber);
                printf(">请输入读者姓名：");
                scanf("%s",p->name);
                printf(">请输入读者最大借阅额度：");
                scanf("%d",&p->maxLoanAmount);
                printf("正在更新请稍等");
                Sleep(500);
                printf(".");
                Sleep(500);
                printf(".");
                Sleep(500);
                printf(".\n");
                printf("\n>提醒：修改成功!\n");
            }
        }
        p = p->next;
    }
    if(found==0)    printf(">提醒：没有您要修改的信息！\n");
    puts("按任意键继续...");
    fflush(stdin);
    getchar();
}

//借阅信息查询 
void search_borrow_list()
{
	system("cls");
	puts(">按书本名称查找请按 1\n>按读者姓名查找请按 2\n>按书本作者查找请按3");
	int x,found=0;
	char search[101];
	scanf("%d",&x);
	system("cls");
	borlist *p=head3;
	switch(x)
	{
		case 1:
			{
				puts(">请输入要查找的书本名称:"); 
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->Bname)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						printf("读者编号：%s\n",p->Rnumber);
						printf("读者姓名：%s\n",p->Rname);
						printf("书本名称：%s\n",p->Bname);
						printf("书本作者：%s\n",p->Bauthor);
						printf("借书日期：%s\n",p->borrowDate);
						printf("还书日期：%s\n",p->returnDate);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 2:
			{
				puts(">请输入要查找的读者姓名:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->Rname)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						printf("读者编号：%s\n",p->Rnumber);
						printf("读者姓名：%s\n",p->Rname);
						printf("书本名称：%s\n",p->Bname);
						printf("书本作者：%s\n",p->Bauthor);
						printf("借书日期：%s\n",p->borrowDate);
						printf("还书日期：%s\n",p->returnDate);
						puts("");
					}
					p = p->next;
				}
				break;
			}
		case 3:
			{
				puts(">请输入要查找的书本作者姓名:");
				scanf("%s",search);
				puts("");
				while(p!=NULL)
				{
					if(strcmp(search,p->Bauthor)==0)
					{
						found=1;
						printf("正在查找");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".");
						Sleep(500);
						printf(".\n");
						printf("读者编号：%s\n",p->Rnumber);
						printf("读者姓名：%s\n",p->Rname);
						printf("书本名称：%s\n",p->Bname);
						printf("书本作者：%s\n",p->Bauthor);
						printf("借书日期：%s\n",p->borrowDate);
						printf("还书日期：%s\n",p->returnDate);
						puts("");
					}
					p = p->next;
				}
				break;
			}

		default:
			{
				printf("输入错误！请重新搜索！\n") ;
				found = 2;
				break;
			}
	}
	if(found==1)	puts("已找到以上读者，按任意键继续...\n");
	if(found==0) 	puts("你要查找的借阅信息不存在!\n");
	if(found==2)	search_borrow_list();
	fflush(stdin);
	getchar();
	
}

//借书功能   前提：保存好读者编号，遍历读者指针，储存;后输入书的名称，通过指针遍历找到该书指针 
void borrow_book(book *p1,reader *p2)
{
	borlist *p = NULL;
	system("cls");
	p = (borlist*)malloc(sizeof(borlist));
	if(p1->quantity==0)
	{
		printf("抱歉！该书已无库存");
	}
	if(p1->quantity>0)
	{
		printf("请问您是否需要借阅：%s\n",p1->bookName);
		printf("是（1）or 否（0）：");
		int choice;
		scanf("%d",&choice);
		if(choice)
		{
			if(p2->borrowNum == p2->maxLoanAmount)
			{
				printf("借书失败，已超出您的最大借阅额度！"); 
			}
			else
			{
				borrowListNumber++;													//借阅表内数量加一 
				p1->quantity--;														//书本库存数量减一 
				p1->lendNumber++;													//书本借阅次数加一 
				p2->borrowNum++;													//读者已借阅数量加一
				p2->Rlend++;														//读者总借阅次数加一 
				strcpy(p->Rnumber,p2->number);		//借阅表写入读者编号
				strcpy(p->Rname,p2->name);			//借阅表写入读者名称
				strcpy(p->Bname,p1->bookName);		//借阅表写入书本名称
				strcpy(p->Bauthor,p1->author);		//借阅表写入书本作者 
				char s[20];
				printf("请输入借书日期：");
				scanf("%s",s);
				strcpy(p->borrowDate,s);			//借阅表写入借书日期
				p->next = head3;
				head3 = p; 
				printf("恭喜您借书成功！请记得按时归还哦！\n");
				system("pause");
			} 
		}
		else{system("pause");}	 
	}
}


//还书功能   前提：保存好读者编号，遍历读者指针，储存;后输入书的名称，通过指针遍历找到该书指针 
void return_book(book *p1,reader *p2)
{
	system("cls");
	borlist *p=head3;
	printf("请问您是否需要归还：%s\n",p1->bookName);
		printf("是（1）or 否（0）：");
		int choice;
		scanf("%d",&choice);
		if(choice)
		{
			while(p!=NULL)
			{
				if(strcmp(p->Bname,p1->bookName)==0&&strcmp(p->Rname,p2->name)==0)
				{
					p1->quantity++;											//书本库存数量加一 
					p2->borrowNum--;										//读者已借阅数量减一
					char s[20];
					printf("请输入还书日期：");
					scanf("%s",s);
					strcpy(p->returnDate,s);								//借阅表写入还书日期
					break;
				}
				p = p->next;
			}
			printf("找不到您要归还的书！");	 
		}
		else system("pause");	
}

//图书借阅统计查询
void total_borrow1()
{
	system("cls"); 
	book *p1=head1;
	borlist *p2;
	int i,j;
	int num[100];
	char name[100][20];
	char date[100][20];
	for(i=0;i<Book_num;i++)
	{
		num[i]=p1->lendNumber;
		strcpy(name[i],p1->bookName);
		for(p2=head3;p2!=NULL;p2=p2->next)
		{
			if(strcmp(p2->Bname,p1->bookName)==0)
			{
				strcpy(date[i],p2->borrowDate);
			}
		}
		p1=p1->next;
	}
	for(i=0;i<Book_num-1;i++)
	{
		for(j=0;j<Book_num-1-i;j++)
		{
			if(num[j] < num[j+1])
			{
				int k=num[j];
				num[j]=num[j+1];
				num[j+1]=k;
				char s[20];
				strcpy(s,name[j]);
				strcpy(name[j],name[j+1]);
				strcpy(name[j+1],s);
				char n[20];
				strcpy(n,date[j]);
				strcpy(date[j],date[j+1]);
				strcpy(date[j+1],n);
			}
		}
	 }
	printf("请问您需要按年份还是月份统计？按1按年统计，按2按月统计:");
	int k;
	scanf("%d",&k);
	if(k==1)
	{
		printf("请输入您要统计借阅量的具体年：");
		char year[5];
		scanf("%s",year);
		int m=0;
		for(i=0;i<Book_num;i++)
		{
			if(year[0]==date[i][0]&&year[1]==date[i][1]&&year[2]==date[i][2]&&year[3]==date[i][3])
			{
				m++;
				printf("第%d名为:%s",m,name[i]);
				printf("\t\t借阅量为：%d",num[i]);
				printf("\n");
			}
		}	
	} 
	else if(k==2)
	{
		printf("请输入您要统计借阅量的具体月：");
		char month[8];
		scanf("%s",month);
		int m=0;
		for(i=0;i<Book_num;i++)
		{
			if(strlen(month)==6)
			{
				if(month[0]==date[i][0]&&month[1]==date[i][1]&&month[2]==date[i][2]&&month[3]==date[i][3]&&month[4]==date[i][4]&&month[5]==date[i][5])
				{
					m++;
					printf("第%d名为:%s",m,name[i]);
					printf("\t\t借阅量为：%d",num[i]);
					printf("\n");
				}
			}
			else
			{
				if(month[0]==date[i][0]&&month[1]==date[i][1]&&month[2]==date[i][2]&&month[3]==date[i][3]&&month[4]==date[i][4]&&month[5]==date[i][5]&&month[6]==date[i][6])
				{
					m++;
					printf("第%d名为:%s",m,name[i]);
					printf("\t\t借阅量为：%d",num[i]);
					printf("\n");
				}
			}
		}	
	}
	system("pause");
}



//读者借阅统计查询
void total_borrow2()
{
	system("cls"); 
	reader *p1=head2;
	borlist *p2;
	int i,j;
	int num[100];
	char name[100][20];
	char date[100][20];
	for(i=0;i<Reader_num;i++)
	{
		num[i]=p1->Rlend;
		strcpy(name[i],p1->name);
		for(p2=head3;p2!=NULL;p2=p2->next)
		{
			if(strcmp(p2->Rname,p1->name)==0)
			{
				strcpy(date[i],p2->borrowDate);
			}
		}
		p1=p1->next;
	}
	for(i=0;i<Reader_num-1;i++)
	{
		for(j=0;j<Reader_num-1-i;j++)
		{
			if(num[j] < num[j+1])
			{
				int k=num[j];
				num[j]=num[j+1];
				num[j+1]=k;
				char s[20];
				strcpy(s,name[j]);
				strcpy(name[j],name[j+1]);
				strcpy(name[j+1],s);
				char n[20];
				strcpy(n,date[j]);
				strcpy(date[j],date[j+1]);
				strcpy(date[j+1],n);
			}
		}
	 }
	printf("请问您需要按年份还是月份统计？按1按年统计，按2按月统计:");
	int k;
	scanf("%d",&k);
	if(k==1)
	{
		printf("请输入您要统计借阅量的具体年：");
		char year[5];
		scanf("%s",year);
		int m=0;
		for(i=0;i<Reader_num;i++)
		{
			if(year[0]==date[i][0]&&year[1]==date[i][1]&&year[2]==date[i][2]&&year[3]==date[i][3])
			{
				m++;
				printf("第%d名为:%s",m,name[i]);
				printf("\t\t借阅量为：%d",num[i]);
				printf("\n");
			}
		}	
	} 
	else if(k==2)
	{
		printf("请输入您要统计借阅量的具体月：");
		char month[8];
		scanf("%s",month);
		int m=0;
		for(i=0;i<Reader_num;i++)
		{
			if(strlen(month)==6)
			{
				if(month[0]==date[i][0]&&month[1]==date[i][1]&&month[2]==date[i][2]&&month[3]==date[i][3]&&month[4]==date[i][4]&&month[5]==date[i][5])
				{
					m++;
					printf("第%d名为:%s",m,name[i]);
					printf("\t\t借阅量为：%d",num[i]);
					printf("\n");
				}
			}
			else
			{
				if(month[0]==date[i][0]&&month[1]==date[i][1]&&month[2]==date[i][2]&&month[3]==date[i][3]&&month[4]==date[i][4]&&month[5]==date[i][5]&&month[6]==date[i][6])
				{
					m++;
					printf("第%d名为:%s",m,name[i]);
					printf("\t\t借阅量为：%d",num[i]);
					printf("\n");
				}
			}
		}	
	}
	system("pause");
}


