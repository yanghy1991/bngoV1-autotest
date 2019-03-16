#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>


#include "key.h"

//KEY_1  未定义按键 KEY_2 联网键  KEY_3 触摸按键 
#define  KEY_1  2
#define  KEY_2  3
#define  KEY_3  4


int key_func()  
{  
	int keys_fd;   
	struct input_event t;  
  
	keys_fd = open ("/dev/input/event0", O_RDONLY);  
	if (keys_fd <= 0)  
	{  
	  printf ("open /dev/input/event0 device error!\n");  
	  return 0;  
	} 
	
	//select 监听5s
	fd_set readfd; //读文件描述符集合
	struct timeval timeout;
	timeout.tv_sec = 1;
    timeout.tv_usec = 0;
	
	int i,ret;
	for(i= 0;i<10;i++)
    {  
		timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        //文件描述符集合清0
        FD_ZERO(&readfd);

        //将套接字描述符加入到文件描述符集合
        FD_SET(keys_fd, &readfd);
		//select侦听是否有数据到来
        ret = select(keys_fd + 1, &readfd, NULL, NULL, &timeout); //侦听是否可读
		switch (ret)
        {
			case -1: //发生错误
				perror("select error:");
				break;
			case 0: //超时
				//printf("select timeout\n");
				break;
			default:
				//有按键响应
				if (read (keys_fd, &t, sizeof (t)) == sizeof (t))  
				{  
					  if (t.type == EV_KEY)
					  {
							if (t.value == 1)  
							{  	
								   
								  //printf ("key %d %s\n", t.code,  
										  //(t.value) ? "Pressed" : "Released");  
								  if(t.code == KEY_2){
										printf("\n**                 检测到联网键                  **\n");
										close (keys_fd); 
										return 0;
								  }else if(t.code == KEY_3){
										printf("\n**                 检测到联网键                  **\n");
										close (keys_fd); 
										return 0;
								  }
							} 
					  }
				} 
				break;
		}
		
		//printf("i:%d ####\n",i);
		 
    }  
	
    close (keys_fd);  
  
  return -1;  
} 