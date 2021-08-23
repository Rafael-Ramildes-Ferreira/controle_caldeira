# makefile

nome = controladora

C_SOURCE = $(wildcard source/*.c)
H_SOURCE = $(wildcard source/*.h)
OBJ  = $(subst source,obj,$(C_SOURCE:.c=.o))


CC = gcc

flags = \
-lpthread \
-lm

#- Cria o programa -#
all: $(nome)

$(nome): $(OBJ)
	$(CC) -o $(nome) $(OBJ) $(flags)

./obj/%.o: ./source/%.c $(H_SOURCE) ./obj
	$(CC) -o $@ $< -c

./obj:
	mkdir obj

./obj/main.o: ./source/main.c $(H_SOURCE)  ./obj
	$(CC) -o obj/main.o source/main.c -c


#- Limpa os arquivos -#
clean:
	rm -rf ./obj/*.o *~ $(nome)
