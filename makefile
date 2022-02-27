# all: main.out

first.out: main.o
	g++ main.o -o main.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

main.o: main.cpp
	g++ -c main.cpp

clean:
	rm main.out main.o