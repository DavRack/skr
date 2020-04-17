#include <libevdev/libevdev.h>
#include <libinput.h>
#include <libudev.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/* En el archivo definitions.h se encuentran las definiciones de
 * las estructuras de datos para:
 *      - Remapeo de teclas
 *      - Lanzamiento de scripts
 *      - Capas
 *
 * En el archivo config.h se encuentran las definiciones del usuario de:
 *      - Remapeo de teclas
 *      - Lanzamiento de scripts
 *      - Capas
 */
#include "config.h"

/* ######## Como funciona el envio de teclas: ########
 * cada key_stroke esta compuesto por 3 eventos
 * un evento per se envuelto entre dos eventos especiales
 *
 *      (Inicio envoltorio) ---------------> rap1
 *
 *
 *                                 +-------> (evento press)
 *                                 |               
 *                  (event)  -> ---+-------> (evento release)
 *                                 |               
 *                                 +-------> (evento repeat)
 *
 *
 *      (Fin envoltorio) ------------------> rap2
 *
 * ######## Como funciona una estructura input_event ########
 * Una estructura input_event está compuesta (para nuestro proposito)
 * de los siguientes valores
 *
 * input event{
 *      type  = tipo de evento (cuando es un tecleo: type=EV_KEY)
 *      code  = numero de la tecla presionada: (code = 30) -> KEY_A
 *      value = tipo evento de tecla 0=soltada 1=presionada 2=mantenida
 * }
 */

int main(){
    // ruta absoluta al teclado que se va a usar
    // este archivo debe tener permisos 666
    //const char * ruta = "/dev/input/event3";

    // Se asignan lo valores para el primer evento especial rap1
    rap1.type = 4;
    rap1.code = 4;
    rap1.value = 31;

    // Se asignan lo valores para el primer evento especial rap2
    rap2.type = 0;
    rap2.code = 0;
    rap2.value = 0;

    //se usa el comando intercept para obtener los eventos
    //generados por el teclado especificado
    input = popen("sudo intercept -g /dev/input/event3","r");

    // se asigna el teclado en la ruta especificada al teclado
    teclado = popen("sudo uinput -d /dev/input/event3","w");

    makeRemaps();

    // Ciclo principal de la aplicación!
    // se lee cada evento generado por el teclado
    while (fread(&ev, sizeof(event), 1, input) == 1) {
        if(ev.type == EV_KEY){
            remapEnviado = -1;

            if(ev.value == TECLA_PRESIONADA){
                append(teclas,ev.code);
                remapEnviado = getMatchIndex(teclas);
                if(remapEnviado != -1){
                    sendKeyEvent(remaps[remapEnviado].to,TECLA_PRESIONADA);
                }
            }

            else if(ev.value == TECLA_SOLTADA){
                remapEnviado = getMatchIndex(teclas);
                if(remapEnviado != -1){
                    sendKeyEvent(remaps[remapEnviado].to,TECLA_SOLTADA);
                }
                pop(teclas,ev.code);
            }

            else if(ev.value == TECLA_MANTENIDA){
                remapEnviado = getMatchIndex(teclas);
                if(remapEnviado != -1){
                    sendKeyEvent(remaps[remapEnviado].to,TECLA_MANTENIDA);
                }
            }

            if(remapEnviado == -1){
                sendKeyEvent(ev.code,ev.value);
            }
        }
    }
}
 // TODO:
 // - buscar una forma de medir el rendimiento desde c
