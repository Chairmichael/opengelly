# all: main.out

main.out: main.o glad.o
	g++ main.o glad.o -o main.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

main.o: main.cpp
	g++ -c main.cpp

glad.o: glad.c
	gcc -c glad.c

clean:
	rm main.out main.o glad.o test.o