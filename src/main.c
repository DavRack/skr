#include "config.h"
int main(){
    //se usa el comando intercept para obtener los eventos
    //generados por el teclado
    input = popen("sudo intercept -g /dev/input/event3","r");
    teclado = popen("sudo uinput -d /dev/input/event3","w");

    makeRemaps();

    while (1){
        fread(&rawEvent, sizeof(event), 1, input);
        if(rawEvent.type == EV_KEY){

            if(rawEvent.value == TECLA_PRESIONADA){
                append(teclas,rawEvent.code);
                doAction(teclas,rawEvent);
            }
            else if(rawEvent.value == TECLA_SOLTADA){
                doAction(teclas,rawEvent);
                pop(teclas,rawEvent.code);
            }
            else if(rawEvent.value == TECLA_MANTENIDA)
                doAction(teclas,rawEvent);
        }
    }
}
