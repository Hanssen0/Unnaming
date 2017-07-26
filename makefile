objs = main.o Window.o Game.o
Unnaming/Unnaming.out : $(objs)
	g++ -o Unnaming/Unnaming.out $(objs) -lSDL2 -lSDL2_ttf -lSDL2_image

main.o : main.cpp StdHead.h Error.h Game.h Window.h
	g++ -c -o main.o main.cpp

Window.o : Window.cpp Window.h StdHead.h Error.h
	g++ -c -o Window.o Window.cpp

Game.o : Game.cpp Game.h StdHead.h Window.h Error.h
	g++ -c -o Game.o Game.cpp

.PHONY : clean

clean :
	rm $(objs) Unnaming/Unnaming.out
