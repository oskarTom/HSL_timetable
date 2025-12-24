hsl: main.cpp Makefile
	g++ main.cpp -lcpr -o hsltt

test: hsltt
	./hsltt
