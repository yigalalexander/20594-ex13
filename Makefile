
FLAGS = -Wall  -L./ -L /usr/src/linux-headers-3.13.0-43-generic/include/linux/ -m32

all: clean my_cd 

my_cd: my_cd.c 
	gcc ${FLAGS} my_cd.c -o my_cd -lm
	
clean:
	rm -f my_cd 

