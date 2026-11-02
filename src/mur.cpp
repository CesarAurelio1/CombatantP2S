#include "mur.h"
#include <LibRobus.h>
#include "PID.h"

void contourner_mur(float distance)
{
    turn(90, true);
    delay(10);
    avance(30);
    delay(10);
    turn(90, false);
    delay(10);
    avance(30);
    delay(10);
    turn(90, false);
    delay(10);
    avance(30);
}