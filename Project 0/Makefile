CC      = gcc
CFLAGS  = -O -g
LDFLAGS  = -O 


all: simple 

simple:  simple_shell.o
	$(CC) -o $@ $^ $(LDFLAGS)

run: 
	./simple

test: 
	./simple < testfile

clean:
	rm -f simple

leak:
	valgrind --leak-check=yes echo "ls | sort" | ./simple

three:
	./simple < test3 


.c.o:
	$(CC)  $(CFLAGS) -c $<

