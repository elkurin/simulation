CXX_OPT=-std=c++11 -Wall -Wextra -Wconversion

a.out: random_network.o reaction.o
	g++ $(CXX_OPT) -o a.out random_network.o reaction.o

random_network.o: random_network.cpp
	g++ $(CXX_OPT) -c random_network.cpp

reaction.o: reaction.cpp
	g++ $(CXX_OPT) -c reaction.cpp

.PHONY: clean
clean: 
	rm -f a.out random_network.o reaction.o
