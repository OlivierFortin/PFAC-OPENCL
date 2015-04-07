OS := $(shell uname)
OPTIONS:= -Wmultichar

ifeq ($(OS),Darwin)
	OPTIONS += -framework OpenCL
else
	OPTIONS += -std=gnu99  -I/usr/local/browndeer/include -L/usr/local/browndeer/lib -lstdcl
endif

main: main.c
	 gcc  -Wall -g main.c -o /home/linaro/PFAC-OPENCL4/main2 -I/usr/local/browndeer/include -L/usr/local/browndeer/lib -lcoprthr_opencl

clean:
	rm -rf main
