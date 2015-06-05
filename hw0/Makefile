Obj=wc.o
CFLAGS=-g -Wall
CC=gcc
main : $(Obj)
	$(CC) $(CFLAGS) -o main $(Obj)
$(Obj): wc.c
	$(CC) $(CFLAGS) -c wc.c 

.PHONY: clean
clean:
	rm -f $(Obj)
