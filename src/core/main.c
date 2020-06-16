#include "parser.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <wordexp.h>
void help(){
    printf("skr help");
}
void initSkr(){
    // test if the config file exist

    wordexp_t exp_result;
    wordexp("~/.config/skr/skr.config", &exp_result, 0);

    char *configPath = exp_result.we_wordv[0]; 

    if(access(exp_result.we_wordv[0],F_OK) != -1){
        printf("Config file exist\n");
        exit(0);
    }
    else{
        printf("Generating config file at ~/.config/skr/skr.config \n");
        printf("press a key to identify your keyboard\n");
        
        // test if skr was run as root
        if(getuid() == 0){
            printf("\"skr --init\" must not be run as root\n");
            exit(1);
        }
        
        if(system("mkdir -p ~/.config/skr") == -1){
            printf("can't create ~/.config/skr");
            exit(1);
        }

        if(system("touch ~/.config/skr/skr.config") == -1){
            printf("can't create ~/.config/skr/skr.config");
            exit(1);
        }

        system("sudo echo ");
        FILE * commandOut = popen("sudo /usr/share/skr/skrkid","r");
        char *userKb = NULL;
        char *line = NULL;
        size_t len = 0;

        if(getline(&line, &len, commandOut) != -1) {
            fscanf(commandOut,"%s",userKb);
        }

        FILE * configFile = fopen(exp_result.we_wordv[0],"w+");

        printf("\nYour keyboard path is: %s\n",line);

        fprintf(configFile,"# Auto gen SKR config file\n");
        fprintf(configFile,"# The keyboard that skr will intercept\n");
        fprintf(configFile,"KeyboardPath -> %s\n",line);
        fprintf(configFile," \n");
        fprintf(configFile,"# Get a tutorial, config examples and source code at: \n");
        fprintf(configFile,"#   https://github.com/davrack/skr\n");

        fclose(configFile);
        fclose(commandOut);
        free(userKb);
        exit(0);
    }
}
int main(int argc, char *argv[]){
    // if the user has provided some arguments
    if (argc > 1){
        if(strcmp(argv[1],"--init") == 0){
            initSkr();
        }
        else{
            help();
            exit(0);
        }
    }

    //se usa el comando intercept para obtener los eventos
    //generados por el teclado
    FILE *configFile = popen("python3 /usr/share/skr/parser/parser.py","r");
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
