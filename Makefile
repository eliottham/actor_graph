# A simple makefile for CSE 100 PA4

CC=g++
CXXFLAGS=-std=c++11
LDFLAGS=

# if passed "type=opt" at command-line, compile with "-O3" flag (otherwise use "-g" for debugging)

ifeq ($(type),opt)
	    CPPFLAGS += -O3
		    LDFLAGS += -O3
		else
		    CPPFLAGS += -g
			    LDFLAGS += -g
			endif

all: pathfinder main actorconnections



# include what ever source code *.h files pathfinder relies on (these are merely the ones that were used in the solution)
pathfinder: pathfinder.o
pathfinder.o: ActorGraph.o ActorNode.o Movie.o pathfinder.cpp

actorconnections: actorconnections.o
actorconnections.o: ActorGraph.o ActorNode.o Movie.o util.o 
util.o: util.h	


# include what ever source code *.h files ActorGraph relies on (these are merely the ones that were used in the solution)

# Note: you do not have to include a *.cpp file if it aleady has a paired *.h file that is already included with class/method headers


main: main.o
main.o: ActorNode.o Movie.o ActorGraph.o main.cpp
ActorNode.o: ActorNode.h

Movie.o: ActorNode.o Movie.h
ActorGraph.o: ActorGraph.h Movie.o 


clean:
		rm -f all *.o core*


