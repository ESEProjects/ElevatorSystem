#include "BASEutils.h"

#if       ELEVATOR==(1)
#ifndef   ELEVATOR_CAR_H
#define   ELEVATOR_CAR_H


/* Macros */



/* Function Prototypes */

//Component Interface

//All indications on LCD

//Analog buttons

//CAN Communications Interface
void Send_DoorRequest(message *request_message);
void Send_FloorSelectRequest(message *request_message);

void Interpret_Message(message *response_message);

#endif
#endif