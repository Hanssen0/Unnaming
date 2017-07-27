objs = objs/main.o objs/Window.o objs/Game.o
Unnaming/Unnaming.out : $(objs)
	g++ -o Unnaming/Unnaming.out $(objs) -lSDL2 -lSDL2_ttf -lSDL2_image

objs/main.o : main.cpp StdHead.h Error.h Game.h Window.h
	g++ -c -o objs/main.o main.cpp

objs/Window.o : Window.cpp Window.h StdHead.h Error.h
	g++ -c -o objs/Window.o Window.cpp

objs/Game.o : Game.cpp Game.h StdHead.h Window.h Error.h
	g++ -c -o objs/Game.o Game.cpp

.PHONY : clean

clean :
	rm $(objs) Unnaming/Unnaming.out
