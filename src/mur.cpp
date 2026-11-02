#include "mur.h"
#include <LibRobus.h>
#include "suiveurDeLigne.h"
#include "pid.h"

void contourner_mur(float distance)
{
    turn_degrees(90, true);
    delay(10);
    avance(distance);
    delay(10);
    turn_degrees(90, false);
    delay(10);
    avance(distance);
    delay(10);
    turn_degrees(90, false);
    delay(10);
    avance(distance);
}