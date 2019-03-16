#ifndef __BNGO_LED_H_
#define __BNGO_LED_H_

int bngo_led_open(char *pin);
int bngo_led_close(char *pin);
//านตฦ
int bngo_led_write(unsigned int led_level);

#endif //__BNGO_LED_H_