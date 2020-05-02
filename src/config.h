#include "fnskr.h"

void makeRemaps(){
    // Definir todos los keymaps dentro de esta funcion

    keyRemap(15,1); // Tab a ESC
}

void makeScripts(){
    int f[8];

    f[0] = 29; 
    f[1] = 107;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    // lanzar alacritty con la tecla fin
    scriptLaunch(f,"alacritty&",TECLA_SOLTADA);
}

void makeLayers(){
    // seleccionar Caps lock como una capa
    mkNewLayer(58);
    
    // Los siguientes remaps solo se hacen efectivos
    // cuando está activada la capa

    // H,J,K,L a Izquierda, Abajo, Arriba, Derecha
    layerKeyRemap(35,105); 
    layerKeyRemap(36,108); 
    layerKeyRemap(37,103); 
    layerKeyRemap(38,106); 

    layerKeyRemap(57,15); // space a tab

    // Mnatener Shift, Ctrl, alt, altgr, shift derecho 
    // cuando se activa la capa
    layerKeyRemap(42,42);
    layerKeyRemap(29,29);
    layerKeyRemap(56,56);
    layerKeyRemap(100,100);
    layerKeyRemap(54,54);
}
