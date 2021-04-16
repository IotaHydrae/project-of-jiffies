/**********************************************************
 * 函数名称： main
 * 功能描述： 根据用户输入的命令实现某个功能
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 2020/05/10         V1.0    yanxni            创建

  命令  功能  示例
 	p	 在终端显示当前时间 p
 	y	 查看昨天日期   y
 	n	 查看现在距新年的天数 n
 	a	 根据输入的出生年计算出年纪: a 2001
 	e	 退出程序	  e
  
 ***********************************************************************/
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void displaydate(void);
void displayyesterday(void);
void displaynewyear(void);
void displayage(int);

int main(void)
{
    struct tm *ptr;
    time_t lt;
	char  c;
    int age;
	//打印使用手册
	printf( "\n\n"\
			"Usage:\np\n"\
			"y\n"\
			"n\n"\
			"a 2001\n"\
			"e\n"\
			"p:在终端显示当前时间\ny:查看昨天日期\nn:查看现在距新年的天数\n"\
			"a:根据输入的出生年计算出年纪\ne:退出程序\n"\
     );

    //主程序中循环接收输入的命令，根据不同命令执行不同函数
	while (1){
		if (c !='\n')
		  printf("\n请输入命令：");
		scanf("%c",&c); 
		switch(c){
			case 'p'://在终端显示当前时间
 				displaydate();
				break;
    		case 'y'://显示昨天日期
      			displayyesterday();
				break;
			case 'n'://显示现在距新年的天数
      			displaynewyear();
				break;
			case 'a'://根据输入的出生年计算出年纪
			    scanf("%d",&age); 
      			displayage(age);
				break;
			case 'e'://退出程序
      			exit(0);
				break; 
   			
			default : /* 可选的 */
				break;
    	}
	}
}
/**********************************************************
 * 函数名称： displaydate
 * 功能描述： 在终端打印当前时间信息
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 2020/05/10         V1.0    yanxni            创建
***********************************************************/
void displaydate(){	
	struct tm *ptr;
	time_t lt;

    /*获取日历时间*/
    lt = time(NULL);
    
    /*转化为本地时间*/
    ptr = localtime(&lt);
    
    /*以本地时间的字符串方式打印*/
    printf("%s\n",ctime(&lt));

	/*以本地时间的字符串方式打印*/
    printf("%s\n",asctime(ptr));
 
}
/**********************************************************
 * 函数名称： displayyesterday
 * 功能描述： 在终端打印昨天的日期
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 2020/05/10         V1.0    yanxni            创建
***********************************************************/
void displayyesterday(void){	
	struct tm *ptr;
	time_t lt;

    /*获取日历时间*/
    lt = time(NULL);
	lt -= 24*60*60;
    
    /*转化为本地时间*/
    ptr = localtime(&lt);
    
    /*以本地时间的字符串方式打印*/
    printf("昨天是%d年%d月%d日\n",ptr->tm_year + 1900,ptr->tm_mon + 1,ptr->tm_mday);
}
/**********************************************************
 * 函数名称： displaynewyear
 * 功能描述： 在终端打印距离新年的天数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 2020/05/10         V1.0    yanxni            创建
***********************************************************/
void displaynewyear(void){	
	struct tm *ptr;
	time_t lt,lt2;
	int date;

    /*获取日历时间*/
    lt = time(NULL);
    
    /*转化为本地时间*/
    ptr = localtime(&lt);
	/*构造新年的本地时间*/
	ptr->tm_year += 1;
    ptr->tm_mon = 0;
	ptr->tm_mday =1;
	ptr->tm_hour =0;
	ptr->tm_min =0;
	ptr->tm_sec =0;

	lt2 = mktime(ptr);
	date = (lt2-lt)/(24*60*60);
	
    printf("距离新年还有%d天\n",date);
}

/**********************************************************
 * 函数名称： displayage
 * 功能描述： 在终端打印年纪
 * 输入参数： 出生年代
 * 输出参数： 无
 * 返 回 值： 无
 * 2020/05/10         V1.0    yanxni            创建
***********************************************************/
void displayage(int year){	
	struct tm *ptr;
	time_t lt;
	
    /*获取日历时间*/
    lt = time(NULL);
    
    /*转化为本地时间*/
    ptr = localtime(&lt);

	printf("你的年龄是：%d岁\n",ptr->tm_year +1900 - year  );
}



