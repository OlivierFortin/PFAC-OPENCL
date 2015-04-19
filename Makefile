OS := $(shell uname)
OPTIONS:= -Wmultichar

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenCL
else
	OPTIONS += -I/usr/local/browndeer/include -L/usr/local/browndeer/lib -lcoprthr_opencl -std=c99
endif

main: src/main.c
	 gcc  $(OPTIONS) -Wall -g src/main.c -o bin/main

clean:
	rm -rf bin/main
