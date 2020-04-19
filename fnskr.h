#include "definitions.h"

int append(int array[],int element){
    // Agregar elemento,"element", en la primera posicion vacia de array

    for(unsigned short i = 0; i < 8; i++){
        if(array[i] == BLANK){
            array[i] = element;
            return TRUE;
        }
    }
    return FALSE;
}
int in(int array[], int element){
    // Retorna TRUE si element está en array

    for(unsigned short i = 0; i < 8; i++){
        if(array[0] == element) { 
            return TRUE;
        }
    }
    return FALSE;
}
int cpyList(int form[], int to[]){
    // copia las listas eliminando los espacios entre keycodes


    return TRUE;
}
int pop(int array[],int element){
    /* Elimina el elemento ,"element", del array
     * en este contexto "eliminar" es cambiar dicho elemento
     * por -1, BLANK
     */

    for(unsigned short i = 0; i < 8; i++){
        if(array[i] == element){
            array[i] = BLANK;
            return TRUE;
        }
    }

    return FALSE;
}
int clear(int array[]){
    // Restablece el array a una lista de BLANK

    for(unsigned short i = 0; i < 8; i++){
        array[i] = BLANK;
    }

    return TRUE;
}
int printList(int array[]){
    // Imprime la lista de keycodes array en una sola linea de texto 

    for(unsigned short i = 0; i < 8; i++){
        printf("%d ",array[i]);
    }
    printf("\n");
    return TRUE;
}
int popFirst(int destino[], int origen[]){

    short index = 0;
    for(short i = 1; i < 8; i++){
        if(origen[i] > 0){
            destino[index] = origen[i];
            index++;
        }
    }
    for(int i = index; i < 8; i++){
        destino[i] = BLANK;
    }

    return TRUE;
}
int find(int array[],int pattern[]){
    /* Busca si las teclas presionadas (array)
    * son las mismas que las teclas espeficadas en el remapeo
    *
    * Si las listas son iguales retorna TRUE
    * Si las listas son diferentes retorna FALSE
    *
    * Es posible que el array de teclas presionadas tenga 
    * espacios, "BLANK", entre las teclas
    * Ej:
    *      {BLANK,29,BLANK,BLANK,30,BLANK,BLANK,BLANK}
    *
    * por lo tanto primero se genera un array donde todos los keycodes
    * sean consecutivos
    *      
    *      {29,30,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK}
    *
    * en auxList se guardan la lista de keycodes consecutivos
    */ 
    int auxList[8]={BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK,BLANK};

    for(unsigned short i=0;i<8;i++){

        // Para cada elemnto no vacio de la lista de teclas se agrega
        // dicho elemento a la primera posición vacia de auxList 
        if(array[i] != BLANK){
            append(auxList,array[i]);
        }
    }

    // Si las listas son diferentes en cualquier posición se retorna FALSE 
    for(unsigned short i = 0; i < 8; i++){
        if(pattern[i] != auxList[i]){
            return FALSE;
        }
    }
    return TRUE;
}
int getFreeRemaps(){
    // Retorna el indice de la primera posición vacia en remaps
    // Si no se encuentra ninguna posición vacía se retorna -1

    for(int i = 0; i < 256; i++){
        if(remaps[i].to == 0){
            return i;
        }
    }

    return -1;
}
int getFreeScripts(){   
    for(int i = 0; i < 256; i++){
        if(scripts[i].to == 0){
            return i;
        }
    }
    return -1;
}
int setNewLayer(int fnKey){
    for(int i = 0; i < 32; i++){
        if(layers[i].fnKey == 0){
            layers[i].fnKey = fnKey;
            return TRUE;
        }
    }
    return FALSE;
}
int mkKeyRemap(int from, int to){
    // Popula la primera posicion vacia del array remaps con from y to
    
    // Se consigue la primera posicion vacia del array remaps
    int pVacia = getFreeRemaps();

    remaps[pVacia].from=from;

    remaps[pVacia].to=to;

    return TRUE;

}
int mkScriptLaunch(int from[8],char *to, int onAction){
    // Popula la primera posicion vacia del array scripts con
    // from, to y onAction

    int pVacia = getFreeScripts();

    // se popula el vector from de la estructura
    // en la posicion vacia del array remaps
    for(unsigned int i=0;i<8;i++){
        scripts[pVacia].from[i]=from[i];
    }

    // Se popula el "string" to
    scripts[pVacia].to = (char *) malloc(sizeof(to));
    strcpy(scripts[pVacia].to,to);

    scripts[pVacia].onAction = onAction;

    return TRUE;
}
int getLastLayer(){
    int index = -1;
    for(int i = 0; i < 32; i++){
        if(layers[i].fnKey > 0){
            index = i;
        }else{return index;}
    }
    return index;
}
int mkLayerKeyRemap(int from, int to){
    int index = getLastLayer();
    for(int i=0; i < 256; i++){
        if(layers[index].remaps[i].to == 0){
            layers[index].remaps[i].from = from;
            layers[index].remaps[i].to = to;
            return TRUE;
        }
    }
    return FALSE;
}
int getRemapsIndex(int keyCode){
    // Retorna el indice en remaps[] del evento que contenga
    // el patron teclas.
    // si no se encuentra retorna -1

    for(int i = 0; i < 256; i++){

        // Se itera sobre los elemtos que tienen un "to" diferente a 0
        // es decir; elemtos "no vacios"
        if(remaps[i].to != 0){

            if(remaps[i].from == keyCode){

                // se retorna el indice de dicho elemnto
                return i;
            }
        }

        // si se encuentra un elemnto vacio, se retorna -1 para
        // evitar que se recorra el resto de la lista
        else{
            return -1;
        }
    }

    // si se llega al final de la lista sin encontrar un match
    return -1;
}
int getScriptsIndex(int teclas[]){
    // Retorna el indice en scripts[] del evento que contenga
    // el patron teclas.
    // si no se encuentra retorna -1

    for(int i = 0; i < 256; i++){

        // Se itera sobre los elemtos que tienen un "to" diferente a 0
        // es decir; elemtos "no vacios"
        if(scripts[i].to != 0){

            // Si el las teclas presionadas (teclas) coinciden
            // con el patron del elemento a remapear
            if(find(teclas,scripts[i].from) == 1){

                // se retorna el indice de dicho elemnto
                return i;
            }
        }

        // si se encuentra un elemnto vacio, se retorna -1 para
        // evitar que se recorra el resto de la lista
        else{
            return -1;
        }
    }

    // si se llega al final de la lista sin encontrar un match
    return -1;
}
int getLayerIndex(int teclas[8]){
    for(int i = 0; i < 32; i++){
        if(layers[i].fnKey == teclas[0]){
            return i;
        }
    }
    return -1;
}
int sendEvent(struct input_event evento, FILE * kb){
    // Recibe un evento y lo envia por medio del teclado solicitado

    // Se envía el primer envoltorio
    fwrite(&rap1,1,EV_SIZE,kb);

    // se envia el evento perse
    fwrite(&evento,1,EV_SIZE,kb);

    // Se envía el primer envoltorio
    fwrite(&rap2,1,EV_SIZE,kb);

    // Se ejecuta fflush para "guardar" los cambios realizados y 
    // que estos cambios tengan efecto
    fflush(kb);

    return 1;
}
int sendKeyEvent(int KEY,int tipo){
    /*
     * Prepara un evento para ser enviado por medio de la funcion sendEvent
     *  recibe:
     *           KEY     -> Número de la tecla
     *          tipo    -> 0=soltada, 1=presionada, 2=mantenida
     */

    // se idica que event es de tipo KEY
    event.type=EV_KEY;

    event.code = KEY;
    event.value = tipo;

    // Se efectua la escritdiy coffee machineura al teclado
    sendEvent(event,teclado);
    
    return 1;
}
int sendScript(int indiceScript){
    // Recibe el indice de un evento en scripts
    // y envia el script especificado en scripts[].to
    popen(scripts[indiceScript].to,"w");
    return TRUE;
}
struct actionToDo getAction(int teclas[],int keyCode,int keyState){

    struct actionToDo action;
    
    // Remapear tecla
    remapEnviado = getRemapsIndex(keyCode);

    if(remapEnviado != BLANK){
        action.type = 1;
        action.index = remapEnviado;
        action.keyState = keyState;
        return action;
    }

    // Enviar script
    scriptEnviado = getScriptsIndex(teclas);

    if(scriptEnviado != BLANK){ 
        //if (scripts[scriptEnviado].onAction == keyState){
        action.type = 2;
        action.index = scriptEnviado;
        action.keyState = keyState;
        //}
        return action;
    }

    // Activar capa
    capaActivada = getLayerIndex(teclas);

    if(capaActivada != BLANK){
        action.type = 3;
        action.index = capaActivada;
        action.keyState = keyState;
        return action;
    }

    // Enviar tecla
    if(remapEnviado == BLANK){
        action.type = 0;
        action.index = keyCode;
        action.keyState = keyState;
        return action;
    }

    action.type = -1;
    action.index = 0;
    action.keyState = keyState;
    return action;

}
int doAction(struct actionToDo action, int teclas[8]){
    // se manipula ev

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
        int teclasSinFnKey[8];

        popFirst(teclasSinFnKey,teclas);

        if(teclasSinFnKey[0] != -1){
            for(int i = 0; i < 256; i++){
                if(layer.remaps[i].from == teclasSinFnKey[0]){
                    sendKeyEvent(layer.remaps[i].to,keyState);
                    return TRUE;
                }
            }
        }

    }
    return FALSE;
}
