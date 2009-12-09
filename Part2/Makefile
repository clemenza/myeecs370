CC = gcc
CFLAGS = -g
OBJS = *.c

all:	a.out

a.out:	
	$(CC) $(CFLAGS) -o $@ $(OBJS)

exe:
	./a.out sample.mc > output
clean:	
	-rm -rf a.out *.o output 
