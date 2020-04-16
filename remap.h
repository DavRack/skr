#include "fnskr.h"
struct keyRemap {
    int from[8];
    // to TIENE que ser diferente de 0
    int to;
};

struct keyRemap remaps[256];

int getFreeRemaps(){
    // retorna el indice de la primera posición vacia en remaps
    for(int i = 0; i < 256; i++){
        if(remaps[i].to == 0){
            return i;
        }
    }
    return -1;
}

int mkKeyRemap(int from[], int to){
    int pInRemaps = getFreeRemaps();

    for(unsigned int i=0;i<8;i++){
        remaps[pInRemaps].from[i]=from[i];
    }

    remaps[pInRemaps].to=to;

    return 1;

}
int getMatchIndex(int teclas[]){
    //retorna el indice en remaps[] del evento que contenga
    //el patron teclas, si no se encuentra retorna -1
    for(int i=0;i<256;i++){
        if(remaps[i].to != 0){
            if(find(teclas,remaps[i].from) == 1){
                return i;
            }
        }
        else{
            return -1;
        }
    }
    return -1;
}
int makeRemaps(){
    int f[8];

    f[0] = 58; // remapear CapsLock
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,1); // a ESC

    f[0] = 102;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,2);

    f[0] = 104;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,4);

    f[0] = 109;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,9);

    f[0] = 107;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,7);

    return 1;
}
