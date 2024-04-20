# Tareas-Logaritmos  
Contenido de las tareas de logaritmos. Que el poder de Navarro te acompañe.

Para ejecutar el main:  
  make run
    
Para ejecutar test:  
    make test

Para limpiar los archivos compilados:  
    make clean

Para agregar un archivo:  
    1- Crear .h y agregar a Headers/  
    2- Crear .c y agregar a src/  
    3- Agregar nombre "name.c" al archivo Makefile   


## Estructura de los archivos  

-> src          ## Aqui se agregan los codigos  
    -> main.c   
    -> test.c  
    -> file.c  
-> Headers      ## Aqui se agregan los archivos .h  
    -> file.h  
-> Build        ## Aqui se agregan los archivos generados en la compilacion  
    -> main  
    -> test  
    -> file.o  
-> Makefile     ## Archivo con todos los comandos para compilar y ejecutar  

