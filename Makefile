CC = gcc

## agregar flags? como -g o -Wall
CFLAGS = -g -Wall

## agregar librerias extra
LDLIBS= -lm

TARGET=./build/main
TEST=./build/test

## AQUI INCLUIR LOS ARCHIVOS .C 
## excepto main y test
## agregar un "\" al final de cada nombre
SRC = \
	  src/mtree.c\
	  src/point.c



## nombre de los headers (automatico)
HDRS = $(wildcard headers/*.h)
## nombre de los .o (automatico)
OBJS = $(SRC:./src/%.c=./build/%.o)



all:
	@echo " MTree implementation proyect"
	@echo ""
	@echo " use \"make help\" to get more info"

## Compila un .o a partir del src
./build/%.o: ./src/%.c $(HDRS)
	@ $(CC) $(CFLAGS)  -c $< -o $@

## Compila el ejecutable del main y ejecuta
run: $(OBJS) $(TARGET).o
	@ $(CC) $(CFLAGS) $^ -o $(TARGET) $(LDLIBS) && $(TARGET)

## Compila el ejecutable de los test y ejecuta
test: $(OBJS) $(TEST).o
	@ $(CC) $(CFLAGS) $^ -o $(TEST) $(LDLIBS) && $(TEST)

## borra todo lo que se compilo
clean: $(wildcard build/*)
	@ rm -f $^


help:
	@echo " to run main --> make run"
	@echo " to run test --> make test"
	
