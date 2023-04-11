FILE=main
FLAGS=-g -fopenmp -O2 `sdl2-config --cflags --libs` -lSDL2_image -Werror -o

all:
	g++ $(FILE).cpp $(FLAGS) $(FILE).out
	./$(FILE).out

e1:
	g++ examples/e1.cpp $(FLAGS) examples/e1.out

e2:
	g++ examples/e2.cpp $(FLAGS) examples/e2.out

e3:
	g++ examples/e3.cpp $(FLAGS) examples/e3.out

e4:
	g++ examples/e4.cpp $(FLAGS) examples/e4.out


clean:
	rm -f *.out