// Es necesario instalar los programas intercept yay -S interception-tools
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
# define BLANK -1
# define TECLA_PRESIONADA 1
# define TECLA_SOLTADA 0
# define TECLA_MANTENIDA 2
# define PATH_MAX 256
// agregar elemento a array en la primera posicion bacia
int append(int array[],int element){
    for(unsigned short i=0;i<8;i++){
        if(array[i]==BLANK){
            array[i]=element;
            return 1;
        }
    }
    return 0;
}
// eliminar (cambiar por -1)
int pop(int array[],int element){
    for(unsigned short i=0;i<8;i++){
        if(array[i]==element){
            array[i]=BLANK;
            return 1;
        }
    }
    return 0;
}
// restablecer el array
int clear(int array[]){
    for(unsigned short i=0;i<8;i++){
        array[i]=BLANK;
    }
    return 1;
}
// imprimir lista
int printList(int array[]){
    for(unsigned short i=0;i<8;i++){
        if(array[i] != BLANK){
            printf("%d ",array[i]);
        }
    }
    printf("\n");
    return 1;
}
// buscar si pattern es igual a array
int find(int array[],int pattern[]){
    // lista de keycodes consecutivos sin BLANK ente kecodes
    int sublist[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};

    for(unsigned short i=0;i<8;i++){
        if(array[i] != BLANK){
            append(sublist,array[i]);
        }
    }
    //verificar si ambas listas son iguales
    for(unsigned short i=0;i<8;i++){
        if(pattern[i] != sublist[i]){
            return 0;
        }
    }
    return 1;
}
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

            if(event.value == TECLA_PRESIONADA){
                printf("presionaste la tecla %d\n",event.code);
                if(lastKey != event.code){append(teclas,event.code);}
                printList(teclas);
                lastKey=event.code;
                if(find(teclas,pattern)){
                fp = popen("sudo uinput -d /dev/input/event26", "w");
                fprintf(fp,&event);
                status = pclose(fp);
                }


            }else if(event.value == TECLA_SOLTADA){
                printf("soltaste la tecla %d\n",event.code);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                    fp = popen("sudo uinput -d /dev/input/event26", "w");
                    fprintf(fp,&event);
                    status = pclose(fp);

                    pop(teclas,event.code);
                    exit(0);
                    lastKey=BLANK;
                }
                pop(teclas,event.code);
                lastKey=BLANK;


            }else if(event.value == TECLA_MANTENIDA){
                //printf("dejaste la tecla %d presionada\n",event.code);
                printf("teclas presionadas: \n");
                printList(teclas);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                    fp = popen("sudo uinput -d /dev/input/event26", "w");
                    fprintf(fp,&event);
                    status = pclose(fp);
                }
            }
        }
        //fwrite(&event, sizeof(event), 1, stdout);
    }
}
