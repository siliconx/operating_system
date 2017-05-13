// @siliconx
// 2017-05-10 10:03:32

//数据结构的定义和全局变量
typedef struct Pcb{ 
	char name[10];      //进程名
	char state[10];    //运行状态
	char reason[10];    //若阻塞，其原因
	int breakp;         //断点保护
	struct Pcb *next;   //阻塞时的顺序
}Pcb,*link;

int s1,s2; //信号量
link p1;//生产者进程
link c1;//消费者进程
char str[MAX]; //输入的字符串
char buffer[BUF]; //缓冲池
int len; //输入长度
int sp=0; //string的指针
int in=0; //生产者指针
int out=0; //消费者指针
char temp; //供打印的临时产品
char rec_p[MAX];//生产记录
int rp1=0;//生产记录指针
char rec_c[MAX];//消费记录
int rp2=0;//消费记录指针
link ready; //就绪队列
link b_s1; //s1阻塞队列
link b_s2; //s2阻塞队列
int pc; //程序计数器
int count; //字符计数器
int con_cnt; //消费计数器