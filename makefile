all: main.out snake.out coordsys.out

main.out: main.o glad.o
	g++ main.o glad.o -o main.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

snake.out: snake.o glad.o
	g++ snake.o glad.o -o snake.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

coordsys.out: coordsys.o
	g++ coordsys.o glad.o -o coordsys.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

main.o: main.cpp
	g++ -c main.cpp

glad.o: glad.c
	gcc -c glad.c

snake.o: snake/snake.cpp
	g++ -c snake/snake.cpp

coordsys.o: coordsys.cpp
	g++ -c coordsys.cpp

clean:
	rm main.out snake.out main.o test.o snake.o