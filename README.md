# TFG_Beneharo_agua
Repositorio para el código del TFG de Beneharo Nuñez Castellano

## GPIO

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
