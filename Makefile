my_exe: main.o graph.o
	g++ -o my_exe main.o graph.o

graph.o: GraphHandler.cpp
	g++ -c -o graph.o GraphHandler.cpp

main.o: main.cpp
	g++ -c -o main.o main.cpp

clean:
	rm *.o my_exe
