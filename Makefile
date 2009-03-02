#CFLAGS = -g -Wall -pendantic
CFLAGS = -O2 -pipe
CC = g++-4.2

all : index

index : randBit.o skipList.o Driver.cc Driver.h
	$(CC) $(CFLAGS) Driver.cc randBit.o skipList.o -o index

randBit.o : randBit.cc randBit.h
	$(CC) $(CFLAGS) -c randBit.cc

skipList.o : skipList.cc skipList.h
	$(CC) $(CFLAGS) -c skipList.cc

clean :
	rm -rf index *.o *.dSYM
