src := $(wildcard src/*.c)
out := lang
cflags := -Isrc/ -lm -std=c99


all: $(src)
	make build
	make run
build: $(src)
	gcc $(src) -o $(out) $(cflags) 
run:
	./$(out)
