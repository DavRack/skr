#define _DEFAULT_SOURCE
#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#define BLANK -1

#define TRUE 1
#define FALSE 0

#define EV_SIZE 24

#define TECLA_MANTENIDA 2
#define TECLA_PRESIONADA 1
#define TECLA_SOLTADA 0

#define TYPE_NORMAL   0
#define TYPE_KEYREMAP 1
#define TYPE_SCRIPT   2

#define NUMBER_OF_REMAPS 256
#define NUMBER_OF_MACROS 128
#define NUMBER_OF_LAYERS 64

#define MACRO_LENGTH 64
#define MIN_SLEEP_TIME 10000
typedef struct action{
    int actionUsed;

    unsigned int sleepSeconds;
    unsigned int sleepMicroSeconds;

    int type;

    int key;
    int keyState;
    char *script;
}action;

// Define la estructura de un remapeo de una tecla o combinación de teclas
typedef struct remap{
    int remapUsed;
    int hotKey;
    action actions[MACRO_LENGTH];
}remap;

remap blankRemap;

// Indice del keyRemap recien eviado 
remap remapEnviado;

typedef struct fnLayer{
    int used;
    int fnKey;
    remap fnRemaps[NUMBER_OF_REMAPS];
}fnLayer;

// Lista con todas las capas de funcion definidas por el usuario
fnLayer layers[NUMBER_OF_LAYERS];

fnLayer layerActivada;

fnLayer blankLayer;

fnLayer nonFreeLayer;

struct input_event rap1 = {{0},4,4,4}; // Inicio de evento
struct input_event rap2 = {{0},0,0,0}; // Finalización de evento
struct input_event event; // Plantilla para enviar un evento KEY
struct input_event rawEvent; // Evento leido de la entrada cruda del teclado

// Indice del script recien eviado 
int scriptEnviado;

// teclas guarda la o las teclas presionadas en el momento
int teclas[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};

FILE *input;
FILE *teclado;
