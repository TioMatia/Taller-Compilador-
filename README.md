
# Taller-Compilador-ChileanCode
Compilador Chileno, en C++ con Flex y Bison
## Analisis Lexico
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
## Tokens definidos
Tokens que el compilador chileno reconoce:
### Identificadores Literales

| Token    | Tipo          |
| -------- | ------------- | 
| `NUM`    | Entero        | 
| `FLOAT`  | Flotante      | 
| `STRING` | Cadena        | 
| `ID`     | Identificador | 
### Palabras clave
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

### Operadores y comparadores
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


