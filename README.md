<h1 align="center"> Simple Key Remap </h1>

### Introduction

SKR is a low level, low latency way to remap keys to:

+ Other keys
+ Shell commands or scripts
+ Secuences of keys and/or commands/scripts
+ Function Layers

[Tabla de keyCodes](docs/keyCodes.md)

### Instalation

```shell
mkdir skr  
cd skr
git clone https://github.com/DavRack/skr.git
```

### Examples

> Create the config file at: ~/.config/skr/skr.config

Remap CapsLock to Esc

```vim
CapsLock -> Esc
```

or with explicit keyCodes

```vim
KeyCode(58) -> KeyCode(58)
```

Swap Meta ("Windows" key) with left alt

```vim
META <-> ALT
```

Remap H,J,K,L  to left, down, up and right arrow keys (like vim) using
CapsLock as fnLayer

> when a fnLayer is defined it's fnKey will **only** work as fnKey

```vim
NewLayer -> CapsLock
    H -> LEFT
    J -> DOWN
    K -> UP
    L -> RIGHT
```

### Limitaciones

Si bien skr permite combinaciones de hasta 8 teclas,
el ["Key Rollover"](https://en.wikipedia.org/wiki/Rollover_%28key%29) del teclado
puede limitar el número máximo de teclas reconocidas al mismo tiempo.

Nota: Este software fue probado en ArchLinux con kernel 5.6.6-arch1-1

### Dependencias

[interception-tools](https://gitlab.com/interception/linux/tools)

### todo

+ md install
+ md custom macros
+ dont run non sudo commands as root

