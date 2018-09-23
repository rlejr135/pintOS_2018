CC = gcc
OBJS = bitmap.o list.o hash.o main.o
TARGET = testlib
all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)


bitmap.o : bitmap.c bitmap.h debug.h round.h limits.h
list.o : list.c debug.h list.h
hash.o : hash.c debug.h hash.h list.h
