#ifndef BORROWING_LIST_H
#define BORROWING_LIST_H
//建立书籍信息
typedef struct book
{
	int bookNumber;				//1、编号 
	char bookName[20];			//2、书名 
	char author[20];			//3、作者名 
	int typenum;				//4、分类号 
	char publishCompany[20];	//5、出版单位 
	char publishTime[20];		//6、出版时间 
	int  quantity;				//7、库存数量 
	float price;				//8、价格
	int lendNumber;				//9、图书借阅次数 
	struct book *next;			//10、书籍指针 
} book;

//建立读者信息
typedef struct reader
{
	char number[20];			//1、编号
	int loanNumber;				//2、借阅号
	char name[20];				//3、姓名
	int maxLoanAmount;			//4、最大借阅额度
	int borrowNum;				//5、已借阅数量
	int Rlend;					//6、总借阅次数 
	struct reader *next;		//6、读者指针 
} reader;

//建立借阅表信息 
typedef struct borlist {
    char Rnumber[20];     				//读者编号 
    char Rname[20];         			//读者姓名 
    char Bname[20];						//书本名称
	char Bauthor[20];					//书本作者	
    char borrowDate[20];       			//借书日期
    char returnDate[20];       			//还书日期
    struct borlist *next;				//借阅表指针 
} borlist;
#endif // BORROWING_LIST_H
