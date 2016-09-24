CFLAGS	= -g -Wall -Wextra -std=c99

CC	= gcc
OUT	= polyTest
OBJS	= map.o poly.o polyTest.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)
clean:
	rm -f $(OUT) $(OBJS)
