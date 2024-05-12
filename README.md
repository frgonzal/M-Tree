# MTree
El M-Tree12 es un método de acceso métrico dinámico, que consiste en un árbol balanceado.




## Ejecutar Proyecto
### Necesario!
El proyecto se construyo en un sistema operativo con linux y se utilizo
la funcionalidad "make" para la compilación y ejecución del proyecto.   
Por lo anterior, es necesario tener la funcionalidad "make" para ejecutar los test y el main.



### Comandos para ejecutar el proyecto

    make run
    make test


### make run
Compila y ejecuta el archivo "main", que contiene un test de uso básico para un MTree.  
Permite crearlo y realizar consultas al MTree creado.   
Este es el archivo para el uso normal del MTree.

### make test
Compila y ejecuta el archivo "test", que contiene el código utilizado para la experimentación.  
Crea un MTree utilizando ambos métodos y realiza 100 consultas con el MTree creado.
Esto lo hace para $n \in \{2^{10},...,2^{25}\}$, siendo n el tamaño del conjunto 
de puntos que se utiliza para la creación del MTree.  
Además, el archivo "test" escribirá las pruebas realizadas en la carpeta ./resultados


## Estructura del proyecto

     MTree
    │
    ├──  build                         ## para guardar los ejecutables
    │   ├──  main
    │   ├──  main.o
    │   ├──  test
    │   └──  test.o
    │
    ├──  headers                       ## headers de todo el proyecto, con la documentacion de las funciones y estructuras
    │   ├──  mtree.hpp
    │   ├──  point.hpp
    │   └──  utils
    │       ├──  closest_pair.hpp
    │       └──  random.hpp
    │
    ├── 󱧼 src                           ## archivos del proyecto
    │   ├──  main.cpp                  ## (main) archivo para usar la estructura y funciones
    │   ├──  mtree
    │   │   ├──  mtree.cpp
    │   │   ├──  mtree_create_cp.cpp
    │   │   ├──  mtree_create_ss.cpp
    │   │   └──  point.cpp
    │   ├──  test.cpp                  ## (test) archivo con los test utilizados para obtener los resultados
    │   └──  utils
    │       ├──  closest_pair.cpp
    │       └──  random.cpp
    │
    ├──  Makefile                      ## Makefile del proyecto
    │
    └──  resultados                    ## Los resultados de la ejecucion de los algoritmos
        ├──  cp                        ## Resultados de Ciaccia-Patella
        │   ├──  mtree                 ## resultados de la construccion
        │   │   ├──  result
        │   │   │   └──  power.csv
        │   │   └──  time
        │   │       └──  power.csv
        │   └──  search                ## resultados de la busqueda
        │       ├──  result
        │       │   └──  power.csv
        │       └──  time
        │           └──  power.csv
        ├──  img                       ## imagenes de MTrees creados
        │   ├──  cp_power.png
        │   └──  ss_power.png
        ├──  read.ipynb                ## Archivo ipynb para leer los resultados y obtener graficos para interpretar
        ├──  requirements.txt          ## Archivo con los modulos necesarios para ejecutar read.ipynb
        └──  ss                        ## Resultados de Sexton-Swinbank
            ├──  mtree
            │   ├──  result
            │   │   └──  power.csv
            │   └──  time
            │       └──  power.csv
            └──  search
                ├──  result
                │   └──  power.csv
                └──  time
                    └──  power.csv




## Resultados

Los resultados de la experimentación se encuentran disponibles en el archivo read.ipynb, 
el cual lee los archivos generados por "test" para generar gráficos y figuras con la información
obtenida.


### Algunos resultados 

#### Método Ciaccia-Patella 
![Alt](./resultados/img/cp_13.png)


#### Método Sexton-Swinbank 
![Alt](./resultados/img/ss_13.png)




Que el poder de Navarro te acompañe.