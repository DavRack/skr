# skr

*ALPHA!*

una piesa de software que permite:

+ Remapear teclas
+ Lanzar comandos shell con una combinación de teclas
+ Crear capas

[Tabla de keyCodes](docs/keyCodes.md)

## Limitaciones

Si bien skr permite combinaciones de hasta 8 teclas,
el ["Key Rollover"](https://en.wikipedia.org/wiki/Rollover_%28key%29) del teclado
puede limitar el número máximo de teclas reconocidas al mismo tiempo.

Nota: Este software fue probado en ArchLinux con kernel 5.6.6-arch1-1

## Dependencias

[interception-tools](https://gitlab.com/interception/linux/tools)

