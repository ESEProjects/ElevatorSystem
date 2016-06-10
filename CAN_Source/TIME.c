#include <hidef.h>      /* common defines and macros */
#include <stdarg.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "BASEutils.h"
#include "TIMEutils.h"

unsigned int heartbeat_count;

/******************************************************************************
Name:               Heartbeat_Init()
Arguments:          void
Returns:            void
Description:        This function sets up the registers needed for the RTI
                    module to start running at a 1 ms time interval and
                    initializes the count to zero.
******************************************************************************/
void Heartbeat_Init(void){
          SET_RTI_INTERVAL(RTI_MS_DELTA);
          CLR_RTI_FLAG;
          ENABLE_RTI;
          heartbeat_count=0;
}

/******************************************************************************
Name:               msDelay()
Arguments:          int delay - the number of milliseconds to wait.
Returns:            void
Description:        This function will configure timer channel 2 to track the
                    time. The timer is reset for the number of ms is specified
******************************************************************************/
void msDelay(unsigned int delay) {
          unsigned int i,j;
          
          i=heartbeat_count;
          
          j=i+delay;
          
          for(;i!=j;){
                    i=heartbeat_count;
          }
}

/*****************************************************************************/
/*                                 Interrupt Handlers                        */
/*****************************************************************************/

/******************************************************************************
Name:               Heartbeat_Init()
Arguments:          void
Returns:            void
Description:        This function sets up the registers needed for the RTI
                    module to start running at a 1 ms time interval and
                    initializes the count to zero.
******************************************************************************/
interrupt 7 void Vrti_Handler(void) {
          CLR_RTI_FLAG;
          ++heartbeat_count;
}