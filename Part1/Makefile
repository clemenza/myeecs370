CC = gcc
CFLAGS = -g
OBJS = *.c

all:	a.out

a.out:	
	$(CC) $(CFLAGS) -o $@ $(OBJS)

exe:
	./a.out sample.as sample.mc
clean:	
	-rm -rf a.out *.o *.mc 
