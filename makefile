objects = main.o findgraph.o graph.o

run: $(objects)
	gcc -o run $(objects)

main.o: main.c
	gcc -g -c main.c

findgraph.o: findgraph.c
	gcc -g -c findgraph.c

graph.o: graph.c
	gcc -g -c graph.c

clean:
	rm $(objects) run testfile
