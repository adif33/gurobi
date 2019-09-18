#include <stdio.h>

#include "main_aux.h"


int main() {
    int status = 0;
    printf(GAME_BEGINNING_BANNER);

    status = mainLoop();

    printf(GAME_ENDING_BANNER);

    return status;
}
