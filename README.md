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
    ├──  build
    │   ├──  main
    │   ├──  test
    ├──  headers
    │   ├──  mtree.hpp
    │   ├──  point.hpp
    │   └──  utils
    │       ├──  closest_pair.hpp
    │       └──  random.hpp
    ├──  Makefile
    ├──  README.md
    ├──  results
    │   ├──  img/
    │   ├──  Informe.pdf
    │   ├──  results.pdf
    │   ├──  cp/
    │   └──  ss/
    └── 󱧼 src
        ├──  main.cpp
        ├──  mtree
        │   ├──  mtree.cpp
        │   ├──  mtree_create_cp.cpp
        │   ├──  mtree_create_ss.cpp
        │   └──  point.cpp
        ├──  test.cpp
        └──  utils
            ├──  closest_pair.cpp
            └──  random.cpp




## Resultados
Los resultados de la experimentación se encuentran disponibles en la carpeta ./results.

### Algunos resultados 

#### Método Ciaccia-Patella 
![Alt](./results/img/cp_13.png)


#### Método Sexton-Swinbank 
![Alt](./results/img/ss_13.png)

