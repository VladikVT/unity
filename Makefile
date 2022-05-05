.PHONY: all clean install uninstall

PREFIX=/usr/local
CXX=g++
CXXFLAGS=

all: unity

clean:
	rm -rf unity *.o
main.o: main.cpp
	$(CXX) -c main.cpp
libSCG.o: libs/libSCG.cpp
	$(CXX) -c libs/libSCG.cpp
unity.o: libs/unity.cpp
	$(CXX) -c libs/unity.cpp
unity: main.o libSCG.o unity.o
	$(CXX) main.o libSCG.o unity.o -o unity
install: all
	mkdir -p $(PREFIX)/bin
	install unity $(PREFIX)/bin
uninstall:
	rm -rf $(PREFIX)/bin/unity
