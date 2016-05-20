CXX=g++
CXXFLAGS=-O3 -std=c++11
LIBS=-lntl -lgmp -lgf2x #install NTL with gf2x and gmp


main: main.cpp 

	    $(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@ 
