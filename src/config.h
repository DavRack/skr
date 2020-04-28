#include "fnskr.h"

void makeRemaps(){
    // Definir todos los keymaps dentro de esta funcion

    mkKeyRemap(15,1); // Tab a ESC
}

void makeScripts(){
    int f[8];

    f[0] = 107; 
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    // lanzar alacritty con la tecla fin
    mkScriptLaunch(f,"alacritty&",TECLA_SOLTADA);
}

void makeLayers(){
    // seleccionar Caps lock como una capa
    setNewLayer(58);
    
    // Los siguientes remaps solo se hacen efectivos
    // cuando está activada la capa

    // H,J,K,L a Izquierda, Abajo, Arriba, Derecha
    mkLayerKeyRemap(35,105); 
    mkLayerKeyRemap(36,108); 
    mkLayerKeyRemap(37,103); 
    mkLayerKeyRemap(38,106); 

    mkLayerKeyRemap(57,15); // space a tab

    // Mnatener Shift, Ctrl, alt, altgr, shift derecho 
    // cuando se activa la capa
    mkLayerKeyRemap(42,42);
    mkLayerKeyRemap(29,29);
    mkLayerKeyRemap(56,56);
    mkLayerKeyRemap(100,100);
    mkLayerKeyRemap(54,54);
}
