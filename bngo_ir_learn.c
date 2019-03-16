#include "bngo_ir_learn.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <ctype.h>


//红外发送脉宽设置
struct pwm_date {
	int pwm_time;
	int pwm_set;
};

//红外学习接受的数据
struct ir_learn {
	unsigned int count;
	unsigned int times[512];
};


void SEND_BYTE(char byte)
{
	int i;
	//低位先发
	for(i = 0;i<8;i++){
		SENG_BIT((byte>>i)&0x01);
	}

}

void SENG_BIT(char bit)
{
	if(bit){
		IR_SEND(560,1);
		IR_SEND(1680,0);
	} else {
		IR_SEND(560,1);
		IR_SEND(560,0);
	}
}

int IR_SEND(uint32 time,int set)
{	
	int fd;
	fd = open("/dev/ir_send",O_RDWR);
	if(fd<0){
		printf("open /dev/ir_send error\n");
		return -1;
	}
	
	struct pwm_date p_date;
	p_date.pwm_time = time;
	p_date.pwm_set = set;	
	write(fd,&p_date,sizeof(p_date));

	lseek(fd,0,SEEK_SET);
	fsync(fd);

	return close(fd);
}

int check_ir_data(struct ir_learn l_date)
{
	int i;
	/*
	 for(i = 0;i<l_date.count;i++){
		 printf("%d ",l_date.times[i]); 
		 
		 //IR_SEND(l_date.times[i],flag);
		 //flag ^= 1;
	 }
	 putchar('\n');
	*/
	
	//1 确认引导码
	//时间判定范围为 8.5～9.5ms，
	if(l_date.times[0] < 8500 || l_date.times[0] > 9500){
		printf("****      【引导码校验失败，请重新尝试】       ****\n");
		return -1;
	}
	
	if(l_date.times[1] < 4000 || l_date.times[1] > 5000){
		printf("****      【引导码校验失败，请重新尝试】       ****\n");
		return -1;
	}
	

	//计数 
	int j=0,m = 0;
	unsigned char byte[4] = {0,0,0,0};
	int sum = 0;
	
	for(i = 2;i<l_date.count;i=i+2){
		sum = l_date.times[i] + l_date.times[i+1];
		//低电平 1120  时间判定范围为 340～780us，	
		if(sum > 680 && sum < 1560){
			byte[j] |= 0x00;
		} else if(sum > 1800 && sum < 2680){
			byte[j] |= 0x80;
		} else {
				if(i == 66)
					break;

				printf("****      【按键码校验失败，请重新尝试】       ****\n");
				return -1;				
			
		}
		m++;

		if(m == 8){
			j++;
			m = 0;
		} else if(m < 8){
			byte[j] >>= 1;	
		}
	}
	//printf("byte[1]:%02x byte[2]:%02x byte[3]:%02x byte[4]:%02x\n",byte[0],byte[1],byte[2],byte[3]);	
	
	if(byte[0] != 0x40 || byte[1] != 0x40 || byte[2] != 0x40 || byte[3] != 0xbf){
		printf("****      【按键码校验失败，请重新尝试】       ****\n");
		return -1;
	} else {
		printf("****            【红外数据校验成功】           ****\n");		
	}

	return 0;
}

/*
 *  红外学习模式
 */ 
int READ_IR_LEARN()
 {
	 struct ir_learn l_date;
	 l_date.count = 0;
	 memset(l_date.times,0,512);
	 int fd;
	 //int flag = 1;
	 fd = open("/dev/ir_learn",O_RDWR);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return 11;
	 }
 
	 read(fd,&l_date,sizeof(l_date));
	 close(fd);	 
	 
	 //printf("count:%d .\n",l_date.count);
	 //没有学习到数据
	 if(l_date.count == 0){
		printf("****   【红外接收超时，请确认硬件是否正常】    ****\n");
		return 13;
	 }
	 
	 
	 check_ir_data(l_date);
	 /*
	 int i;
	 for(i = 0;i<l_date.count;i++){
		 printf("%d ",l_date.times[i]); 
		 
		 //IR_SEND(l_date.times[i],flag);
		 //flag ^= 1;
	 }	 
	  */
	 return 0;
 }

int IR_LEARN_SET(char byte)
{ 
	 int fd;
	 fd = open("/dev/ir_learn",O_WRONLY);
	 if(fd<0){
		 printf("open /dev/ir_learn error\n");
		 return 11;
	 }
 
	 write(fd,&byte,sizeof(byte));	 
	 close(fd);
	 
	 return 0;
}

