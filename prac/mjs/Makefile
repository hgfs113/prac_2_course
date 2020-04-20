main: mjs.cpp
	g++ -Wall -Werror mjs.cpp -o main
test: main
	cat readme.md
	./main test.cpp
	./main test1.cpp
	./main test2.cpp
	./main test3.cpp
clean:
	rm -f main
