.PHONY:main
all:main

main:
	g++ -std=c++11 -g ds.cpp main.cpp -o main

clean:
	/bin/rm -rf *.o main