hsltt: main.cpp Makefile
	g++ main.cpp -lcpr -o hsltt

test: hsltt main.cpp
	./hsltt
