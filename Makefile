test: individual.hpp config.hpp debug.hpp test_individual.cpp
	g++ -O2 -std=c++2a -o test_individual.out test_individual.cpp

main: individual.hpp population.hpp config.hpp debug.hpp main.cpp
	g++ -O2 -std=c++2a -ggdb -o main.out main.cpp

clean:
	rm -rf *.out *.s core