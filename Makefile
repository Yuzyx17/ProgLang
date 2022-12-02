#executable
exec = lome.exe
#finding sources of makefile in src folder with extence .c
sources = $(wildcard src/*.c)
#conpile into executable
objects = $(sources:.c=.o)
#for debugging purposes
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

#creatiing the o files - whenever there is a detection in our source file, recreate the executable
%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
	cp ./lome.exe/usr/local/bin/new
#clean command
clean:
	-rm *.exe
	-rm *.o
	-rm src/*.o

