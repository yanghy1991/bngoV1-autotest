#include "SerDrive.h"  
//===========================================  
  
#include     <stdio.h>      /*��׼�����������*/  
#include     <stdlib.h>     /*��׼�����ⶨ��*/  
#include     <unistd.h>     /*Unix ��׼��������*/  
#include     <sys/types.h>  
#include     <sys/stat.h>  
#include     <fcntl.h>      /*�ļ����ƶ���*/  
#include     <termios.h>    /*POSIX �ն˿��ƶ���*/  
#include     <errno.h>      /*����Ŷ���*/  
#include	 <string.h>
  
//=============================================  
  
//�������õĺ���  
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop)  
{  
    struct termios newtio, oldtio;  
    /*����������д��ڲ������ã�������������ںŵȳ���������صĳ�����Ϣ*/  
    if (tcgetattr(fd, &oldtio) != 0)  
    {  
        perror("SetupSerial 1");  
        return -1;  
    }  
    bzero(&newtio, sizeof(newtio));  
    /*����һ�������ַ���С*/  
    newtio.c_cflag |= CLOCAL;   //������ã�modem �Ŀ����߽��ᱻ���ԡ����û�����ã��� open()����������ֱ���ز���������� modem ����ժ��״̬Ϊֹ��  
    newtio.c_cflag |= CREAD;    //ʹ�˿��ܶ�ȡ���������  
    /*����ÿ�����ݵ�λ��*/  
    switch (nBits)  
    {  
    case 7:  
        newtio.c_cflag |= CS7;  
        break;  
    case 8:  
        newtio.c_cflag |= CS8;  
        break;  
    }  
    /*������żУ��λ*/  
    switch (nEvent)  
    {  
    case 'O': //����  
        newtio.c_iflag |= (INPCK | ISTRIP);  
        newtio.c_cflag |= PARENB;   //ʹ��У�飬�������PARODD����żУ��  
        newtio.c_cflag |= PARODD;   //��У��  
        break;  
    case 'E': //ż��  
        newtio.c_iflag |= (INPCK | ISTRIP);  
        newtio.c_cflag |= PARENB;  
        newtio.c_cflag &= ~PARODD;  
        break;  
    case 'N':  //����żУ��λ  
        newtio.c_cflag &= ~PARENB;  
        break;  
    }  
    /*���ò�����*/  
    switch (nSpeed)  
    {  
    case 2400:  
        cfsetispeed(&newtio, B2400);  
        cfsetospeed(&newtio, B2400);  
        break;  
    case 4800:  
        cfsetispeed(&newtio, B4800);  
        cfsetospeed(&newtio, B4800);  
        break;  
    case 9600:  
        cfsetispeed(&newtio, B9600);  
        cfsetospeed(&newtio, B9600);  
        break;  
    case 57600:  
        cfsetispeed(&newtio, B57600);  
        cfsetospeed(&newtio, B57600);  
        break;
    case 115200:  
        cfsetispeed(&newtio, B115200);  
        cfsetospeed(&newtio, B115200);  
        break;  
    case 460800:  
        cfsetispeed(&newtio, B460800);  
        cfsetospeed(&newtio, B460800);  
        break;  
    default:  
        cfsetispeed(&newtio, B9600);  
        cfsetospeed(&newtio, B9600);  
        break;  
    }  
    /* 
     * ����ֹͣλ 
     * ����ֹͣλ��λ���� ������ã������ÿ֡���������ֹͣλ�� ���û�����ã������һ�� 
     * ֹͣλ��һ�㶼��ʹ��һλֹͣλ����Ҫ��λֹͣλ���豸�ѹ�ʱ�ˡ� 
     * */  
    if (nStop == 1)  
        newtio.c_cflag &= ~CSTOPB;  
    else if (nStop == 2)  
        newtio.c_cflag |= CSTOPB;  
    /*���õȴ�ʱ�����С�����ַ�*/  
    newtio.c_cc[VTIME] = 0;  
    newtio.c_cc[VMIN] = 0;  
    /*����δ�����ַ�*/  
    tcflush(fd, TCIFLUSH);  
    /*����������*/  
    if ((tcsetattr(fd, TCSANOW, &newtio)) != 0)  
    {  
        perror("com set error");  
        return -1;  
    }  
    return 0;  
}  
//======================================================  
//�Ӵ����ж�ȡ����  
int read_datas_tty(int fd, char *rcv_buf, int TimeOut, int Len)  
{  
    int retval;  
    fd_set rfds;  
    struct timeval tv;  
    int ret, pos;  
    tv.tv_sec = TimeOut / 1000;  //set the rcv wait time  
    tv.tv_usec = TimeOut % 1000 * 1000;  //100000us = 0.1s  
  
    pos = 0;  
    while (1)  
    {  
        FD_ZERO(&rfds); //ÿ��ѭ����Ҫ��ռ��ϣ������ܼ���������仯  
        FD_SET(fd, &rfds);  //���������  
        retval = select(fd + 1, &rfds, NULL, NULL, &tv);    //fd+1:���������ֵ��1 ���ظ���������  0:timeout  >0:���ļ��ɶ�д  
        if (retval == -1)  
        {  
            perror("select()");  
            break;  
        }  
        else if (retval)  
        {  
            ret = read(fd, rcv_buf + pos, 1);  
            if (-1 == ret)  
            {  
                break;  
            }  
  
            pos++;  
            if (Len <= pos)  
            {  
                break;  
            }  
        }  
        else  
        {  
            break;  
        }  
    }  
  
    return pos;  
}  
  
//�򴮿ڴ�����  
int send_data_tty(int fd, char *send_buf, int Len)  
{  
    ssize_t ret;  
  
    ret = write(fd, send_buf, Len);  
    if (ret == -1)  
    {  
        printf("write device error\n");  
        return -1;  
    }  
  
    return 1;  
}  