// @siliconx
// 2017-05-10 10:02:25

#include "a.h"
#include "b.h"
#include "c.h"

void main(){
	printf("*生产者消费者模拟\n");
	printf("—————————\n");
	printf("*请输入字符串:\n");
	scanf("%s",str);    //string数组存放将要产生的字符
	len=strlen(str);
	count=len;     //输入字符的个数
	init();        //初始化
	while(con_cnt<len) //消费完所有的字符为结束
	{	
		system("cls");  //清屏操作	
		printf("—————————模拟指令流程————————\n");
		control();      //处理器调度程序
		processor();    //模拟处理器指令执行
		print();        //输出显示各个信息
	}
	printf("\n程序结束!\n");
}