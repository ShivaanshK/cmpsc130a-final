.PHONY:rb
all:rb

rb:
	g++ -std=c++11 ds.cpp rb.cpp -o rb

clean:
	/bin/rm -rf *.o rb
