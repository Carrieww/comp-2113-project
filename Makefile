FLAG = -lncurses
FLAGS2 = -pedantic-errors -std=c++11

floor_1.o: floor_1.cpp floor.h
	g++ $(FLAGS2) -c $<

floor_2.o: floor_2.cpp floor.h
	g++ $(FLAGS2) -c $<

floor_3.o: floor_3.cpp floor.h
	g++ $(FLAGS2) -c $<

floor_4.o: floor_4.cpp floor.h
	g++ $(FLAGS2) -c $<

main.o: main.cpp floor.h
	g++ $(FLAGS2) -c $<

main: main.o floor_1.o floor_2.o floor_3.o floor_4.o
	g++ $(FLAGS2) $^ -o $@ $(FLAG)

clean:
	rm -f main floor_1.o floor_2.o floor_3.o floor_4.o main.o main.tgz

tar:
	tar -czvf main.tgz *.cpp *.h

.PHONY: clean tar
