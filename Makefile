.PHONY: all clean install uninstall

all: unity

clean:
	rm -rf unity *.o
main.o: main.cpp
	g++ -c main.cpp
libSCG.o: libs/libSCG.cpp
	g++ -c libs/libSCG.cpp
unity.o: libs/unity.cpp
	g++ -c libs/unity.cpp
unity: main.o libSCG.o unity.o
	g++ main.o libSCG.o unity.o -o unity
install:
	install ./unity /usr/local/bin
uninstall:
	rm -rf /usr/local/bin/unity


