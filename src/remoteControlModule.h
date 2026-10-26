// parcours.h
#ifndef REMOTE_CONTROL_MODULE_H
#define REMOTE_CONTROL_MODULE_H

#include <Arduino.h>

typedef void (*Button_Callback_t)(void);

void InitRemoteControlModule(void);
void UpdateRemoteControlModule(void);


#endif /* REMOTE_CONTROL_MODULE_H */