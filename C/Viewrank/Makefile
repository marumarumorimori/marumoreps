CC = gcc
PROGRAM = viewrank
LIBS = viewrank.h
OBJS = viewrank_main.o viewrank_thread.o
CFLAGS = -Wall -pthread -g

all: $(PROGRAM).o $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS) -lm

$(PROGRAM).o $(OBJS): $(LIBS)

clean:
	rm -f *.o
