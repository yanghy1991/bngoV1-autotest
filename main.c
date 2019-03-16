#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
 
#include "bngo_led.h"
#include "uart.h"
#include "key.h"
#include "bngo_ir_learn.h"
#include "bngo_mac.h"

int pthread_flag = 1;

static void *step1_func(void *arg)
{
	uart_send();
	while(pthread_flag){
		
		//联网成功
		bngo_led_close("PA7");
		//联网失败
		bngo_led_open("PA9");
		//禁麦
		bngo_led_open("PA10");
		usleep(500000);
		
		bngo_led_close("PA7");
		bngo_led_close("PA9");
		bngo_led_close("PA10");
		usleep(500000);
		
		//联网成功
		bngo_led_open("PA7");
		//联网失败
		bngo_led_close("PA9");
		//禁麦
		bngo_led_open("PA10");
		usleep(500000);
		
		bngo_led_close("PA7");
		bngo_led_close("PA9");
		bngo_led_close("PA10");
		usleep(500000);
	}
	
	//printf("###step1_pthread exit.###\n");
	
	return NULL;
}

static void *step3_func(void *arg)
{
	int ret = 0;
	ret = key_func();
	if(ret != 0)
		printf("**      未检测到按键输入，请确认硬件是否正常     **\n");
	return NULL;
}

static void *step4_func(void *arg)
{
	/*	
	count:67 .
	9058 4516 565 601 538 628 602 606 598 574 567 601 570 600 565 1695 589 626 566 600 543 625 574 4573 594 1019 600 749 596 643 593 1781 599 603 569 619 622 627 596 647 596 636 602 691 596 613 596 1788 598 594 570 1720 579 1715 593 1698 567 1728 564 1719 564 1716 567 601 567 1728 585
	
	*/
	////////////TV//////////////////
	usleep(500000);
	
	IR_SEND(9000,1);
	IR_SEND(4500,0);
	SEND_BYTE(0x40);
	SEND_BYTE(0x40);
	SEND_BYTE(0x40);
	SEND_BYTE(0xbf);
	IR_SEND(560,1);
	
	printf("\n\n");
	printf("***************************************************\n");
	printf("**                已发送红外数据                 **\n");
	printf("***************************************************\n");
	return NULL;
}


struct msg_st
{
	long int msg_type;
	int flag;
	char text[4096];
};

//消息队列，调节时钟和底灯的亮度、电器控制
int msq_send(int msg_key,int flag,char *buf)
{
	struct msg_st MessagesUart;
	int msgid = -1;
	//建立消息队列
	msgid = msgget((key_t)msg_key, 0666 | IPC_EXCL);
	if(msgid == -1)
	{
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		return -1;
	}

	MessagesUart.msg_type = 1;
	MessagesUart.flag = flag;
	strcpy(MessagesUart.text,buf);

	//向队列发送数据
	if(msgsnd(msgid,&MessagesUart,sizeof(struct msg_st),0) == -1)
	{
		fprintf(stderr, "msgsnd failed\n");
		return -2;
	}

	return 0;
}

int main(int argc,char *argv[])
{
	struct sched_param param; 
	int maxpri; 
	maxpri = sched_get_priority_max(SCHED_FIFO);
	if(maxpri == -1) 
	{ 
		perror("sched_get_priority_max() failed"); 
		return -1;
	} 
		param.sched_priority = maxpri; 
	if (sched_setscheduler(getpid(), SCHED_FIFO, &param) == -1) //设置优先级
	{ 
		perror("sched_setscheduler() failed"); 
		return -1;
	} 
	
	char bngoMac[50] = {0};
	if(get_bngo_mac(bngoMac) != 0){
		strcpy(bngoMac,"获取设备MAC失败，请重新打开。");
	}
	
	
	//杀死 uart 进程  为了正常显示8888
	//ps| grep "uart" |grep -v grep| cut -c 9-15 | xargs kill -9
	system("ps| grep \"uart\" |grep -v grep| cut -c 1-6 | xargs kill -9");
	printf("***************************************************\n");
	printf("****          【开始运行自动测试程序】         ****\n");
	printf("****           DXMF  version: V1.0.0           ****\n");
	printf("****                                           ****\n");
	printf("****          设备MAC：%s            ****\n",bngoMac);
	
	printf("***************************************************\n");
	printf("\n\n");
	
	printf("***************************************************\n");
	printf("****           一、测试指示灯和数码管          ****\n");
	printf("**   已点亮指示灯、数码管，请确认硬件是否正常    **\n");
	printf("**     继续请按【Enter】\\退出请按【CTRL + C】    **\n");
	printf("***************************************************\n");

	printf("\n\n");
	
	//创建线程跑指示灯数码管
	pthread_t step1_pthread;
	if (pthread_create(&step1_pthread, NULL, step1_func, NULL) == -1)
		printf("Create step1_pthread failed: %s.\n", strerror(errno));
	pthread_detach(step1_pthread);
	
	char buf[100] = {0};
	while(NULL != fgets(buf, 100, stdin)){
		if(strstr(buf,"\n") != NULL)
			break;
		
		printf("****       输入有误，请按【Enter】继续         ****\n");
	}
	pthread_flag = 0;
	
	printf("***************************************************\n");
	printf("****         二、测试触摸按键、夜灯            ****\n");
	printf("**    请按触摸按键，确认四个LED灯是否都点亮      **\n");
	printf("**     继续请按【Enter】\\退出请按【CTRL + C】    **\n");
	printf("***************************************************\n");
	printf("\n\n");
	memset(buf,0,sizeof(buf));
	while(NULL != fgets(buf, 100, stdin)){
		if(strstr(buf,"\n") != NULL)
			break;
		
		printf("****       输入有误，请按【Enter】继续         ****\n");
	}
	
	printf("***************************************************\n");
	printf("****            三、测试联网按键               ****\n");
	printf("**             请在 10S 内单击联网键             **\n");
	printf("**     继续请按【Enter】\\退出请按【CTRL + C】    **\n");
	printf("***************************************************\n");
	printf("\n\n");
	//创建按键检测线程
	pthread_t step3_pthread;
	if (pthread_create(&step3_pthread, NULL, step3_func, NULL) == -1)
		printf("Create step3_pthread failed: %s.\n", strerror(errno));
	pthread_detach(step3_pthread);
	
	memset(buf,0,sizeof(buf));
	while(NULL != fgets(buf, 100, stdin)){
		if(strstr(buf,"\n") != NULL)
			break;
		
		printf("****       输入有误，请按【Enter】继续         ****\n");
	}
	
	
	printf("***************************************************\n");
	printf("****            四、测试红外收发               ****\n");
	printf("**     继续请按【Enter】\\退出请按【CTRL + C】    **\n");
	printf("***************************************************\n");
	printf("\n\n");
	
	//进入红外学习模式
	IR_LEARN_SET(1);
	
	//创建发送 红外码库线程
	pthread_t step4_pthread;
	if (pthread_create(&step4_pthread, NULL, step4_func, NULL) == -1)
		printf("Create step4_pthread failed: %s.\n", strerror(errno));
	pthread_detach(step4_pthread);
	
	//等待接受红外码
	READ_IR_LEARN();
	
	
	memset(buf,0,sizeof(buf));
	while(NULL != fgets(buf, 100, stdin)){
		if(strstr(buf,"\n") != NULL)
			break;
		
		printf("****       输入有误，请按【Enter】继续         ****\n");
	}
	
	
	printf("***************************************************\n");
	printf("****            五、测试语音唤醒               ****\n");
	printf("**     继续请按【Enter】\\退出请按【CTRL + C】    **\n");
	printf("***************************************************\n");
	printf("\n\n");
	
	//消息队列，发送到mozart vr.c用于手动唤醒mozart，然后播放音频，判断语音转文字是否正确； 
	msq_send(4781,10,"autotest");
	usleep(500000);
	system("mplayer -af volume=0 /usr/fs/usr/share/vr/tone/nihaoxiaole.mp3");
	printf("***************************************************\n");
	printf("****    已播放音频，请确认是否有语音识别数据   ****\n");
	printf("***************************************************\n");
	printf("\n\n");
	

	memset(buf,0,sizeof(buf));
	while(NULL != fgets(buf, 100, stdin)){
		if(strstr(buf,"\n") != NULL)
			break;
		
		printf("****       输入有误，请按【Enter】继续         ****\n");
	}
		
	
	
	printf("***************************************************\n");
	printf("****            【自动测试已结束】             ****\n");
	printf("***************************************************\n");
	printf("\n\n");
	return 0;
}