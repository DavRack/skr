#include "arrayFunctions.h"
void initRemaps(){
    for(int i=0; i < NUMBER_OF_REMAPS; i++)
        for(int j = 0; j < 8; j++)
            userRemaps[i].from[j] = BLANK;
}
int getFreeRemaps(remap remaps[]){
    // Retorna el indice de la primera posición vacia en remaps
    // Si no se encuentra ninguna posición vacía se retorna -1
    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if(remaps[i].remapIsEmpty)
            return i;
    return -1;
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
int mkNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0){
        layers[index].fnKey = fnKey;
        return TRUE;
    }
    else return FALSE;
}
int getRemapsIndex(remap remaps[],int teclas[],struct input_event ev){
    // Retorna el indice en remaps[] del evento que contenga el patron teclas.
    // si no se encuentra retorna -1
    int teclasOrdenadas[8];
    arCpy(teclasOrdenadas,teclas);
    removeSpaces(teclasOrdenadas); 

    int teclasEmpty = arrIsEmpty(teclasOrdenadas);

    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if((eql(remaps[i].from,teclas) && !teclasEmpty) || remaps[i].hotKey == ev.code)
            return i;
    return -1;
}
int getLayerIndex(int teclas[8]){
    for(int i = 0; i < NUMBER_OF_LAYERS; i++)
        if(in(teclas,layers[i].fnKey))
            return i;
    return -1;
}
void sendEvent(struct input_event evento, FILE * teclado){ // no testeada!
    // Recibe un evento y lo envia por medio del teclado solicitado
    fwrite(&rap1,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fwrite(&evento,1,EV_SIZE,teclado);// se envia el evento perse
    fwrite(&rap2,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fflush(teclado);
}
void sendKeyEvent(int KEY,int tipo){ // no testeada!!
    event.type=EV_KEY;
    event.code = KEY;
    event.value = tipo;
    sendEvent(event,teclado);
}
void mkKeyRemap(int from, int to,remap remaps[]){
    // Popula la primera posicion vacia del array remaps con from y to
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapIsEmpty = FALSE;
        remaps[pVacia].type = TYPE_KEYREMAP;
        remaps[pVacia].hotKey = from;
        remaps[pVacia].keyRemap=to;
    }
}
void keyRemap(int hotKey,int keyRemap){
    mkKeyRemap(hotKey,keyRemap,userRemaps);
}
void layerKeyRemap(int hotKey,int keyRemap){
    mkKeyRemap(hotKey,keyRemap,layers[getLastLayer()].fnRemaps);
}
void mkScriptLaunch(int from[8],char *script, int onAction,remap remaps[]){
    // Popula la primera posicion vacia del array scripts con
    // from, to y onAction
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        // se popula el vector from de la estructura
        // en la posicion vacia del array remaps
        remaps[pVacia].remapIsEmpty = FALSE;
        remaps[pVacia].type = TYPE_SCRIPT;
        remaps[pVacia].onKeyState = onAction;
        arCpy(remaps[pVacia].from,from);

        remaps[pVacia].script = (char *) malloc(sizeof(*script));
        strcpy(remaps[pVacia].script,script);
    }
}
void scriptLaunch(int from[8],char *script,int onKeyState){
    mkScriptLaunch(from,script,onKeyState,userRemaps);
}
int sendScript(char *script){ // no testeada!!
    // Recibe el indice de un evento en scripts
    // y envia el script especificado en scripts[].to
    popen(script,"w");
    return TRUE;
}
remap getAction(int teclas[],struct input_event keyEvent){

    capaActivada = getLayerIndex(teclas);
    if(capaActivada != BLANK){
        if(layers[capaActivada].fnKey == teclas[0]){
            int *teclasSinFnKey;
            teclasSinFnKey=popFirst(teclas);

            remapEnviado = -1;

            if(teclasSinFnKey[0] != -1){
                remapEnviado = getRemapsIndex(layers[capaActivada].fnRemaps,teclasSinFnKey,keyEvent);
            }
            free(teclasSinFnKey);
            if(remapEnviado > -1){return layers[capaActivada].fnRemaps[remapEnviado];}
            else{return blankRemap;}
        }
        else if(keyEvent.code == layers[capaActivada].fnKey){
            auxRemap.type = BLANK;
            return auxRemap;
        }
    }

    remapEnviado = getRemapsIndex(userRemaps,teclas,keyEvent);
    if(remapEnviado != BLANK)
        return userRemaps[remapEnviado];

    else{
        auxRemap.type = 0;
        auxRemap.keyRemap = keyEvent.code;
        auxRemap.onKeyState = keyEvent.value;
        return auxRemap;
    }
}
void doAction(remap action,struct input_event ev){
    if(action.type == TYPE_NORMAL){
        sendKeyEvent(ev.code,ev.value);
    }
    else if(action.type == TYPE_KEYREMAP){
        sendKeyEvent(action.keyRemap,ev.value);
    }
    else if(action.type == TYPE_SCRIPT){
        if(action.onKeyState == ev.value){
            sendScript(action.script);
        }
    }
}
