#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define BLANK -1
# define TECLA_PRESIONADA 1
# define TECLA_SOLTADA 0
# define TECLA_MANTENIDA 2
# define PATH_MAX 256


int sendEvent(struct input_event event){
    FILE *fp = popen("sudo uinput -d /dev/input/event3","w");
    //recibe un evento y lo envia por medio del teclado solicitado
    //teclado por defecto es event3 
    //ecribe stdin con el contenido de event a uinput
    fwrite(&event,1,sizeof(event),fp);
    pclose(fp);

    return 1;
}
int writeEvent(struct input_event event,FILE *target){
    fwrite(&event,1,sizeof(event),target);
    return 1;
}
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
