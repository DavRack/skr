#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>

int append(int array[],int element){
    for(unsigned short i=0;i<8;i++){
        if(array[i]==-1){
            array[i]=element;
            return 1;
        }
    }
    return 0;
}
int pop(int array[],int element){
    for(unsigned short i=0;i<8;i++){
        if(array[i]==element){
            array[i]=-1;
            return 1;
        }
    }
    return 0;
}
int clear(int array[]){
    for(unsigned short i=0;i<8;i++){
        array[i]=-1;
    }
    return 1;
}
int printList(int array[]){
    for(unsigned short i=0;i<8;i++){
        if(array[i] != -1){
            printf("%d ",array[i]);
        }
    }
    printf("\n");
    return 1;
}
int find(int array[],int pattern[]){
    int sublist[8]={-1,-1,-1,-1,-1,-1,-1,-1};
    for(unsigned short i=0;i<8;i++){
        if(array[i] !=-1){
            append(sublist,array[i]);
        }
    }
    for(unsigned short i=0;i<8;i++){
        if(pattern[i] != sublist[i]){
            return 0;
        }
    }
    return 1;
}
int main(void) {
    struct input_event event;
    int teclas[8]={-1,-1,-1,-1,-1,-1,-1,-1};
    int pattern[8]={29,KEY_C,-1,-1,-1,-1,-1,-1};
    int lastKey=-1;

    setbuf(stdin, NULL), setbuf(stdout, NULL);

    while (fread(&event, sizeof(event), 1, stdin) == 1) {
        if (event.type == EV_KEY){

            if(event.value == 1){
                printf("presionaste la tecla %d\n",event.code);
                if(lastKey != event.code){append(teclas,event.code);}
                printList(teclas);
                lastKey=event.code;


            }else if(event.value == 0){
                printf("soltaste la tecla %d\n",event.code);
                if(find(teclas,pattern)){
                    printf("Copiar!\n");
                    pop(teclas,event.code);
                    lastKey=-1;
                    system("blender");
                }
                pop(teclas,event.code);
                lastKey=-1;


            }else if(event.value == 2){
                //printf("dejaste la tecla %d presionada\n",event.code);
                printf("teclas presionadas: \n");
                printList(teclas);
            }
        }
        //fwrite(&event, sizeof(event), 1, stdout);
    }
}
