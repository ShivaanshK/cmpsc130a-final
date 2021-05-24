.PHONY:main
all:main

main:
	g++ -std=c++11 ds.cpp rb.cpp -o main

clean:
	/bin/rm -rf *.o main
