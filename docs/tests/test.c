#include "mtf.h"

// TS -> test suite
// T -> test individual

int T_eql(int arr1[],int arr2[]){
    return arrEq(arr1,arr2) == eql(arr1,arr2);
}
int TS_eql(){
    int tp[256];
    init(tp);

    int arr1[8];
    int arr2[8];

    arr(arr1,-1,-1,-1,-1,-1,-1,-1,-1);
    arr(arr2,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,99,-1,-1,-1,-1,-1,-1,-1);
    arr(arr2,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,99,-1,-1,-1,55,-1,-1,-1);
    arr(arr2,88,-1,-1,-1,55,-1,-1,99);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,99,88,77,66,55,44,33,22);
    arr(arr2,88,-1,-1,-1,55,-1,-1,99);
    apnd(T_eql(arr1,arr2),tp);

    return analisis(tp,"eql");
}
int T_getIndexInArray(int arr[],int element,int index){
    return index == getIndexInArray(element,arr);
}
int TS_getIndexInArray(){
    int tp[256];
    init(tp);

    int ar[8];
    int element = 0;
    int index = 0;

    arr(ar,-1,-1,-1,-1,-1,-1,-1,-1);
    element = 99;
    index = -1;
    apnd(T_getIndexInArray(ar,element,index),tp);

    arr(ar,99,-1,-1,-1,-1,-1,-1,-1);
    element = 99;
    index = 0;
    apnd(T_getIndexInArray(ar,element,index),tp);

    arr(ar,-1,-1,-1,-1,-1,-1,-1,99);
    element = 99;
    index = 7;
    apnd(T_getIndexInArray(ar,element,index),tp);

    arr(ar,99,-1,-1,-1,-1,-1,-1,99);
    element = 99;
    index = 0;
    apnd(T_getIndexInArray(ar,element,index),tp);

    arr(ar,99,58,88,44,-1,-1,-1,99);
    element = 44;
    index = 3;
    apnd(T_getIndexInArray(ar,element,index),tp);

    return analisis(tp,"getIndexInArray");
}
int T_append(int array[], int element,int output[]){
    return arrEq(append(array,element),output);
}
int TS_append(){
    int tp[256];
    init(tp);

    // array input
    int i[8];
    // array output
    int o[8];
    apnd(T_append(arr(i,-1,-1,-1,-1,-1,-1,-1,-1),1,arr(o,1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_append(arr(i,-1,-1,-1,-1,-1,-1,-1,-1),5,arr(o,5,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_append(arr(i,151,-1,-1,-1,-1,-1,-1,-1),5,arr(o,151,5,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_append(arr(i,1,1,-1,-1,-1,-1,-1,-1),5,arr(o,1,1,5,-1,-1,-1,-1,-1)),tp);
    apnd(T_append(arr(i,51,71,-1,25,-1,-1,-1,-1),5,arr(o,51,71,5,25,-1,-1,-1,-1)),tp);
    apnd(T_append(arr(i,51,71,-1,25,-1,55,-1,-1),5,arr(o,51,71,5,25,-1,55,-1,-1)),tp);
    apnd(T_append(arr(i,1,1,1,1,1,1,1,1),5,arr(o,1,1,1,1,1,1,1,1)),tp);
    apnd(T_append(arr(i,1,1,-1,-1,-1,1,1,1),5,arr(o,1,1,5,-1,-1,1,1,1)),tp);

    return analisis(tp,"append");
}
int T_in(int array[], int element, int output){
    if(in(array,element) == output){
        return True;
    }else{return False;}
}
int TS_in(){
    int tp[256];
    init(tp);

    int i[8];

    apnd(T_in(arr(i,5,-1,-1,-1,-1,-1,-1,-1),5,True),tp);
    apnd(T_in(arr(i,5,-1,-1,-1,-1,-1,-1,-1),9,False),tp);
    apnd(T_in(arr(i,29,10,39,49,5,-1,-1,-1),9,False),tp);
    apnd(T_in(arr(i,29,10,39,49,5,54,58,-1),9,False),tp);
    apnd(T_in(arr(i,29,10,39,49,5,54,58,-1),58,True),tp);
    apnd(T_in(arr(i,29,10,39,49,5,54,58,87),87,True),tp);
    apnd(T_in(arr(i,29,-1,-1,-1,-1,54,58,87),87,True),tp);
    return analisis(tp,"in");
}
int T_pop(int array[],int element, int output[]){
    return arrEq(pop(array,element),output);
}
int TS_pop(){
    int tp[256];
    init(tp);

    int i[8];
    int o[8];

    apnd(T_pop(arr(i,-1,-1,-1,-1,-1,-1,-1,-1),5,arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_pop(arr(i,5,-1,-1,-1,-1,-1,-1,-1),5,arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_pop(arr(i,5,-1,-1,-1,-1,5,-1,-1),5,arr(o,-1,-1,-1,-1,-1,5,-1,-1)),tp);
    apnd(T_pop(arr(i,29,30,39,-1,-1,-1,-1,-1),5,arr(o,29,30,39,-1,-1,-1,-1,-1)),tp);
    apnd(T_pop(arr(i,29,30,39,40,41,42,43,44),5,arr(o,29,30,39,40,41,42,43,44)),tp);
    apnd(T_pop(arr(i,29,30,39,40,5,42,43,44),5,arr(o,29,30,39,40,-1,42,43,44)),tp);

    return analisis(tp,"pop");
}

int T_clear(int i[],int o[]){
    return arrEq(clear(i),o);
}
int TS_clear(){
    int tp[256];
    init(tp);

    int i[8];
    int o[8];

    apnd(T_clear(arr(i,-1,-1,-1,-1,-1,-1,-1,-1),arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_clear(arr(i,10,-1,-1,-1,-1,-1,-1,-1),arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_clear(arr(i,10,5,-1,29,-1,-1,6,-1),arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);
    apnd(T_clear(arr(i,10,5,-1,29,-1,-1,6,-1),arr(o,-1,-1,-1,-1,-1,-1,-1,-1)),tp);

    return analisis(tp,"clear");
}

int T_popFirst(int input[],int output[]){
    return eql(popFirst(input),output);
}

int TS_popFirst(){
    int tp[256];
    init(tp);

    int i[8];
    int o[8];

    arr(i,-1,-1,-1,-1,-1,-1,-1,-1);
    arr(o,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_popFirst(i,o),tp);

    arr(i,88,-1,-1,-1,-1,-1,-1,-1);
    arr(o,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_popFirst(i,o),tp);

    arr(i,88,99,-1,-1,-1,-1,-1,-1);
    arr(o,99,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_popFirst(i,o),tp);

    arr(i,88,99,-1,-1,22,-1,-1,-1);
    arr(o,99,22,-1,-1,-1,-1,-1,-1);
    apnd(T_popFirst(i,o),tp);

    arr(i,11,22,33,44,55,66,77,88);
    arr(o,22,33,44,55,66,77,88,-1);
    apnd(T_popFirst(i,o),tp);

    return analisis(tp,"popFirst");
}
int T_removeSpaces(int o[], int d[]){
    removeSpaces(o);
    return eql(o,d);
}
int TS_removeSpaces(){
    int tp[256];
    init(tp);

    int i[8];
    int d[8];
    
    arr(i,-1,-1,-1,-1,-1,-1,-1,-1);
    arr(d,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_removeSpaces(i,d),tp);
    
    arr(i,55,-1,66,77,-1,-1,-1,-1);
    arr(d,55,66,77,-1,-1,-1,-1,-1);
    apnd(T_removeSpaces(i,d),tp);
    
    arr(i,55,-1,66,77,88,99,11,22);
    arr(d,55,66,77,88,99,11,22,-1);
    apnd(T_removeSpaces(i,d),tp);
    
    arr(i,55,33,66,77,88,99,11,22);
    arr(d,55,33,66,77,88,99,11,22);
    apnd(T_removeSpaces(i,d),tp);

    return analisis(tp,"removeSpaces");
}

int T_find(int array[],int pattern[],int output){
    return (find(array,pattern) == output);
}
int TS_find(){
    int tp[256];
    init(tp);

    int array[8];
    int pattern[8];

    arr(array,-1,-1,-1,-1,-1,-1,-1,-1);
    arr(pattern,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,True),tp);

    arr(array,55,-1,-1,-1,-1,-1,-1,-1);
    arr(pattern,55,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,True),tp);

    arr(array,29,44,-1,-1,-1,-1,-1,-1);
    arr(pattern,29,44,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,True),tp);

    arr(array,29,-1,-1,44,-1,-1,-1,-1);
    arr(pattern,29,44,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,True),tp);

    arr(array,29,-1,-1,-1,-1,44,-1,-1);
    arr(pattern,29,44,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,True),tp);

    arr(array,29,3,-1,44,-1,-1,-1,-1);
    arr(pattern,29,44,-1,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,False),tp);

    arr(array,29,3,-1,44,-1,-1,-1,-1);
    arr(pattern,29,44,3,-1,-1,-1,-1,-1);
    apnd(T_find(array,pattern,False),tp);

    arr(array,11,22,33,44,55,66,77,88);
    arr(pattern,11,22,33,44,55,66,77,88);
    apnd(T_find(array,pattern,True),tp);

    return analisis(tp,"find");
}

int T_getFreeRemaps(int index,remap remaps[]){
    return (getFreeRemaps(remaps) == index);
}
int TS_getFreeRemaps(){
    int tp[256];
    init(tp);

    initRemaps(userRemaps);
    apnd(T_getFreeRemaps(0,userRemaps),tp);

    initRemaps(userRemaps);
    fillRemapsUpTo(userRemaps,1);
    apnd(T_getFreeRemaps(1,userRemaps),tp);

    initRemaps(userRemaps);
    fillRemapsUpTo(userRemaps,10);
    apnd(T_getFreeRemaps(10,userRemaps),tp);

    initRemaps(userRemaps);
    fillRemapsUpTo(userRemaps,255);
    apnd(T_getFreeRemaps(255,userRemaps),tp);

    initRemaps(userRemaps);
    fillRemapsUpTo(userRemaps,511);
    apnd(T_getFreeRemaps(511,userRemaps),tp);

    initRemaps(userRemaps);
    fillRemapsUpTo(userRemaps,512);
    apnd(T_getFreeRemaps(-1,userRemaps),tp);

    return analisis(tp,"getFreeRemap");
}
int T_getFreeLayer(int index){
    return (getFreeLayer() == index);
}
int TS_getFreeLayer(){
    int tp[256];
    init(tp);

    clearLayer();
    apnd(T_getFreeLayer(0),tp);

    clearLayer();
    fillLayerUpTo(1);
    apnd(T_getFreeLayer(1),tp);

    clearLayer();
    fillLayerUpTo(31);
    apnd(T_getFreeLayer(31),tp);

    clearLayer();
    fillLayerUpTo(64);
    apnd(T_getFreeLayer(-1),tp);

    return analisis(tp,"getFreeLayer");
}
/*
int T_getRemapsMatch(int index){
    remaps[index].from = 99;
    remaps[index].to = 55;

    int r = getRemapsIndex(99);

    if(index != r)
        return False;
    else
        return True;
}
int TS_getRemapsMatch(){
    int tp[256];
    init(tp);

    // array vacio
    fillRemapsUpTo(-1);
    apnd(T_getRemapsIndex(-1),tp);

    clearRemaps();
    fillRemapsUpTo(0);
    apnd(T_getRemapsIndex(0),tp);

    clearRemaps();
    fillRemapsUpTo(1);
    apnd(T_getRemapsIndex(1),tp);

    clearRemaps();
    fillRemapsUpTo(255);
    apnd(T_getRemapsIndex(255),tp);

    clearRemaps();
    fillRemapsUpTo(256);
    apnd(T_getRemapsIndex(-1),tp);

    return analisis(tp,"getRemapsIndex");
}
int T_setNewLayer(){
    int fnKey = 100;
    fnLayer old[NUMBER_OF_LAYERS];
    memcpy(old,layers, sizeof(layers));
    int change = getFreeLayer();
    setNewLayer(fnKey);
    for(int i = 0; i<NUMBER_OF_LAYERS;i++){
        if(old[i].fnKey != layers[i].fnKey){
            if(i != change || layers[i].fnKey != fnKey){
                return False;
            }
        }
    }
    return True;
}
int TS_setNewLayer(){
    int tp[256];
    init(tp);

    clearLayer();
    apnd(T_setNewLayer(),tp);

    clearLayer();
    fillLayerUpTo(1);
    apnd(T_setNewLayer(),tp);

    clearLayer();
    fillLayerUpTo(32);
    apnd(T_setNewLayer(),tp);

    clearLayer();
    fillLayerUpTo(64);
    apnd(T_setNewLayer(),tp);

    return analisis(tp,"setNewLayer");
}
int T_getFreeScripts(int index){
    return (getFreeScripts() == index);
}
int TS_getFreeScripts(){
    int tp[256];
    init(tp);

    clearScipts();
    apnd(T_getFreeScripts(0),tp);

    clearScipts();
    fillScriptsUpTo(1);
    apnd(T_getFreeScripts(1),tp);

    clearScipts();
    fillScriptsUpTo(255);
    apnd(T_getFreeScripts(255),tp);

    clearScipts();
    fillScriptsUpTo(256);
    apnd(T_getFreeScripts(-1),tp);

    return analisis(tp,"getFreeScripts");
}
int T_mkKeyRemap(){
    int from = 88;
    int to = 99;
    struct keyRemap rem[256];
    memcpy(rem,remaps,sizeof(remaps));
    int change = getFreeRemaps();
    int r =mkKeyRemap(from,to); 
    if( r != change){
        return False;
    }
    for(int i = 0; i < 256; i++){
        if(rem[i].from != remaps[i].from || rem[i].to != remaps[i].to){
            if(i != change){
                return False;
            }
        }
    }
    return True;
}
int TS_mkKeyRemap(){
    int tp[256];
    init(tp);

    clearRemaps();
    apnd(T_mkKeyRemap(),tp);

    clearRemaps();
    fillRemapsUpTo(1);
    apnd(T_mkKeyRemap(),tp);

    clearRemaps();
    fillRemapsUpTo(255);
    apnd(T_mkKeyRemap(),tp);

    clearRemaps();
    fillRemapsUpTo(256);
    apnd(T_mkKeyRemap(),tp);

    return analisis(tp,"mkKeyRemap");
}
int T_mkScriptLaunch(){
    int from[8] = {100,-1,-1,-1,-1,-1,-1,-1};
    char *to = "test";
    int onAction = 1;

    int change = getFreeScripts();

    mkScriptLaunch(from,to,onAction);

    if(change >= 0){
        if(*scripts[change].from != *from){
            return False;
        }
        if(*scripts[change].to != *to){
            return False;
        }
        if(scripts[change].onAction != onAction){
            return False;
        }
    }
    return True;
}
int TS_mkScriptLaunch(){
    int tp[256];
    init(tp);

    clearScipts();
    apnd(T_mkScriptLaunch(),tp);

    clearScipts();
    fillScriptsUpTo(1);
    apnd(T_mkScriptLaunch(),tp);

    clearScipts();
    fillScriptsUpTo(255);
    apnd(T_mkScriptLaunch(),tp);

    clearScipts();
    fillScriptsUpTo(256);
    apnd(T_mkScriptLaunch(),tp);

    return analisis(tp,"mkScriptLaunch");
}
int T_getLastLayer(int output){
    int r = getLastLayer();
    return r == output;
}
int TS_getLastLayer(){
    int tp[256];
    init(tp);

    clearLayer();
    apnd(T_getLastLayer(-1),tp);

    clearLayer();
    fillLayerUpTo(1);
    apnd(T_getLastLayer(0),tp);

    clearLayer();
    fillLayerUpTo(32);
    apnd(T_getLastLayer(31),tp);

    return analisis(tp,"getLastLayer");
}
int T_mkLayerKeyRemap(){
    int old = getLastLayer();
    int r = mkLayerKeyRemap(58,15);

    if(old != r){
        return False;
    }
    int index = 0;
    for (int i = 0;i<256; i++){
        if(layers[r].remaps[i].to == 0){
            index = i-1;
            break;
        }
    }

    if(layers[r].remaps[index].from != 58){return False;}
    if(layers[r].remaps[index].to != 15){return False;}

    return True;
}
int TS_mkLayerKeyRemap(){
    int tp[256];
    init(tp);

    clearLayer();
    apnd(T_mkLayerKeyRemap(),tp);

    clearLayer();
    fillLayerUpTo(1);
    apnd(T_mkLayerKeyRemap(),tp);

    clearLayer();
    fillLayerUpTo(31);
    apnd(T_mkLayerKeyRemap(),tp);

    clearLayer();
    fillLayerUpTo(32);
    apnd(T_mkLayerKeyRemap(),tp);

    return analisis(tp,"mkLayerKeyRemap");
}
int T_getScriptsIndex(int input, int index){

    if(input > -1){
        scripts[input].from[0] = 99;
        scripts[input].from[1] = 88;
        scripts[input].from[2] = -1;
        scripts[input].from[3] = -1;
        scripts[input].from[4] = -1;
        scripts[input].from[5] = -1;
        scripts[input].from[6] = -1;
        scripts[input].from[7] = -1;
        scripts[input].to = "test";
    }

    int i[8] = {99,88,-1,-1,-1,-1,-1,-1};
    int r = getScriptsIndex(i);

    return index == r;

}
int TS_getScriptsIndex(){
    int tp[256];
    init(tp);
    int input;

    clearRemaps();
    input = -1;
    fillScriptsUpTo(input);
    apnd(T_getScriptsIndex(input,-1),tp);

    clearRemaps();
    input = 0;
    fillScriptsUpTo(input);
    apnd(T_getScriptsIndex(input,0),tp);

    clearRemaps();
    input = 1;
    fillScriptsUpTo(input);
    apnd(T_getScriptsIndex(input,1),tp);

    clearRemaps();
    input = 255;
    fillScriptsUpTo(input);
    apnd(T_getScriptsIndex(input,255),tp);

    clearRemaps();
    input = 256;
    fillScriptsUpTo(input);
    apnd(T_getScriptsIndex(input,-1),tp);

    return analisis(tp,"getScriptsIndex");
}
int T_getLayerIndex(int pattern[], int index){
    int r = getLayerIndex(pattern);
    return r == index;
}
int TS_getLayerIndex(){
    int tp[256];
    init(tp);

    int teclas[8];
    int index;

    arr(teclas,-1,-1,-1,-1,-1,-1,-1,-1);
    clearLayer();
    index = 0;
    apnd(T_getLayerIndex(teclas,-1),tp);

    arr(teclas,58,-1,-1,-1,-1,-1,-1,-1);
    clearLayer();
    index = 10;
    fillLayerUpTo(index);
    apnd(T_getLayerIndex(teclas,-1),tp);

    arr(teclas,58,-1,-1,-1,-1,-1,-1,-1);
    clearLayer();
    index = 10;
    fillLayerUpTo(index);
    layers[10].fnKey = 58;
    apnd(T_getLayerIndex(teclas,10),tp);

    arr(teclas,-1,-1,-1,-1,-1,-1,-1,-1);
    clearLayer();
    index = 10;
    fillLayerUpTo(index);
    layers[10].fnKey = 58;
    apnd(T_getLayerIndex(teclas,-1),tp);

    arr(teclas,58,-1,-1,-1,-1,-1,-1,-1);
    clearLayer();
    index = 32;
    fillLayerUpTo(index);
    apnd(T_getLayerIndex(teclas,-1),tp);

    return analisis(tp,"getLayerIndex");
}
int T_getAction(int teclas[],int keyCode,int keyState,struct actionToDo output){
    struct actionToDo r = getAction(teclas,keyCode,keyState);

    if(r.type != output.type){return False;}
    if(r.index != output.index){return False;}
    if(r.keyState != output.keyState){return False;}
    return True;
}
int TS_getAction(){
    int tp[256];
    init(tp);
    struct actionToDo output;

    int teclas[8];
    int keyCode;
    int kS;

    makeTestRemaps();
    makeTestLayers();
    makeTestScripts();

    arr(teclas,30,-1,-1,-1,-1,-1,-1,-1);
    keyCode = 30;
    kS = 1;

    output.type = 0;
    output.index = 30;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    arr(teclas,15,-1,-1,-1,-1,-1,-1,-1);
    keyCode = 15;
    kS = 1;

    output.type = 1;
    output.index = 0;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    arr(teclas,29,107,-1,-1,-1,-1,-1,-1);
    keyCode = 107;
    kS = 1;

    output.type = 2;
    output.index = 0;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    arr(teclas,58,-1,-1,-1,-1,-1,-1,-1);
    keyCode = 58;
    kS = 1;

    output.type = 3;
    output.index = 0;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    arr(teclas,35,58,-1,-1,-1,-1,-1,-1);
    keyCode = 58;
    kS = 1;

    output.type = BLANK;
    output.index = BLANK;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    arr(teclas,58,109,-1,-1,-1,-1,-1,-1);
    keyCode = 109;
    kS = 1;

    output.type = 3;
    output.index = 0;
    output.keyState = kS;

    apnd(T_getAction(teclas,keyCode,kS,output),tp);

    return analisis(tp,"getAction");
}
int T_eql(int arr1[], int arr2[]){
    printf("%d\n",eql(arr1,arr2));
    return eql(arr1,arr2) == arrEq(arr1,arr2);
}
int TS_eql(){
    int tp[256];
    init(tp);

    int arr1[8];
    int arr2[8];

    arr(arr1,-1,-1,-1,-1,-1,-1,-1,-1);
    arr(arr2,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,2,-1,-1,-1,-1,-1,-1,-1);
    arr(arr2,-1,-1,-1,-1,-1,-1,-1,-1);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,99,-1,-1,-1,-1,-1,-1,-1);
    arr(arr2,99,99,99,-1,-1,-1,-1,-1);
    apnd(T_eql(arr1,arr2),tp);

    arr(arr1,99,99,99,88,27,888,36,33);
    arr(arr2,99,99,99,88,27,888,36,33);
    apnd(T_eql(arr1,arr2),tp);

    return analisis(tp,"eql");
}
*/
int main(){
int f = 0;

f+=TS_eql();
f+=TS_getIndexInArray();
f+=TS_append();
f+=TS_clear();
f+=TS_removeSpaces();
f+=TS_in();
f+=TS_pop();
f+=TS_popFirst();
f+=TS_find();

f+=TS_getFreeRemaps();
f+=TS_getFreeLayer();
//f+=TS_setNewLayer();

/*
f+=TS_getAction();
f+=TS_getFreeScripts();
f+=TS_getLastLayer();
f+=TS_getLayerIndex();
f+=TS_getRemapsIndex();
f+=TS_getScriptsIndex();
f+=TS_mkKeyRemap();
f+=TS_mkLayerKeyRemap();
f+=TS_mkScriptLaunch();
*/
    if(f == 0){printf("ALL OK\n");}
    else{printf("Algunos test fallaron\n");}
}

