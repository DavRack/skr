#include "arrayFunctions.h"
int keyIsFnKey(int key){
    for(int i=0; i < NUMBER_OF_LAYERS; i++)
        if(layers[i].fnKey == key){return TRUE;}
    return FALSE;
}
int getFreeRemaps(remap remaps[]){
    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if(!remaps[i].remapUsed)
            return i;
    return -1;
}
remap getRemapMatch(remap remaps[],int keyCode){
    for(int i = 0; i < NUMBER_OF_REMAPS; i++)
        if(remaps[i].hotKey == keyCode)
            return remaps[i];
    return blankRemap;
}
int getFreeLayer(){
    for(int i=0;i<NUMBER_OF_LAYERS;i++)
        if(layers[i].used == 0)
            return i;
    return -1;
}
int getLastLayer(){
    int index = getFreeLayer();
    if(index == 0){return 0;}
    else{return index-1;}
}
fnLayer getLayerMatch(int teclas[8]){
    for(int i = 1; i < NUMBER_OF_LAYERS; i++)
        if(teclas[0] == layers[i].fnKey)
            return layers[i];
    return layers[0];
}
void sendKeyEvent(int KEY,int tipo){
    event.type=EV_KEY;
    event.code = KEY;
    event.value = tipo;

    fwrite(&rap1,1,EV_SIZE,teclado);// envía el primer envoltorio
    fwrite(&event,1,EV_SIZE,teclado);// envia el evento per se
    fwrite(&rap2,1,EV_SIZE,teclado);// envía el primer envoltorio
    fflush(teclado);
}
void sendScript(char *script){
    popen(script,"w");
}
void executeAction(action toExecute,int keyState){
    if(toExecute.type == TYPE_KEYREMAP)
        sendKeyEvent(toExecute.key,keyState);
    else if(toExecute.type == TYPE_SCRIPT)
        if(toExecute.keyState == keyState)
            sendScript(toExecute.script);
} 
void executeActions(action actions[],struct input_event event){
    for(int i = 0; i < MACRO_LENGTH; i++){
        if(actions[i].actionUsed == TRUE){

           if(actions[i].sleepSeconds > 0)
                sleep(actions[i].sleepSeconds);
            if(actions[i].sleepMicroSeconds > 0)
                usleep(actions[i].sleepMicroSeconds);

            executeAction(actions[i],event.value);
        }
    }
}
void releaseLayerKeys(int teclas[],remap layerRemaps[]){
    for(int i = 7; i >= 1; i--){
        remapEnviado=getRemapMatch(layerRemaps,teclas[i]);
        if(remapEnviado.remapUsed && !remapEnviado.actions[1].actionUsed)
            if(remapEnviado.actions[0].type == TYPE_KEYREMAP)
                executeAction(remapEnviado.actions[0],TECLA_SOLTADA);
    }
}
void doAction(int teclas[],struct input_event keyEvent){
    layerActivada = getLayerMatch(teclas); 

    if(keyEvent.value == TECLA_SOLTADA && keyIsFnKey(keyEvent.code))
        releaseLayerKeys(teclas,layerActivada.fnRemaps);

    remapEnviado = getRemapMatch(layerActivada.fnRemaps,keyEvent.code);

    if(remapEnviado.remapUsed == TRUE)
        executeActions(remapEnviado.actions,keyEvent);
    else if(!keyIsFnKey(keyEvent.code))
        sendKeyEvent(keyEvent.code,keyEvent.value);
}
