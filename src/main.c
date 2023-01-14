/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                  
#include <stdbool.h>                    
#include <stdlib.h>
#include <stdio.h>                       
#include "definitions.h"               


#define VREFA                (3.3f)

uint16_t raw_data;
float voltage;


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    
    ADC0_Enable();
    
    
    while ( true )
    {
        //   ADC0_ConversionStart();
        raw_data = ADC0_ConversionResultGet();
        voltage = (float)raw_data * VREFA / 4095U;
                 
   if (voltage > 2.9 && voltage < 3.1)
       {
            BLED_Set();
            GLED_Clear();
            OLED_Clear();
       }
   
        else if (voltage > 2.4 && voltage < 2.6)
        {
            GLED_Set();
            OLED_Clear();
            BLED_Clear();
        }
        else if (voltage > 0.9 && voltage < 1.1)
        {
            OLED_Set();
            GLED_Clear();
            BLED_Clear();
        }
        
   
        printf("Input Voltage = %d.%02d V\r", (int)voltage, (int)((voltage - (int)voltage)*100.0));
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

