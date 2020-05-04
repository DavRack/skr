#include "arrayFunctions.h"
int getFreeRemaps(remap remaps[]){
    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if(!remaps[i].remapUsed)
            return i;
    return -1;
}
remap getRemapMatch(remap remaps[],int teclas[],struct input_event ev){
    int teclasOrdenadas[8];
    arCpy(teclasOrdenadas,teclas);
    removeSpaces(teclasOrdenadas); 

    int teclasEmpty = arrIsEmpty(teclasOrdenadas);

    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if((eql(remaps[i].from,teclas) && !teclasEmpty) || (remaps[i].hotKey == ev.code))
            return remaps[i];
    return blankRemap;
}
int getFreeLayer(){
    for(int i=0;i<NUMBER_OF_LAYERS;i++)
        if(layers[i].fnKey == 0)
            return i;
    return -1;
}
int getLastLayer(){
    int index = getFreeLayer();
    if(index == 0){return 0;}
    else{return index-1;}
}
fnLayer getLayerMatch(int teclas[8]){
    for(int i = 0; i < NUMBER_OF_LAYERS; i++)
        if(in(teclas,layers[i].fnKey))
            return layers[i];
    return blankLayer;
}
void mkNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0)
        layers[index].fnKey = fnKey;
}
void mkKeyRemap(int from, int to,remap remaps[]){
    // Popula la primera posicion vacia del array remaps con from y to
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].type = TYPE_KEYREMAP;
        remaps[pVacia].hotKey = from;
        remaps[pVacia].remapAction.keyRemap=to;
    }
}
void keyRemap(int hotKey,int keyRemap){
    mkKeyRemap(hotKey,keyRemap,userRemaps);
}
void layerKeyRemap(int hotKey,int keyRemap){
    mkKeyRemap(hotKey,keyRemap,layers[getLastLayer()].fnRemaps);
}
void mkScriptLaunch(int from[8],char *script, int onAction,remap remaps[]){
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].type = TYPE_SCRIPT;
        remaps[pVacia].remapAction.onKeyState = onAction;
        arCpy(remaps[pVacia].from,from);

        remaps[pVacia].remapAction.script = (char *) malloc(sizeof(*script));
        strcpy(remaps[pVacia].remapAction.script,script);
    }
}
void scriptLaunch(int from[8],char *script,int onKeyState){
    mkScriptLaunch(from,script,onKeyState,userRemaps);
}
void sendKeyEvent(int KEY,int tipo){ // no testeada!!
    event.type=EV_KEY;
    event.code = KEY;
    event.value = tipo;

    fwrite(&rap1,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fwrite(&event,1,EV_SIZE,teclado);// se envia el evento perse
    fwrite(&rap2,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fflush(teclado);
}
void sendScript(char *script){ // no testeada!!
    popen(script,"w");
}
void executeRemap(remap action,struct input_event ev){
    if(action.type == TYPE_KEYREMAP)
        sendKeyEvent(action.remapAction.keyRemap,ev.value);
    else if(action.type == TYPE_SCRIPT)
        if(action.remapAction.onKeyState == ev.value)
        sendScript(action.remapAction.script);
}
void doAction(int teclas[],struct input_event keyEvent){

    layerActivada = getLayerMatch(teclas);
    if(layerActivada.fnKey != 0){
        if(layerActivada.fnKey == teclas[0]){
            int *teclasSinFnKey=popFirst(teclas);

            if(teclasSinFnKey[0] != -1){
                remapEnviado = getRemapMatch(layerActivada.fnRemaps,teclasSinFnKey,keyEvent);
                executeRemap(remapEnviado,keyEvent);
            }
            free(teclasSinFnKey);
        }
    }
    else{
        remapEnviado = getRemapMatch(userRemaps,teclas,keyEvent);
        if(remapEnviado.remapUsed == TRUE)
            executeRemap(remapEnviado,keyEvent);
        else
            sendKeyEvent(keyEvent.code,keyEvent.value);
    }
}
