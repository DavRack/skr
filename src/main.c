#include "config.h"
int main(){
    // ruta absoluta al teclado que se va a usar
    // este archivo debe tener permisos 666
    //const char * ruta = "/dev/input/event3";

    // Se asignan lo valores para el primer evento especial rap1
    rap1.type = 4;
    rap1.code = 4;
    rap1.value = 31;

    // Se asignan lo valores para el primer evento especial rap2
    rap2.type = 0;
    rap2.code = 0;
    rap2.value = 0;

    //se usa el comando intercept para obtener los eventos
    //generados por el teclado especificado
    input = popen("sudo intercept -g /dev/input/event3","r");

    // se asigna el teclado en la ruta especificada al teclado
    teclado = popen("sudo uinput -d /dev/input/event3","w");

    makeRemaps();
    makeScripts();
    makeLayers();

    while (fread(&rawEvent, sizeof(event), 1, input) == 1) {
        if(rawEvent.type == EV_KEY){

            // se inicializa el evento a realizar
            toDo.type = -1;
            toDo.index = -1;
            toDo.keyState = -1;

            if(rawEvent.value == TECLA_PRESIONADA){
                append(teclas,rawEvent.code);
                toDo = getAction(teclas,rawEvent,userRemaps);
                doAction(toDo,teclas,rawEvent);
            }
            else if(rawEvent.value == TECLA_SOLTADA){
                toDo = getAction(teclas,rawEvent,userRemaps);
                doAction(toDo,teclas,rawEvent);
                pop(teclas,rawEvent.code);
            }
            else if(rawEvent.value == TECLA_MANTENIDA){
                toDo = getAction(teclas,rawEvent,userRemaps);
                doAction(toDo,teclas,rawEvent);
            }
            
        }
    }
}
