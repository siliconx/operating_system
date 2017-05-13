// @siliconx
// 2017-05-10 10:03:17
void init();       //初始化
void p(int s);     //P操作
void v(int s);     //V操作
void block(int s);//阻塞函数
void wakeup(int s);//唤醒函数
void control();   //处理机调度
void processor();//处理机执行
void print();  //打印函数


void init(){ //初始化
	s1=BUF;
	s2=0;
	p1=(link)malloc(sizeof(Pcb));//建立新的结点,并初始化为生产者
	strcpy(p1->name,"Producer");
	strcpy(p1->state,"Ready");
	strcpy(p1->reason,"Null");
	p1->breakp=0;
	p1->next=NULL;

	c1=(link)malloc(sizeof(Pcb));//建立新的结点,并初始化为消费者
	strcpy(c1->name,"Consumer");
	strcpy(c1->state,"Ready");
	strcpy(c1->reason,"Null");
	c1->breakp=0;
	c1->next=NULL;

	ready=p1;
	ready->next=c1;//初始化为生产进程在前，消费进程在后
	c1->next=NULL;
	b_s1=NULL;
	b_s2=NULL;//阻塞进程为NULL
	pc=0;
	con_cnt=0; //消费计数器
}

void p(int s){
	if(s==1){ //p(s1)
		s1--;
		if(s1<0) 
			block(1); //阻塞当前生产进程
		else{
			printf("\t* s1信号申请成功!\n");
			ready->breakp=pc; //保存断点
		}
	}
	else{ //p(s2)
		s2--;
		if(s2<0) 
			block(2);//阻塞当前消费进程
		else{
			printf("\t* s2信号申请成功!\n");
			ready->breakp=pc; //保存断点
		}
	}
}

void v(int s){
	if(s==1){ //v(s1)
		s1++;
		if(s1<=0)
			wakeup(1); //唤醒生产进程
		ready->breakp=pc; //保存断点
	}
	else{ //v(s2)
		s2++;
		if(s2<=0) 
			wakeup(2);//唤醒消费进程
		ready->breakp=pc; //保存断点
	}
}

void block(int s){//阻塞函数的定义
	link p;
	int num1=0;
	int num2=0;
	if(s==1){//生产进程
		strcpy(p1->state,"Block");//改变状态
		strcpy(p1->reason,"S1");//说明原因
		p=b_s1;
		while(p){
			num1++;
			p=p->next;//p的值为NULL,表示队尾
		}
		if(!b_s1)
			b_s1=p1;
		else
			p=p1;
		p1->next=NULL;
		printf("\t* p1生产进程阻塞了!\n");
		ready->breakp=pc; //保存断点
		ready=ready->next;//在就绪队列中去掉,指向下一个	
		num1++;
	}
	else{//消费进程
		strcpy(c1->state,"Block");
		strcpy(c1->reason,"S2");
		p=b_s2;
		while(p){
			num2++;
			p=p->next;//p的值为NULL,表示队尾
		}	
		if(!b_s2)
			b_s2=c1;
		else
			p=c1;	
		ready->breakp=pc; //保存断点
		ready=ready->next;//在就绪队列中去掉,指向下一个		
		c1->next=NULL;
		printf("\t* c1消费进程阻塞了!\n");
		num2++;
	}	
	printf("\t* 阻塞的生产进程个数为:%d\n",num1);		
	printf("\t* 阻塞的消费进程个数为:%d\n",num2);
}

void wakeup(int s){//唤醒函数的定义
	link p;
	link q=ready;
	if(s==1){ //唤醒b_s1队首进程,生产进程队列
		p=b_s1;
		b_s1=b_s1->next;//阻塞指针指向下一个阻塞进程
		strcpy(p->state,"Ready");
		strcpy(p->reason,"Null");
		while(q)//插入就绪队列
			q=q->next;
		q=p;
		p->next=NULL;
		printf("\t* p1生产进程唤醒了!\n");
	}
	else{ //唤醒b_s2队首进程,消费进程队列
		p=b_s2;
		b_s2=b_s2->next;//阻塞指针指向下一个阻塞进程
		strcpy(p->state,"Ready");
		strcpy(p->reason,"Null");
		while(q->next)//插入就绪队列
			q=q->next;
		q->next=p;
		p->next=NULL;
		printf("\t* c1消费进程唤醒了!\n");
	}
}

void control() //处理器调度程序
{
	int rd;
	int num=0;

	link p=ready;
    if(ready==NULL) //若无就绪进程,结束
		return;

	while(p) //统计就绪进程个数
	{
		num++;
		p=p->next;//最终p变为NULL
	}	
	printf("\t* 就绪进程个数为:%d\n",num);
	
	time_t t; 
	srand((unsigned) time(&t));
	rd=rand()%num;//随机函数产生随机数
	if(rd==1){
		p=ready;
		ready=ready->next;
		ready->next=p;
		p->next=NULL;
		strcpy(ready->state,"Run");
		strcpy(ready->next->state,"Ready");
	}
	else 
		strcpy(ready->state,"Run");
	pc=ready->breakp;
}

void processor(){ //模拟处理器指令执行
	if(strcmp(ready->name,"Producer")==0) //当前进程为生产者
		switch(pc)
		{
        case 0://produce
			printf("\t* 生产者生产了字符%c\n",str[sp]);
			rec_p[rp1]=str[sp];//添加到生产记录
		    sp=(sp+1)%len;
			pc++;
			ready->breakp=pc; //保存断点
			break;
		case 1: //p(s1)
			pc++;
			p(1);
			break;
		case 2: //put
			buffer[in]=rec_p[rp1]; //放到缓冲区
			printf("\t* %c字符成功入驻空缓存!\n",buffer[in]);
			rp1++; 	
			in=(in+1)%BUF;
			pc++;
			ready->breakp=pc; //保存断点
			break;
		case 3: //v(s2)
			pc++;
			printf("\t* 释放一个s2信号\n");
			v(2);
			break;
		case 4://goto01  
			printf("\t* 生产进程goto 0 操作\n");
			pc=0;
			count--; //剩余字符个数减1
			printf("\t* 剩余字符count=%d个\n",count);
			ready->breakp=pc; //保存断点

			if(count<=0){ //生产结束
				printf("\t* 生产者结束生产!\n");
				strcpy(p1->state,"Stop");
				strcpy(p1->reason,"Null");
				ready->breakp=-1;
				ready=ready->next;//在就绪队列中去掉
			}
		} 
	else  //当前进程为消费者
		switch(pc)
		{
		case 0: //p(s2)
			pc++;
			p(2); 
			break;
		case 1: //get
			printf("\t* 消费者取字符!\n");
			temp=buffer[out];
			out=(out+1)%BUF;
			pc++;
			ready->breakp=pc; //保存断点
			break;
		case 2: //v(s1)
			pc++;
			printf("\t* 释放一个s1\n");
			v(1);
			break;
		case 3: //consume
			printf("\t* 消费了字符%c\n",temp);
			rec_c[rp2]=temp;//添加到消费记录
			rp2++;
			con_cnt++;
			if(con_cnt>=len){
				strcpy(c1->state,"Stop");//完成态
				c1->breakp=-1;
				return;
			}
			pc++;
			ready->breakp=pc; //保存断点
			break;
		case 4: //goto0
			printf("\t* 消费进程goto 0 操作\n");
			pc=0;
			ready->breakp=pc; //保存断点
		}
}

void print(){
	int i,j;
	printf("————————生产者消费者模拟———————\n");
	printf("* 模拟过程的字符串为:\t");
	printf("%s\n",&str);
	
	printf("* 已生产:");
	for(j=0;j<=rp1;j++)
		printf("%c",rec_p[j]);
	printf("\n* 空缓存:");
	for(j=rp2;j<=rp1;j++)
		printf("%c",buffer[j]);
	printf("\n* 已消费:");
	for(j=0;j<=rp2;j++)
		printf("%c",rec_c[j]);
	printf("\n———————进程控制块的信息————————\n");
	printf("进程名\t\t状态\t等待原因\t断点\n");
	printf("%s\t%s\t%s\t\t%d\n\n",p1->name,p1->state,p1->reason,p1->breakp);
	printf("%s\t%s\t%s\t\t%d\n",c1->name,c1->state,c1->reason,c1->breakp);
	printf("———————————————————————\n");
	printf("1.继续 0.退出\n");
	scanf("%d",&i);
	if(i==0){
		exit(0);
	}
}