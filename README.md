# TFG_Beneharo_agua
Repositorio para el código del TFG de Beneharo Nuñez Castellano

## Envío a BBB

Para el envío del código a la BBB (por ejemplo la `bbb`) usar comando:

```bash
rsync  -av --exclude .git --exclude build --exclude doc $PWD debian@bbb:./
```

Creamos script `EnviaBBB.sh` para hacer la sincronización.


## Código

El código fuente ha de estar en el directorio raiz.

Las clases deben comenzar en mayúscula (`Clase`) y estar definidas en
dos ficheros: `Clase.hpp` y `Clase.cpp`.

El código de los programas (que contengan el `main`) deben estar en ficheros
que comiencen por `test_` y con la extensión `.cpp`, como por ejemplo
`test_GPIO_valor_1.cpp`.

Los ficheros objeto (`.o`) y los ejecutables se generan en el subdirectorio
`build/`.

### Compilación

La compilación se hace a través de `make` utilizando el fichero `Makefile`
definido.

Invocando `make` (sin argumentos) trata de hacer todos los ejecutables.

Si deseamos hacer un ejecutable concreto (`test_GPIO_valor_1`) debebmos invocar

```
make build/test_GPIO_valor_1
```

ya que los ejecutables se crean en carpeta `buid/`.

### Ejecución

Para la ejecución invocar `build/test_GPIO_valor_1`

## Documentación

Las clases proporcionadas están documentadas con
[Doxygen](http://www.doxygen.org/index.html).

La documentación se genera invocando `make docu`.

Se accedea a ella a partir de `doc/html/index.html`.

Podemos incluir comentarios *doxygen* en nuevas clases o ejecutables.
Al generar la documentación se incluirá dicha información nueva.

## BBB_GPIO

### Valores I/O

Para manejar los pines como GPIO podemos usar el estandar del Kernel
en la carpeta `/sys/class/gpio` exportando el pin etc. como
se indica en [este documento](https://elinux.org/GPIO)
o en [este otro](https://www.mjmwired.net/kernel/Documentation/gpio).

Lo primero es *exportar* el pin escribiendo su `<número>` en
`/sys/class/gpio/export`. Aquí el `<numero>` es como se identifica
el el kernel dicho pin.

Entonces aparecerá subdirectorio `/sys/class/gpio/gpio<numero>` donde
destacan los ficheros

- `direccion`: para fijar dirección `in` o `out`.

- `value`: al leer devuelve valores `0` o `1`. Si es de salida
se escribe `0` para salida a baja y algo distinto de cero para
salida en alta.
Sobre este fichero hay que hacer `poll()` si están configuradas
la detección por flanco.

- `edge`: admite valores `none`, `rising`, `falling` o `both` para
indicar el flanco que generará *interrupción* haciendo `poll()` en
`value`.

- `active_low`: devuelve `0` o `1`. Escribiendo algo distinto de cero
invierte el valor. No queda clara al funcionalida ¿invertir la lógica
del pin?

### Pin-mux

Pero para que lo anterior tenga efecto es necesario configurar adecuadamente
el *pin-mux* de los pines.

Eso vemos, investigando en `config-pin`, que se hace
(en los kernels > 4) a través de
`/sys/devices/platform/ocp/ocp:<pin>_pinmux/`
donde `<pin>` se expresa según la posición, es decir, algo como `P8_40`.

En ese direcctorio existe el fichero `state` en el que se escribe el
valor que queremos que tenga el *pin-mux*.
Los valores interesantes para el GPIO parecen ser:

- `gpio_pd`: GPIO con pull-down
- `gpio_pu`: GPIO con pull-up
- `gpio`: GPIO con ambos pull deshabilitados

### Doble identificación de los pines

Como en el interfaz GPIO hace falta el numero del pin que entiende
el kernell, y para pin-mux (y para localizar más facilmente el pin)
es necesaria la posición del pin, es necesaria tabla para relacionar
posición con número de pin, como figura en el apartado de
*Pines considerados*.

## BBB_I2C_LCD

Clase para la interacción con LCD con controlador Hitachy HD44780U
conectado por I2C a la BeagleBoard Black (BBB).

Basado en el código [Arduino-LiquidCrystal-I2C-library][1]
 de [fdebrabander](https://github.com/fdebrabander)

[1]: https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library


