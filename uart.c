#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>      /*文件控制定义*/ 
#include <unistd.h> 
#include "SerDrive.h" 

int uart_send(void)
{
	int SerFd = -1;
	/* 
     * 打开USB转串口（ttyUSB0） 
     * O_RDWR 可读写设备 
     * O_NOCTTY 如果欲打开的文件为终端机设备时，则不会将该终端机当成进程控制终端机 
     * O_NDELAY 以不可阻断的方式打开文件，也就是无论有无数据读取或等待，都会立即返回进程之中 
     */  
    SerFd = open("/dev/ttyS1", O_RDWR|O_NOCTTY|O_NDELAY);  
    if (0 < SerFd)  
    {  
        set_opt(SerFd, BAUD_57600, DATA_BIT_8, PARITY_NONE, STOP_BIT_1);//设置串口参数  
    }  
    else  
    {  
        printf("open_port ERROR !\n"); 
    }
	
	
	ssize_t ret;  
	char data[3] = {88,88,9};
	
    ret = write(SerFd, data, sizeof(data));  
    if (ret == -1)  
    {  
		close(SerFd);
        printf("write device error\n");  
        return -1;  
    } 
	
	close(SerFd);
	
	return 0;
}