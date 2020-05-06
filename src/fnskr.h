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
void mkNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0){
        layers[index].used = TRUE;
        layers[index].fnKey = fnKey;
    }
}
void setNewMacro(int hotKey){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getFreeRemaps(remaps);
    if(pVacia>=0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;
    }
}
void add2Actions(int key,int keyState,int type,float sleep,char *script,remap rp[],int index){
    int sleepSeconds = (int) sleep;
    int sleepMicroSeconds = (int) ((sleep-(sleepSeconds))*1000000);

    if(sleepMicroSeconds > 0 && sleepMicroSeconds < MIN_SLEEP_TIME)
        sleepMicroSeconds=MIN_SLEEP_TIME;
    for(int i = 0; i < MACRO_LENGTH; i++){
        if(rp[index].actions[i].actionUsed == FALSE){
            rp[index].actions[i].actionUsed = TRUE;
            rp[index].actions[i].sleepSeconds = sleepSeconds;
            rp[index].actions[i].sleepMicroSeconds = sleepMicroSeconds;
            rp[index].actions[i].type = type;
            rp[index].actions[i].key = key;
            rp[index].actions[i].keyState = keyState;
            rp[index].actions[i].script = script;
            break;
        }
    }
}
void scriptLaunch(int hotKey,char *script, int onAction){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(0,0,TYPE_SCRIPT,0,0,remaps,pVacia);
    }
}
void keyRemap(int hotKey, int keyRemap){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(keyRemap,0,TYPE_KEYREMAP,0,0,remaps,pVacia);
    }
}
void macroKey(int keyCode, int keyState, float sleepTime){
    int index = getFreeRemaps(layers[getLastLayer()].fnRemaps);
    if(index>=0){
        if(index > 0){index--;}
        remap *remaps = layers[getLastLayer()].fnRemaps;
        add2Actions(keyCode,keyState,TYPE_KEYREMAP,sleepTime,0,remaps,index);
    }
}
void macroScript(char *script,float sleep,remap remaps[]){
    int remapIndex = getFreeRemaps(layers[getLastLayer()].fnRemaps);
    if(remapIndex>=0){
        if(remapIndex > 0){remapIndex--;}
        add2Actions(0,TECLA_PRESIONADA,TYPE_SCRIPT,sleep,0,remaps,remapIndex);
    }
}
void sendKeyEvent(int KEY,int tipo){ // no testeada!!
    event.type=EV_KEY;
    event.code = KEY;
    event.value = tipo;

    fwrite(&rap1,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fwrite(&event,1,EV_SIZE,teclado);// se envia el evento per se
    fwrite(&rap2,1,EV_SIZE,teclado);// Se envía el primer envoltorio
    fflush(teclado);
}
void sendScript(char *script){ // no testeada!!
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

            if(actions[i].sleepSeconds)
                sleep(actions[i].sleepSeconds);
            if(actions[i].sleepMicroSeconds)
                usleep(actions[i].sleepMicroSeconds);

            executeAction(actions[i],event.value);
        }
    }
}
void doAction(int teclas[],struct input_event keyEvent){
    layerActivada = getLayerMatch(teclas);
    
    remapEnviado = getRemapMatch(layerActivada.fnRemaps,keyEvent.code);

    if(remapEnviado.remapUsed == TRUE)
        executeActions(remapEnviado.actions,keyEvent);
    else if(!keyIsFnKey(keyEvent.code))
        sendKeyEvent(keyEvent.code,keyEvent.value);
}
