CC=g++
CFLAGS = -g -std=c++11
OBJS=tbl.o codec.o util.o ../pflayer/pflayer.a

dblayer.a: $(OBJS)
	ld -r -o dblayer.a $(OBJS)

../pflayer/pflayer.a:
	cd ../pflayer; make

tbl.o : tbl.c tbl.h
	$(CC) -c $(CFLAGS) tbl.c

codec.o: codec.h codec.c
	$(CC) -c $(CFLAGS) codec.c

util.o: util.h util.c tbl.h
	$(CC) -c $(CFLAGS) util.c

clean:
	rm  -rf *.o *.a a.out* *~ data.db* *db
