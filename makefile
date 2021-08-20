# makefile

nome = controladora

C_SOURCE = $(wildcard source/*.c)
H_SOURCE = $(wildcard source/*.h)
OBJ  = $(subst source,obj,$(C_SOURCE:.c=.o))

H_TEST = $(subst source,source/test,$(H_SOURCE:.h=.o))

CC = gcc

flags = \
-lpthread \
-lm

#- Cria o programa -#
all: $(nome)

$(nome): $(OBJ)
	$(CC) -o $(nome) $(OBJ) $(flags)

./obj/%.o: ./source/%.c $(H_SOURCE)
	$(CC) -o $@ $< -c

./obj/main.o: ./source/main.c $(H_SOURCE) 
	$(CC) -o obj/main.o source/main.c -c


#- Testa se os headers compilariam -#
test_headers: test_done

test_done: $(H_TEST)
	rm -f ./source/test/*.o

./source/test/%.o: ./source/test/test_%_h.c ./source/%.h
	$(CC) -o $@ $< -c


#- Limpa os arquivos -#
clean:
	rm -rf ./obj/*.o *~ $(nome)
