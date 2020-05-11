#include "mainFunctions.h"
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
void add2Actions(int key,int keyState,int type,float sleep,char *script,remap *rp){
    int sleepSeconds = (int) sleep;
    int sleepMicroSeconds = (int) ((sleep-sleepSeconds)*100000);

    // if the time introduced by the user is unsafe replaceit 
    // with a safe one
    if(sleepMicroSeconds > 0 && sleepMicroSeconds < MIN_SLEEP_TIME)
        sleepMicroSeconds=MIN_SLEEP_TIME;

    for(int i = 0; i < MACRO_LENGTH; i++){
        if(rp->actions[i].actionUsed == FALSE){
            rp->actions[i].actionUsed = TRUE;
            rp->actions[i].sleepSeconds = sleepSeconds;
            rp->actions[i].sleepMicroSeconds = sleepMicroSeconds;
            rp->actions[i].type = type;
            rp->actions[i].key = key;
            rp->actions[i].keyState = keyState;
            rp->actions[i].script = script;
            break;
        }
    }
}
void script(int hotKey,char *script, int onAction){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(0,onAction,TYPE_SCRIPT,0,0,&remaps[pVacia]);
    }
}
void keyRemap(int hotKey, int keyRemap){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getFreeRemaps(remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(keyRemap,BLANK,TYPE_KEYREMAP,0,0,&remaps[pVacia]);
    }
}
void macroKey(int keyCode, int keyState, float sleepTime){
    int index = getFreeRemaps(layers[getLastLayer()].fnRemaps);
    if(index>=0){
        if(index > 0){index--;}
        remap *remaps = layers[getLastLayer()].fnRemaps;
        add2Actions(keyCode,keyState,TYPE_KEYREMAP,sleepTime,0,&remaps[index]);
    }
}
void macroScript(char *script,float sleep,remap remaps[]){
    int remapIndex = getFreeRemaps(layers[getLastLayer()].fnRemaps);
    if(remapIndex>=0){
        if(remapIndex > 0){remapIndex--;}
        add2Actions(0,TECLA_PRESIONADA,TYPE_SCRIPT,sleep,0,&remaps[remapIndex]);
    }
}
