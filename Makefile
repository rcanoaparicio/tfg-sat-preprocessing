all:
	g++ -std=c++14 -I/usr/include main.cpp -L/usr/lib/ -lboost_coroutine -lboost_context -lboost_system -O2 -o main
	g++ probing.cpp -std=c++14 -O2 -o probing
	g++ bronkerbosch.cpp -std=c++14 -O2 -o bronkerbosch
	g++ graphGenerator.cpp -std=c++14 -O2 -o graphGenerator
