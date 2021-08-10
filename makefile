# makefile

nome = controladora

C_SOURCE = $(wildcard ./source/*.c)
H_SOURCE = $(wildcard ./source/*.h)
OBJ  = $(subst source,obj,$(C_SOURCE:.c=.o))

CC = gcc

flags = \
-lpthread \
-lm


all: $(nome)

$(nome): $(OBJ)
	$(CC) -o $(nome) $(OBJ) $(flags)

./obj/%.o: ./source/%.c $(H_SOURCE)
	$(CC) -o $@ $< -c

./obj/main.o: ./source/main.c $(H_SOURCE) 
	$(CC) -o ./obj/main.o ./source/main.c -c

clean:
	rm -rf ./obj/*.o *~ $(nome)
