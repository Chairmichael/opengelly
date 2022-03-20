flags = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
srcfiles = snakepong.cpp game.cpp glad.c shader.cpp resource_manager.cpp
objfiles = snakepong.o game.o glad.o shader.o resource_manager.o

all:
	g++ -o main.out $(srcfiles) $(flags)

snakepong.o:
	g++ -c snakepong.cpp

game.o:
	g++ -c game.cpp

glad.o:
	gcc -c glad.c

shader.o:
	g++ -c shader.cpp

resource_manager.o:
	g++ c resource_manager.cpp