CC=gcc

src=*.c

objact=*.o

target=game

LDFLAGS= -lpthread

all: 
	$(CC) -o $(target) $(src)  $(LDFLAGS)

clean:
	rm -rf *.o

