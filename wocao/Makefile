all: a5-ece650
.PHONY : all
a5-ece650:a5-ece650.o
	g++ -o a5-ece650 a5-ece650.o -L. -lsat -pthread -lrt
a5-ece650.o:a5-ece650.c
	g++ -c a5-ece650.c
clean:
	rm a5-ece650 a5-ece650.o
	
