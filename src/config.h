#include "parser.h"

void makeRemaps(){
    mkNewLayer(0);
    // Definir todos los keymaps dentro de esta funcion

    keyRemap(15,1,BLANK,0); // Tab a ESC

    // seleccionar Caps lock (58) como una capa
    mkNewLayer(58);

    // Los siguientes remaps solo se hacen efectivos
    // cuando está activada la capa

    // H,J,K,L a Izquierda, Abajo, Arriba, Derecha
    keyRemap(35,105,BLANK,0);
    keyRemap(36,108,BLANK,0);
    keyRemap(37,103,BLANK,0);
    keyRemap(38,106,BLANK,0);

    keyRemap(57,15,BLANK,0); // space a tab

    // Mantener Shift, Ctrl, alt, altgr, shift derecho
    // cuando se activa la capa
    keyRemap(42,42,BLANK,0);
    keyRemap(29,29,BLANK,0);
    keyRemap(56,56,BLANK,0);
    keyRemap(100,100,BLANK,0);
    keyRemap(54,54,BLANK,0);

    //setNewMacro(30);


    keyRemap(30,18,TECLA_PRESIONADA,0.2);
    keyRemap(30,18,TECLA_SOLTADA,0.2);

    keyRemap(30,30,TECLA_PRESIONADA,0.2);
    keyRemap(30,30,TECLA_SOLTADA,0.2);

    keyRemap(30,57,TECLA_PRESIONADA,0.2);
    keyRemap(30,57,TECLA_SOLTADA,0.2);

    script(30,"st&",TECLA_PRESIONADA,0.2);

    keyRemap(30,25,TECLA_PRESIONADA,0.2);
    keyRemap(30,25,TECLA_SOLTADA,0.2);

    keyRemap(30,25,TECLA_PRESIONADA,0.2);
    keyRemap(30,25,TECLA_SOLTADA,0.2);

    script(30,"st -e htop&",TECLA_PRESIONADA,0.2);
}
