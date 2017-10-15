all: tp_2

tp_2: imageBase.o main_2.o
	g++ -o tp_2 imageBase.o main_2.o

imageBase.o: ImageBase.cpp
	g++ -o imageBase.o -c ImageBase.cpp -W -Wall -ansi -pedantic

tp1_compression.o: main_2.cpp ImageBase.h
	g++ -o main_2.o -c main_2.cpp -W -Wall -ansi -pedantic

clean:
	rm -rf *.o

mrproper: clean
	rm -rf tp_2
