#ifndef SERDRIVE_H  
#define SERDRIVE_H  
//=======================  
//��������  
#define     TTY0_NAME   "/dev/ttyGS0"  
#define     TTY1_NAME   "/dev/ttyGS1"  
#define     TTY2_NAME   "/dev/ttyGS2"  
  
  
//����  
#define     TTYS0   1  
#define     TTYS1   2  
#define     TTYS2   3  
//������  
#define     BAUD_2400       2400  
#define     BAUD_4800       4800  
#define     BAUD_9600       9600 
#define     BAUD_38400      38400
#define     BAUD_57600      57600
#define     BAUD_115200     115200  
#define     BAUD_460800     460800  
//��żУ��λ  
#define     PARITY_ODD    'O' //����  
#define     PARITY_EVEN   'E' //ż��  
#define     PARITY_NONE   'N' //����żУ��λ  
//ֹͣλ  
#define     STOP_BIT_1     1  
#define     STOP_BIT_2     2  
//����λ  
#define     DATA_BIT_7     7  
#define     DATA_BIT_8     8  
//========================================  
//����API  
  
/*�򿪴��ں���*/  
int open_port(int fd,int comport);  
//�������õĺ���  
int set_opt(int fd,int nSpeed, int nBits,  
            char nEvent, int nStop);  
//�Ӵ����ж�ȡ����  
int read_datas_tty(int fd,char *rcv_buf,int TimeOut,int Len);  
//�򴮿ڴ�����  
int send_data_tty(int fd, char *send_buf,int Len);  
  
  
//==============  
#endif // SERDRIVE_H 