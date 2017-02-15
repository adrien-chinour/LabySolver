AR 		=	ar
CC		=	gcc
CFLAGS	=	-Wall -std=c99
ARFLAGS	=	rcs
EXEC	=	tester_laby

all: tester_laby laby.o liblaby.a

liblaby.a: laby.o
	$(AR) $(ARFLAGS) $@ laby.o

$(EXEC) : tester_laby.c liblaby.a
	$(CC) -g -o $@ $(CFLAGS) $< -L. -llaby

laby.o : laby.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean : 
	rm -f liblaby.a laby.o *~ $(EXEC)