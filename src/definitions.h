#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
#define TYPE_MACRO    3

#define NUMBER_OF_REMAPS 512
#define NUMBER_OF_LAYERS 64

// Define la estructura de un remapeo de una tecla o combinación de teclas
typedef struct remap{
    int remapUsed;
    int type;
    int onKeyState;
    int hotKey;
    int keyRemap;
    int from[8];
    char *script;
}remap;

// Lista con todos los remaps definidos por el usuario
remap userRemaps[NUMBER_OF_REMAPS];

remap auxRemap;

remap blankRemap;

// Indice del keyRemap recien eviado 
remap remapEnviado;

typedef struct fnLayer {
    int fnKey;
    remap fnRemaps[NUMBER_OF_REMAPS];
}fnLayer;

// Lista con todas las capas de funcion definidas por el usuario
fnLayer  layers[NUMBER_OF_LAYERS];

fnLayer layerActivada;

fnLayer blankLayer;

struct input_event rap1 = {{0},4,4,4}; // Inicio de evento
struct input_event rap2 = {{0},0,0,0}; // Finalización de evento
struct input_event event; // Plantilla para enviar un evento KEY
struct input_event rawEvent; // Evento leido de la entrada cruda del teclado

// De input se leen los eventos de teclado
FILE *input;

// Al archivo teclado se escriben los eventos
FILE *teclado;

// Indice del script recien eviado 
int scriptEnviado;

// teclas guarda la o las teclas presionadas en el momento
int teclas[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};
