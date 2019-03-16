#include "bngo_mac.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>

//get wifi_mac
void get_mac(char *Str)
{
	char *dest = Str;
	while(*Str != '\0')
	{
		if(*Str != ':' && *Str != 0x0a)
		{
			*dest++ = *Str;
		}
		
		++Str;
	}
	*dest = '\0';	
}

int get_bngo_mac(char *bngoMac)
{
	int w_fd;
	char b_buf[25] = {0};
	
	w_fd = open("/sys/class/net/wlan0/address",O_RDONLY);

	if(w_fd < 0){
		printf("open mac file error!!\n");
		return -1;
	}
	
	read(w_fd,b_buf,25);
	get_mac(b_buf);
	close(w_fd);
	
	int i;
	for(i = 0; i < sizeof(b_buf); i++)
        bngoMac[i] = toupper(b_buf[i]);
	
	///////	
	return 0;
}
