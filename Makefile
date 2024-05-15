all: diskSim

diskSim: diskSim.cpp
		g++ -Wall -Werror -std=c++11 -g -o $@ $?
		
clean:
		-rm -rf schedule