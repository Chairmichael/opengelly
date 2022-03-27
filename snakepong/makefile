flags = -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
staticfiles = shader.cpp resource_manager.cpp texture.cpp post_processor.cpp stb_image.cpp glad.c
dynamicfiles = program.cpp game.cpp renderer2d.cpp
srcfiles = $(dynamicfiles) $(staticfiles)
objfiles = program.o game.o shader.o resource_manager.o texture.o renderer2d.o post_processor.o stb_image.o glad.o
outfile = program.out

default: $(objfiles)
	g++ -o $(outfile) $(objfiles) $(flags)

re:
	g++ -c $(dynamicfiles)
	g++ -o $(outfile) $(objfiles) $(flags)

# all:
# 	rm $(objfiles) $(outfile)
# 	g++ -o $(outfile) $(objfiles) $(flags)

# $(objfiles):
# 	g++ -c $(srcfiles)

program.o:
	g++ -c program.cpp

game.o:
	g++ -c game.cpp

shader.o:
	g++ -c shader.cpp

resource_manager.o:
	g++ -c resource_manager.cpp

game_object.o:
	g++ -c game_object.cpp

renderer2d.o:
	g++ -c renderer2d.cpp

texture.o:
	g++ -c texture.cpp

post_processor.o:
	g++ -c post_processor.cpp

stb_image.o:
	g++ -c stb_image.cpp

glad.o:
	gcc -c glad.c

clean:
	rm $(objfiles) $(outfile)