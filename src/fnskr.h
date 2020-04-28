#include "arrayFunctions.h"

int getFreeRemaps(){
    // Retorna el indice de la primera posición vacia en remaps
    // Si no se encuentra ninguna posición vacía se retorna -1
    for(int i = 0; i < 256; i++)
        if(remaps[i].to == 0)
            return i;
    return -1;
}
int getFreeScripts(){   
    for(int i = 0; i < 256; i++)
        if(scripts[i].to == 0)
            return i;
    return -1;
}
int getFreeLayer(){
    for(int i=0;i<32;i++)
        if(layers[i].fnKey == 0)
            return i;
    return -1;
}
int setNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0){
        layers[index].fnKey = fnKey;
        return TRUE;
    }
    else return FALSE;
}
int mkKeyRemap(int from, int to){
    // Popula la primera posicion vacia del array remaps con from y to
    int pVacia = getFreeRemaps();
    if(pVacia >= 0){
        remaps[pVacia].from=from;
        remaps[pVacia].to=to;
    }
    return pVacia;
}
int mkScriptLaunch(int from[8],char *to, int onAction){
    // Popula la primera posicion vacia del array scripts con
    // from, to y onAction
    int pVacia = getFreeScripts();
    if(pVacia >= 0){
        // se popula el vector from de la estructura
        // en la posicion vacia del array remaps
        for(unsigned int i=0;i<8;i++){scripts[pVacia].from[i]=from[i];}
        scripts[pVacia].to = (char *) malloc(sizeof(*to));
        strcpy(scripts[pVacia].to,to);
        scripts[pVacia].onAction = onAction;
    }
    return TRUE;
}
int getLastLayer(){
    int index = -1;
    for(int i = 0; i < 32; i++){
        if(layers[i].fnKey > 0){index = i;}
        else{return index;}
    }
    return index;
}
int mkLayerKeyRemap(int from, int to){
    int index = getLastLayer();
    for(int i=0; i < 256; i++){
        if(layers[index].remaps[i].to == 0){
            layers[index].remaps[i].from = from;
            layers[index].remaps[i].to = to;
            return index;
        }
    }
    return index;
}
int getRemapsIndex(int keyCode){
    // Retorna el indice en remaps[] del evento que contenga el patron teclas.
    // si no se encuentra retorna -1
    for(int i = 0; i < 256; i++)
        if(remaps[i].from == keyCode)
            return i;
    return -1;
}
int getScriptsIndex(int teclas[]){
    // Retorna el indice en scripts[] del evento que contenga el patron teclas.
    // si no se encuentra retorna -1
    for(int i = 0; i < 256; i++){
        if(find(teclas,scripts[i].from))
            return i;
    }
    return -1;
}
int getLayerIndex(int teclas[8]){
    for(int i = 0; i < 32; i++)
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
int sendScript(int indiceScript){ // no testeada!!
    // Recibe el indice de un evento en scripts
    // y envia el script especificado en scripts[].to
    popen(scripts[indiceScript].to,"w");
    return TRUE;
}
struct actionToDo getAction(int teclas[],int keyCode,int keyState){
    struct actionToDo action;
    capaActivada = getLayerIndex(teclas);
    if(capaActivada != BLANK){
        if(layers[capaActivada].fnKey == teclas[0]){
            action.type = 3;
            action.index = capaActivada;
            action.keyState = keyState;
            return action;
        }
        else if(keyCode == layers[capaActivada].fnKey){
            action.type = BLANK;
            action.index = BLANK;
            action.keyState = keyState;
            return action;
        }
    }
    remapEnviado = getRemapsIndex(keyCode);
    if(remapEnviado != BLANK){
        action.type = 1;
        action.index = remapEnviado;
        action.keyState = keyState;
        return action;
    }
    scriptEnviado = getScriptsIndex(teclas);
    if(scriptEnviado != BLANK){ 
        action.type = 2;
        action.index = scriptEnviado;
        action.keyState = keyState;
        return action;
    }
    action.type = 0;
    action.index = keyCode;
    action.keyState = keyState;
    return action;
}
int doAction(struct actionToDo action, int teclas[8],struct input_event ev){
    if(action.type == 0){
        sendKeyEvent(action.index,action.keyState);
        return TRUE;
    }
    else if(action.type == 1){
        sendKeyEvent(remaps[action.index].to,action.keyState);
        return TRUE;
    }
    else if(action.type == 2){
        if(action.keyState == scripts[action.index].onAction){
            sendScript(action.index);
            return TRUE;
        }
    }
    else if(action.type == 3){
        struct functionLayer layer = layers[action.index];
        int keyState = action.keyState;
        int *teclasSinFnKey;

        teclasSinFnKey=popFirst(teclas);

        if(teclasSinFnKey[0] != -1){
            for(int i = 0; i < 256; i++){
                if(layer.remaps[i].from == ev.code){
                    sendKeyEvent(layer.remaps[i].to,keyState);
                    return TRUE;
                }
            }
        }
        free(teclasSinFnKey);
    }
    return FALSE;
}
