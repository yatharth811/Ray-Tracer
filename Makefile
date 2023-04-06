FILE=main
FLAGS=`sdl2-config --cflags --libs` -lSDL2_image -Werror -o

all:
	g++ $(FILE).cpp $(FLAGS) $(FILE).out
	./$(FILE).out

clean:
	rm -f *.out