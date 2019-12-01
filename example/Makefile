all:
	gcc -o count `pkg-config --cflags --libs glib-2.0` count.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm
	gcc -o calculate `pkg-config --cflags --libs glib-2.0` calculate.c `pkg-config --libs glib-2.0` ../lib/libstemmer.o -lm
clean:
	rm count
	rm calculate
