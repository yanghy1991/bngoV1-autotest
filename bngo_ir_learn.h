#ifndef __BNGO_IR_LEARN_H_
#define __BNGO_IR_LEARN_H_

typedef unsigned char uint8;
typedef unsigned int uint32;

void SEND_BYTE(char byte);
void SENG_BIT(char bit);
//���ⷢ�ͽӿ�
int IR_SEND(uint32 time,int set);
//��ȡѧϰ���ݣ�Ȼ����
int READ_IR_LEARN();
//�������ѧϰģʽ
int IR_LEARN_SET(char byte);

#endif //__BNGO_IR_LEARN_H_
