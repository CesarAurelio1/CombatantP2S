#include "mur.h"
#include <LibRobus.h>
#include "suiveurDeLigne.h"
#include "pid.h"

void contourner_mur(float distance)
{
    turn_degrees(95, 1);
    delay(10);
    avance(distance);
    delay(10);
    turn_degrees(90, 0);
    delay(10);
    avance(distance + 20);
    delay(10);
    turn_degrees(95, 0);
    delay(10);
    avance(distance);
}