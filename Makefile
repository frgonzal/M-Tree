CC = gcc
CFLAGS = -g -Wall
LDLIBS=

TARGET=./Build/main
TEST=./Build/test

## AQUI INCLUIR LOS ARCHIVOS .C
## agregar un "\" al final de cada nombre
SRC = \
	  src/MTree.c\
	  src/Point.c



## nombre de los headers (automatico)
HDRS = $(wildcard Headers/*.h)
## nombre de los .o (automatico)
OBJS = $(SRC:./src/%.c=./Build/%.o)



all:
	@echo " MTree implementation proyect"
	@echo ""
	@echo " use \"make help\" to get more info"

## Compila un .o a partir del src
./Build/%.o: ./src/%.c $(HDRS)
	@ $(CC) $(CFLAGS) $(LDLIBS) -c $< -o $@

## Compila el ejecutable del main y ejecuta
run: $(OBJS) $(TARGET).o
	@ $(CC) $(CFLAGS) $^ -o $(TARGET) && $(TARGET)

## Compila el ejecutable de los test y ejecuta
test: $(OBJS) $(TEST).o
	@ $(CC) $(CFLAGS) $^ -o $(TEST) && $(TEST)

## borra todo lo que se compilo
clean: $(wildcard Build/*)
	@ rm -f $^


help:
	@echo " to run main --> make run"
	@echo " to run test --> make test"
	

