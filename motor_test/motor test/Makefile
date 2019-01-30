CROSS_COMPILE=/usr/local/arm/arm-2009q3/bin/arm-none-linux-gnueabi-

CC = $(CROSS_COMPILE)gcc

TARGET=test

all: $(TARGET)

CFLAGS = -static

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c  -o $(TARGET) -lpthread

clean:
	rm -f $(TARGET)
