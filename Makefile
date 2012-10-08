GCC_OPTIONS=-Wall -pedantic -I ../include
GL_OPTIONS= -framework GLUT -framework OpenGL
OPTIONS=$(GCC_OPTIONS) $(GL_OPTIONS)

all:	main run

main: objects/objects.o main.o
	g++ includes/initShader.o kixor/objLoader.o kixor/obj_parser.o kixor/list.o kixor/string_extra.o objects/object.o main.o $(OPTIONS) -o main

objects/objects.o: objects/object.cpp objects/object.h
	g++ -c objects/object.cpp -o objects/object.o
	
main.o: main.cpp
	g++ -c main.cpp  -o main.o

clean:
	rm *.o objects/object.o  main

run:
	./main
