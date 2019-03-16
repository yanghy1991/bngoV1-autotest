CFLAGS= -Wall -O2
CC=mipsel-linux-gcc

COMMAND=-L/yuan/pro/autotest/lib  -lpthread
obj +=main.o bngo_led.o uart.o SerDrive.o key.o bngo_ir_learn.o bngo_mac.o
TARGET = autotest
main: $(obj)
	$(CC) $(CFLAGS) -o $(TARGET) $(obj) $(COMMAND)

clean:
	rm -rf *.o autotest
