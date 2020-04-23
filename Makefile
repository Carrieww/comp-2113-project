FLAGS = -pedantic-errors -std=c++11

level_1.o: level_1.cpp level.h
	g++ $(FLAGS) -c $<

level_2.o: level_2.cpp level.h
	g++ $(FLAGS) -c $<

level_3.o: level_3.cpp level.h
	g++ $(FLAGS) -c $<

level_4.o: level_4.cpp level.h
	g++ $(FLAGS) -c $<

main.o: main.cpp level.h
	g++ $(FLAGS) -c $<

main: main.o level_1.o level_2.o level_3.o level_4.o
	g++ $(FLAGS) $^ -o $@

clean:
	rm -f main level_1.o level_2.o level_3.o level_4.o main.o main.tgz

tar:
	tar -czvf main.tgz *.cpp *.h

.PHONY: clean tar
