// Es necesario instalar los programas intercept yay -S interception-tools
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include "fnskr.h"
int main(void) {

    struct input_event event;
    struct input_event event2;
    struct input_event event3;

    event2.value = 1;



    event3.value = 1;
    int teclas[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};
    int pattern[8]={KEY_C,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};
    int lastKey=BLANK;
    FILE *pFile2 = fopen("/dev/input/event3", "w+");
    char *test;
    FILE *fp;
    int status;
    char path[PATH_MAX];
                    
                    
    

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY){
            printf("%ld",event.time.tv_usec);

            if(event.value == TECLA_PRESIONADA){
                printf("presionaste la tecla %d\n",event.code);
                if(lastKey != event.code){append(teclas,event.code);}
                printList(teclas);
                lastKey=event.code;


            }else if(event.value == TECLA_SOLTADA){
                printf("soltaste la tecla %d\n",event.code);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                    //fp = popen("sudo uinput -d /dev/input/event26", "w");
                    //fprintf(fp,&event);
                    //status = pclose(fp);

                    pop(teclas,event.code);
                    lastKey=BLANK;
                    return 1;
                }
                pop(teclas,event.code);
                lastKey=BLANK;


            }else if(event.value == TECLA_MANTENIDA){
                //printf("dejaste la tecla %d presionada\n",event.code);
                printf("teclas presionadas: \n");
                printList(teclas);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                }
            }
        }
        //fwrite(&event, sizeof(event), 1, stdout);
    }
}
