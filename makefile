objects = main.o findgraph.o stack.o graph.o

run: $(objects)
	gcc -o run $(objects)

main.o: main.c
	gcc -c main.c

findgraph.o: findgraph.c
	gcc -c findgraph.c

stack.o: stack.c
	gcc -c stack.c

graph.o: graph.c
	gcc -c graph.c

clean:
	rm $(objects) run testfile
