all: diskSim

diskSim: diskSim.cpp diskSim.h
		g++ -Wall -Werror -std=c++11 -g -o $@ $?
		
clean:
		-rm -rf diskSim