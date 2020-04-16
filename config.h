#include "fnskr.h"

int makeRemaps(){
    // Definir todos los keymaps dentro de esta funcion
    int f[8];

    f[0] = 58; // remapear CapsLock
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,1); // a ESC

    f[0] = 102;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,2);

    f[0] = 104;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,4);

    f[0] = 109;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,9);

    f[0] = 107;
    f[1] = BLANK;
    f[2] = BLANK;
    f[3] = BLANK;
    f[4] = BLANK;
    f[5] = BLANK;
    f[6] = BLANK;
    f[7] = BLANK;

    mkKeyRemap(f,7);

    return 1;
}
