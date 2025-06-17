
# Taller-Compilador-ChileanCode 🇨🇱
Compilador Chileno, en C++ con Flex y Bison
## Analisis Lexico 📝
### Palabras Reservadas
Estas son las palabras reservadas de nuestro lenguaje
#### Condicionales
| Chileno       | Significado |
|---------------|-------------|
| `si_po`       | IF          |
| `si_no_po`    | ELSE        |

#### Ciclos
|   Chileno        | Significado |
| ---------------- | ----------- |
| `mientras_la_wa` | `while`     |
| `pa_cada`        | `for`       |


#### Funciones
| Chileno             | Significado |
|--------------------|-------------|
| `hace_la_pega`     | FUNCTION    |
| `devuelve_la_wa`   | RETURN      |

#### Entrada / Salida
| Palabra clave   | Significado |
|------------------|-------------|
| `lee_la_wa`      | INPUT       |
| `suelta_la_wa`   | PRINT       |

#### Tipos de datos
| Chileno                 | Significado |
|-------------------------|-------------|
| `numerito`              | int         |
| `numerito_con_punto`    | float       |
| `palabrita`             | string      |

#### Operadores de Comparación:
| Chileno      | Equivalente |
| ------------ | ----------- |
| `igualito`   | `==`        |
| `igualitont` | `!=`        |
| `igualitito` | `<=`        |
| `igualitote` | `>=`        |

---
### Tokens definidos
Tokens que el compilador chileno reconoce:
#### Identificadores Literales

| Token    | Tipo          |
| -------- | ------------- | 
| `NUM`    | Entero        | 
| `FLOAT`  | Flotante      | 
| `STRING` | Cadena        | 
| `ID`     | Identificador | 
#### Palabras clave
| Token         | Significado        |
| ------------- | ------------------ |
|  `IF`          | Condicional `if`   |
| `ELSE`        | Condicional `else` |
| `WHILE`       | Bucle `while`      |
| `FOR`         | Bucle `for`        |
| `PRINT`       | `print`            |
| `FUNCTION`    | Declarar función   |
| `RETURN`      | `return`           |
| `LEE`         | Leer input         |
|  `TIPO_INT`    | Tipo `int`         |
| `TIPO_FLOAT`  | Tipo `float`       |
|  `TIPO_STRING` | Tipo `string`      |

#### Operadores y comparadores
| Token | Significado        |
|  ----- | ------------------ |
| `'='` | Asignación(=)       |
|`'+'`  | Suma(+)             |
| `'-'` | Resta (-)          |
| `'*'` | Multiplicación (*)  |
| `'/'` | División (/)           |
| `'<'` | Menor que (<)        |
| `'>'` | Mayor que (>)          |
| `EQ`  | Igual (==)         |
| `NEQ` | Distinto (!=)      |
| `LEQ` | Menor o igual (<=) |
|  `GEQ` | Mayor o igual (>=) |

## Análisis sintáctico 🖥️
La gramática está definida en Bison, generando un Árbol de Sintaxis Abstracta (AST) para luego evaluarse y traducirse a C++.
### Estructuras soportadas (ejemplos)
#### Declaraciones
```
numerito x;
numerito x = 5;
numerito_con_punto pi = 3.14;
palabrita nombre = "Matias";

``` 
#### Ciclos
```
mientras_la_wa (x < 10) {
    x = x + 1;
}
pa_cada (numerito i = 0; i < 5; i = i + 1) {
    suelta_la_wa i;
}

```
#### Condicionales
```
si_po (x > 0) {
    suelta_la_wa "positivo";
} si_no_po {
    suelta_la_wa "negativo o cero";
}

```
### Funciones
```
hace_la_pega saludo(name) {
    palabrita hola="Hola " + name;
    devuelve_la_wa hola;
}

palabrita nombre = "Matias";
suelta_la_wa saludo(nombre);

```
### Input/Output
```
lee_la_wa nombre;
suelta_la_wa nombre;
```

## Manual 📖
Una vez obtenido el repositorio.
### Compilar
```bison -d chileno.y```
```mv chileno.tab.c chileno.tab.cpp```
```flex chileno.l```
```g++ chileno.tab.cpp lex.yy.c ast.cpp -o chileno_compilador```

### Ejecutar
Ejecuta el comando requerido para el test ha probar.
#### Test ciclos
```./chileno_compilador test/ciclos.txt```
#### Test condicionales
```./chileno_compilador test/condicionales.txt```
#### Test funciones
```./chileno_compilador test/funciones.txt```
#### Test ciclos
```./chileno_compilador test/input.txt```
#### Test completo
```./chileno_compilador test/completo.txt```
### Ejercicio Profesor
```./chileno_compilador ejercicio_profesor/ejercicio.txt```
### ¿Que muestra por pantalla?
```
Primero imprime el arbol de sintaxis abstracta.
Luego imprime lo que ejecuta el programa.
Finalmente imprime que se ha generado el codigo en c++.
```
#### Ejemplo
##### Codigo ha ejecutar
```
hace_la_pega saludo(name) {
    palabrita hola="Hola " + name;
    devuelve_la_wa hola;
}

palabrita nombre = "Matias";
suelta_la_wa saludo(nombre);
```
##### Lo que se imprime por pantalla
```
--- Arbol de sintaxis generado ---
SEQ
  SEQ
    FUNC_DEF: saludo
      PARAMS
        name
      SEQ
        SEQ
          DECLARACION: hola Tipo: string
          ASSIGN
            ID: hola
            BINOP (+)
              STRING: Hola
              ID: name
        RETURN
          ID: hola
    SEQ
      DECLARACION: nombre Tipo: string
      ASSIGN
        ID: nombre
        STRING: Matias
  PRINT
    FUNC_CALL: saludo
      ARGS
        ID: nombre

--- Ejecucion del programa ---
Hola Matias

--- Generando codigo C++ ---
Archivo generado: cpp_chileno.cpp
```

### Compilar archivo generado C++
```
g++ cpp_chileno.cpp -o cpp_chileno.exe
```
### Ejecutar archivo generado C++
```
./cpp_chileno
```
## Disfruta y divierte programando en Chileno!!😉
