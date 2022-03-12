# Corto Paralela OMP Final

## Resultados

Los resultados varían dependiendo de dónde se hace el paralelismo. Los resultados más obvios son a partir del generador de números random, y el uso de for's en el programa. Sin embargo, hay que tener cuidado, ya que el uso de un for parallelo, en alguno de estos, termina trabando el programa. Sin embargo, solo con este cambio, ya se tiene un speedup del 1.0331598%. Aunque pequeño, baja la latencia del programa. El segundo speedup viene en el uso de secciones. Estas secciones permiten un speedup más grande, del 1.2008937%. Esto baja la ejecución del programa, en promedio, casi un segundo entero.
Para paralelizar más, también se trató de añadir una sección de secciones dentro del parallel quicksort. Sin embargo, esto trababa completamente el programa, por lo que no se hizo así.

## Tablas

Las siguientes tablas utilizan un N de 10000000

### Secuencial

| Corrida |  Tiempo  |
| :-----: | :------: |
|    1    | 5.991057 |
|    2    | 5.947484 |
|    3    | 5.844406 |
|    4    | 5.824116 |
|    5    | 5.824116 |

| Promedio  |
| :-------: |
| 5.8862358 |

### Paralelo

#### Generador de Randoms

| Corrida |  Tiempo  |
| :-----: | :------: |
|    1    | 5.772528 |
|    2    | 5.694279 |
|    3    | 5.674470 |
|    4    | 5.669798 |
|    5    | 5.675493 |

| Promedio  |
| :-------: |
| 5.6973136 |

|  Speedup  |
| :-------: |
| 1.0331598 |

#### Sorting utilizando Sections + Generador de Randoms

| Corrida |  Tiempo  |
| :-----: | :------: |
|    1    | 4.827392 |
|    2    | 4.993839 |
|    3    | 4.937832 |
|    4    | 4.738384 |
|    5    | 5.010283 |

| Promedio |
| :------: |
| 4.901546 |

|  Speedup  |
| :-------: |
| 1.2008937 |
