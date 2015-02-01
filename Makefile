
FLAGS = -Wall  -L./  -m32 -lm -g3

all: clean disk_util.o my_cd.o my_cd my_dir

my_dir: my_dir.o disk_util.o
	gcc ${FLAGS} my_dir.o disk_util.o -o my_dir -lm

my_cd: my_cd.o disk_util.o
	gcc ${FLAGS} my_cd.o disk_util.o -o my_cd -lm

my_dir.o: my_dir.c
	gcc ${FLAGS} -c my_dir.c

my_cd.o: my_cd.c
	gcc ${FLAGS} -c my_cd.c

disk_util.o: disk_util.h disk_util.c
	gcc ${FLAGS} -c disk_util.h disk_util.c

clean:
	rm -f my_cd my_dir *.o

