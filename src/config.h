#include "fnskr.h"

void makeRemaps(){
    mkNewLayer(0);
    // Definir todos los keymaps dentro de esta funcion

    keyRemap(15,1); // Tab a ESC
}

void makeScripts(){
    // lanzar alacritty con la tecla fin
    //scriptLaunch(107,"alacritty&",TECLA_SOLTADA);
}

void makeLayers(){
    // seleccionar Caps lock (58) como una capa
    mkNewLayer(58);
    
    // Los siguientes remaps solo se hacen efectivos
    // cuando está activada la capa

    // H,J,K,L a Izquierda, Abajo, Arriba, Derecha
    keyRemap(35,105); 
    keyRemap(36,108); 
    keyRemap(37,103); 
    keyRemap(38,106); 

    keyRemap(57,15); // space a tab

    // Mnatener Shift, Ctrl, alt, altgr, shift derecho 
    // cuando se activa la capa
    keyRemap(42,42);
    keyRemap(29,29);
    keyRemap(56,56);
    keyRemap(100,100);
    keyRemap(54,54);

    //setNewLayerMacro(30);

    //layerMacroKey(18,TECLA_PRESIONADA,0);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0.5);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0.5);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0.5);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

    //layerMacroKey(18,TECLA_PRESIONADA,0.5);
    //layerMacroKey(18,TECLA_SOLTADA,0.05);

    //layerMacroKey(30,TECLA_PRESIONADA,0.05);
    //layerMacroKey(30,TECLA_SOLTADA,0.05);

    //layerMacroKey(57,TECLA_PRESIONADA,0.05);
    //layerMacroKey(57,TECLA_SOLTADA,0.05);

}
