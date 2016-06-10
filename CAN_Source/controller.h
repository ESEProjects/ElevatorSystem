#if     CONTROLLER==1
#ifndef CONTROLLER_H
#define CONTROLLER_h


/* Function Prototypes */

//Componenet Interface Functions
//No components to interface to.

//CAN Communication Functions
void Interpret_Request(message request_message);

void Send_Message(message *request_message);


#endif
#endif