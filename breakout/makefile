all: program.out

program.out: program.o glad.o game.o resource_manager.o shader.o texture.o
	g++ program.o glad.o game.o resource_manager.o shader.o texture.o -o program.out -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi

program.o: program.cpp
	g++ -c program.cpp

game.o: game.cpp
	g++ -c game.cpp

resource_manager.o: resource_manager.cpp
	g++ -c resource_manager.cpp

shader.o: shader.cpp
	g++ -c shader.cpp

texture.o: texture.cpp
	g++ -c texture.cpp

glad.o: ../glad.c
	gcc -c ../glad.c

clean:
	rm program.out program.o game.o resource_manager.o shader.o texture.o glad.o