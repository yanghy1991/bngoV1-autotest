#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define SYSFS_GPIO_EXPORT        "/sys/class/gpio/export"
#define SYSFS_GPIO_DIR_OUT       "out"
#define SYSFS_GPIO_PIN_VAL_H     "1"
#define SYSFS_GPIO_PIN_VAL_L     "0"


//?????? LED1 :PA9  红 wifi异常
#define SYSFS_GPIO_PIN_PA9 			"9"
#define SYSFS_GPIO_PIN_PA9_DIR      "/sys/class/gpio/gpio9/direction"
#define SYSFS_GPIO_PIN_PA9_VAL      "/sys/class/gpio/gpio9/value"

//?????? LED2 :PA7 绿 wifi正常工作
#define SYSFS_GPIO_PIN_PA7 			"7"
#define SYSFS_GPIO_PIN_PA7_DIR      "/sys/class/gpio/gpio7/direction"
#define SYSFS_GPIO_PIN_PA7_VAL      "/sys/class/gpio/gpio7/value"

//?????: LED3 :PA10  红 禁麦指示灯
#define SYSFS_GPIO_PIN_PA10 		"10"
#define SYSFS_GPIO_PIN_PA10_DIR      "/sys/class/gpio/gpio10/direction"
#define SYSFS_GPIO_PIN_PA10_VAL      "/sys/class/gpio/gpio10/value"

int echo_led_export(char *pin,char *pinDir)
{
 	if(access(pinDir,F_OK) == 0)
		return 0;
	
	/* O_WRONLY O_RDONLY  O_RDWR
	 * echo 91 > export
	 */
	int fd;
	fd = open(SYSFS_GPIO_EXPORT, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:open export error\n");
		return -1;	
	}

	write(fd, pin,sizeof(pin));
	close(fd);

	/*
	 *  /sys/class/gpio/gpio91# echo out > direction
	 */
	fd = open(pinDir, O_WRONLY);
	if(fd < 0){
		printf("echo_led_export:busy pin direction open error\n");
		return -2;	
	}	

	write(fd, SYSFS_GPIO_DIR_OUT, sizeof(SYSFS_GPIO_DIR_OUT));
	close(fd);
	
    return 0;
}

int bngo_led_open(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA9") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA9,SYSFS_GPIO_PIN_PA9_DIR);
		fd = open(SYSFS_GPIO_PIN_PA9_VAL, O_WRONLY);
	} else {
		   return -1;
	}

	if(fd < 0) {
		printf("bngo_led_open: open export error.\n");
		return -1;
	}
	
	write(fd, SYSFS_GPIO_PIN_VAL_L,sizeof(SYSFS_GPIO_PIN_VAL_L));
	fsync(fd);
	close(fd);	
	return 0;
}

int bngo_led_close(char *pin)
{
	int fd;
	if(strcmp(pin,"PA10") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA10,SYSFS_GPIO_PIN_PA10_DIR);
		fd = open(SYSFS_GPIO_PIN_PA10_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA7") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA7,SYSFS_GPIO_PIN_PA7_DIR);
		fd = open(SYSFS_GPIO_PIN_PA7_VAL, O_WRONLY);
	} else if(strcmp(pin,"PA9") == 0){
		echo_led_export(SYSFS_GPIO_PIN_PA9,SYSFS_GPIO_PIN_PA9_DIR);
		fd = open(SYSFS_GPIO_PIN_PA9_VAL, O_WRONLY);
	} else {
		   return -1;
	}

	if(fd < 0) {
		printf("bngo_led_close: open export error.\n");
		return -1;
	}
	write(fd, SYSFS_GPIO_PIN_VAL_H,sizeof(SYSFS_GPIO_PIN_VAL_H));
	fsync(fd);
	close(fd);

	return 0;
}

//夜灯
int bngo_led_write(unsigned int led_level)
{
	int fd;
	//printf("led_level:%d   ...\n",led_level);
	fd = open("/dev/led_bngo",O_RDWR);
	if(fd<0){
		printf("open /dev/led_bngo error\n");
		return -1;
	}
	write(fd,&led_level,sizeof(led_level));
	close(fd);

	return 0;
}


