#include "mainFunctions.h"
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
void mkNewLayer(int fnKey){
    int index = getFreeLayer();
    if (index >= 0){
        layers[index].used = TRUE;
        layers[index].fnKey = fnKey;
    }
}
void script(int hotKey, int onAction,float sleep,char *script){
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
void parseConfigFromFile(FILE *fp){
    if(!fp){
        printf("cant open config file");
        exit(0);}
    int arg1,arg2,arg3,arg4;
    float arg5;
    char *token,*keyWord;
    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1) {
        keyWord = strsep(&line," ");
        if(strcmp(keyWord,"KeyboardPath") == 0){
            token = strsep(&line,"\n");
        }
        else if(strcmp(keyWord,"NewLayer") == 0){
            token = strsep(&line,"\n");
            sscanf(token,"%d",&arg1);
            mkNewLayer(arg1);
        }
        else if(strcmp(keyWord,"KeyRemap") == 0){
            token = strsep(&line," ");
            sscanf(token,"%d",&arg1);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg2);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg3);

            token = strsep(&line,"\n");
            sscanf(token,"%d",&arg4);
            keyRemap(arg1,arg2,arg3,arg4);
        }
        else if(strcmp(keyWord,"Script") == 0){
            token = strsep(&line," ");
            sscanf(token,"%d",&arg1);

            token = strsep(&line," ");
            sscanf(token,"%d",&arg2);

            token = strsep(&line," ");
            sscanf(token,"%f",&arg5);

            token = strsep(&line,"\n");
            script(arg1,arg2,arg5,token);
        }
        else if(strcmp(keyWord,"FAIL") == 0){
            token = strsep(&line,"\n");
            printf("FAIL %s\n",token);
            exit(1);
        }
    }
}
