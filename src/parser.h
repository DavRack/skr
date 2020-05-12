#include "mainFunctions.h"
void mkNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0){
        layers[index].used = TRUE;
        layers[index].fnKey = fnKey;
    }
}
void add2Actions(int key,int keyState,int type,float sleep,char *script,remap *rp){
    int sleepSeconds = (int) sleep;
    int sleepMicroSeconds = (int) ((sleep-sleepSeconds)*100000);

    // if the time introduced by the user is unsafe, replace with a safe one
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
int getWorkingRemapIndex(int hotKey,remap *remaps){
    int pVacia = -1;
    for(int i = 0; i < NUMBER_OF_REMAPS; i++){
        if(remaps[i].hotKey == hotKey)
            pVacia = i;
        else if(remaps[i].remapUsed == FALSE)
            pVacia = i;
    }
    return pVacia;
}
void script(int hotKey,char *script, int onAction,float sleep){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getWorkingRemapIndex(hotKey,remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(BLANK,onAction,TYPE_SCRIPT,sleep,script,&remaps[pVacia]);
    }
}
void keyRemap(int hotKey, int keyCode,int keyState, float sleep){
    remap *remaps = layers[getLastLayer()].fnRemaps;
    int pVacia = getWorkingRemapIndex(hotKey,remaps);
    if(pVacia >= 0){
        remaps[pVacia].remapUsed = TRUE;
        remaps[pVacia].hotKey = hotKey;

        add2Actions(keyCode,keyState,TYPE_KEYREMAP,sleep,0,&remaps[pVacia]);
    }
}
