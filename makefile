objects = main.o findgraph.o graph.o graphfiles.o

run: $(objects)
	gcc -o run $(objects)

main.o: main.c
	gcc -g -c main.c

findgraph.o: findgraph.c
	gcc -g -c findgraph.c

graph.o: graph.c
	gcc -g -c graph.c

graphfiles.o: graphfiles.c
	gcc -g -c graphfiles.c

clean:
	rm $(objects) run testfile
