all: main
main: mjs.cpp scanner.o parser.o
	g++ scanner.o parser.o mjs.cpp -o main
scanner.o: scanner.cpp
	g++ -c scanner.cpp
parser.o: parser.cpp scanner.cpp
	g++ -c parser.cpp
run: main
	./main
clean:
	rm -rf *.o main
