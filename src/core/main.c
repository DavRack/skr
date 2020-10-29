#include "parser.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[]){
    // if the user has provided some arguments
    if (argc == 3){
        if(strcmp(argv[1],"-c") == 0){
            printf("Executing skr with %s as config file",argv[2]);
        }else{
            printf("Wrong arguments\n");
            exit(0);
        }
    }else{
        printf("Wrong number of arguments\n");
        exit(0);
    }

    char pythonCommand[256];
    snprintf(pythonCommand,sizeof pythonCommand,
            "python3 /usr/share/skr/parser/parser.py %s",argv[2]);

    // using intercept comand to get keyboard events
    FILE *configFile = popen(pythonCommand,"r");
    parseConfigFromFile(configFile);

    // generate shell commands to interact with interception-tools
    char uinput[256];
    char intercept[256];
    snprintf(uinput,sizeof uinput,"sudo uinput -d %s",userKeyboard);
    snprintf(intercept,sizeof intercept,"sudo intercept -g %s",userKeyboard);

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
