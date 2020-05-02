#include <stdio.h>
//#include "../../src/config.h"
#include "../../src/fnskr.h"

# define True 1
# define False 0

int nElementos(int list[]){
    //int l = sizeof(*list)/sizeof(int);
    return 256;
}
int testPasados(int tp[]){
    int l = nElementos(tp);
    int pasados = 0;

    for(int i = 0; i < l; i++){
        if(tp[i] == 1){
            pasados++;
        }
    }
    return pasados;
}
int testReprobados(int tp[]){
    int l = nElementos(tp);
    int reprobados = 0;

    for(int i = 0; i < l; i++){
        if(tp[i] == 2){
            reprobados++;
        }
    }
    return reprobados;
}
int totalTest(int tp[]){
    return testReprobados(tp)+testPasados(tp);
}
float porcentageAprobados(int tp[]){
    double total = totalTest(tp);
    double p = testPasados(tp);
    if(total>0){
        return 100*(p/total);
    }else{return -1;}
}
int analisis(int tp[],char *nombreTest){
    printf("#### %s ####\n\n",nombreTest);
    printf("Total de test corridos: %d\n",totalTest(tp));
    if(testReprobados(tp) > 0){
    printf("Test pasados: %d\n",testPasados(tp));
    printf("Test reprobados: %d\n",testReprobados(tp));
    printf("Porcentage de test aprobados: %f\n\n",porcentageAprobados(tp));

        for (int i = 0; i < nElementos(tp);i++){
            if (tp[i] == 2){
                printf("Indice test reprobado: %d\n",i);
            }
        }
    }
    printf("\n");
    return testReprobados(tp);
}
void init(int tp[]){
    for(int i=0;i<256;i++){
        tp[i] = 0;
    }
}
int fac(int n){
    if(n<0){return -1;}
    int resultado = 1;
    for(int i = 1; i <= n; i++){
        resultado *= i;
    }
    return resultado;
}
int getFreeIndex(int list[]){
    int l = nElementos(list);

    for(int i = 0; i < l; i++){
        if(list[i] == 0){
            return i;
        }
    }
    return -1;
}
void apnd(int resultado,int list[]){
    int l = getFreeIndex(list);
    if(resultado == True){list[l] = 1;}
    else{list[l] = 2;}
}
int * arr(int arr[],int e1,int e2,int e3,int e4,int e5,int e6,int e7,int e8){
    arr[0] = e1;
    arr[1] = e2;
    arr[2] = e3;
    arr[3] = e4;
    arr[4] = e5;
    arr[5] = e6;
    arr[6] = e7;
    arr[7] = e8;
    return arr;
}
int arrEq(int arr1[], int arr2[]){
    for(int i = 0; i <8;i ++){
        if(arr1[i] != arr2[i]){return False;}
    }
    return True;
}
void fillRemapsUpTo(int index){
    int from[8] = {11,22,33,44,55,66,77,88};
    for(int i = 0; i <index;i ++){
       arCpy(userRemaps[i].from,from);
    }
}
void clearLayer(){
    for(int i = 0; i < NUMBER_OF_LAYERS;i++){
       layers[i].fnKey = 0;
    }
}
void fillLayerUpTo(int index){
    for(int i = 0; i < index;i ++){
       layers[i].fnKey = 58;
    }
}
/*
void fillScriptsUpTo(int index){
    for(int i = 0; i <index;i ++){
       scripts[i].to = "vacio";
       scripts[i].from[0] = 29;
       scripts[i].from[1] = 30;
       scripts[i].from[2] = -1;
       scripts[i].from[3] = -1;
       scripts[i].from[4] = -1;
       scripts[i].from[5] = -1;
       scripts[i].from[6] = -1;
       scripts[i].from[7] = -1;
    }
}
void clearScipts(){
    for(int i = 0; i < 256;i ++){
       scripts[i].to = 0;
    }
}
int makeTestRemaps(){
    // Definir todos los keymaps dentro de esta funcion

    mkKeyRemap(15,1); // a ESC

    mkKeyRemap(102,2);

    mkKeyRemap(104,4);

    mkKeyRemap(109,9);

    return TRUE;
}

int makeTestScripts(){
    int f[8];

    f[0] = 29; 
    f[1] = 107;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkScriptLaunch(f,"alacritty&",TECLA_SOLTADA);

    return TRUE;
}

int makeTestLayers(){
    setNewLayer(58);
    mkLayerKeyRemap(35,105);
    mkLayerKeyRemap(36,108);
    mkLayerKeyRemap(37,103);
    mkLayerKeyRemap(38,106);
    mkLayerKeyRemap(57,15);
    return TRUE;
}
*/
