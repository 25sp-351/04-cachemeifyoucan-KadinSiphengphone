all: main_lru main_lfu least_recently_used.so least_frequently_used.so

OBJS = cut.o cutlist.o cache.o 
CC = gcc
CFLAGS = -Wall

main_lru: main_lru.o $(OBJS)
	$(CC) -o main_lru main_lru.o $(CFLAGS) $(OBJS) -ldl

main_lfu: main_lfu.o $(OBJS)
	$(CC) -o main_lfu main_lfu.o $(CFLAGS) $(OBJS) -ldl

least_recently_used.so: least_recently_used.c cutlist.h
	$(CC) -fPIC -shared -o least_recently_used.so least_recently_used.c cut.c cutlist.c  -ldl

least_frequently_used.so: least_frequently_used.c cutlist.h
	$(CC) -fPIC -shared -o least_frequently_used.so least_frequently_used.c cut.c cutlist.c  -ldl

main_lru.o: main_lru.c cut.h cache.h cutlist.h

main_lfu.o: main_lfu.c cut.h cache.h cutlist.h

cut.o: cut.c cut.h

cutlist.o: cutlist.c cutlist.h cut.h

cache.o: cache.c cache.h cutlist.h

clean:
	rm -f main_lru main_lfu main_lru.o main_lfu.o $(OBJS)
