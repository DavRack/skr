#include "fnskr.h"

int makeRemaps(){
    // Definir todos los keymaps dentro de esta funcion

    mkKeyRemap(15,1); // a ESC

    mkKeyRemap(102,2);

    mkKeyRemap(104,4);

    mkKeyRemap(109,9);

    return TRUE;
}

int makeScripts(){
    int f[8];

    f[0] = 107; 
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkScriptLaunch(f,"st&",TECLA_SOLTADA);

    return TRUE;
}

int makeLayers(){
    setNewLayer(58);
    mkLayerKeyRemap(35,105);
    mkLayerKeyRemap(36,108);
    mkLayerKeyRemap(37,103);
    mkLayerKeyRemap(38,106);
    return TRUE;
}
