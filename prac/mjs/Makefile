all: main
main: mjs.cpp scanner.o parser.o interpreter.o
	g++ scanner.o parser.o interpreter.o mjs.cpp -o main
scanner.o: scanner.cpp
	g++ -c scanner.cpp
parser.o: parser.cpp scanner.cpp
	g++ -c parser.cpp
interpreter.o: interpreter.cpp parser.cpp scanner.cpp
	g++ -c interpreter.cpp
run: main
	./main
clean:
	rm -rf *.o main
