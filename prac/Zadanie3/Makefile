all: main
main:main.cpp
	g++ main.cpp -Wall -Werror -o main
run: main
	./main
runval: main
	valgrind ./main
clean:
	rm -f main
