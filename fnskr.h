#include <linux/input.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
# define BLANK -1

int append(int array[],int element){
// agregar elemento a array en la primera posicion vacia
    for(unsigned short i=0;i<8;i++){
        if(array[i]==BLANK){
            array[i]=element;
            return 1;
        }
    }
    return 0;
}
int in(int array[], int element){
    // retorna true si element está en array
    for(unsigned short i=0;i<8;i++){
        if(array[0] == element) { 
            return 1;
        }
    }
    return 0;
}
int pop(int array[],int element){
// eliminar (cambiar por -1)
    for(unsigned short i=0;i<8;i++){
        if(array[i]==element){
            array[i]=BLANK;
            return 1;
        }
    }
    return 0;
}
int clear(int array[]){
// restablecer el array
    for(unsigned short i=0;i<8;i++){
        array[i]=BLANK;
    }
    return 1;
}
int printList(int array[]){
// imprimir lista
    for(unsigned short i=0;i<8;i++){
        printf("%d ",array[i]);
    }
    printf("\n");
    return 1;
}
int find(int array[],int pattern[]){
    // buscar si pattern es igual a array
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
            // si las listas son diferentes retorna 0
            return 0;
        }
    }
    return 1;
}
