#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define BLANK -1

# define TRUE 1
# define FALSE 0

# define EV_SIZE 24

# define TECLA_MANTENIDA 2
# define TECLA_PRESIONADA 1
# define TECLA_SOLTADA 0

// define la estructura de un remapeo de una tecla
struct keyRemap {
    // Patron de keycodes
    int from[8];
    int to; // to TIENE que ser diferente de 0
};

struct input_event rap1; // Inicio de evento
struct input_event rap2; // Finalización de evento
struct input_event event; // Plantilla para enviar un evento KEY
struct input_event ev; // Evento leido de la entrada cruda del teclado

// Lista con todos los remaps definidos por el usuario
struct keyRemap remaps[256];

// De input se leen los eventos de teclado
FILE *input;

// Al archivo teclado se escriben los eventos
FILE *teclado;
