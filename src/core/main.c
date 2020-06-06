#include "parser.h"

int main(){
    //se usa el comando intercept para obtener los eventos
    //generados por el teclado
    FILE *configFile = popen("python3 src/parser/parser.py","r");
    parseConfigFromFile(configFile);

    // generate shell commands to interact with interception-tools
    char *uinput = cat("sudo uinput -d ",userKeyboard);
    char *intercept = cat("sudo intercept -g ",userKeyboard);

    input = popen(intercept,"r");
    teclado = popen(uinput,"w");

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
