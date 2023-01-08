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

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include "definitions.h"                // SYS function prototypes
#include <string.h>                     // ** NEW LINE **

#define ADC_VREF                (3.3f)
#define DAC_COUNT_INCREMENT     (31U)  // equivalent to 0.1V(0.1 / (3.3 / ((2^10) - 1))) 
#define DAC_COUNT_MAX           (1023U)

uint16_t adc_count;

char PIN[10];               //  ** NEW LINE **
char A[] = "ADC_AIN[4]";    //  ** NEW LINE **
char B[] = "ADC_AIN[10]";   //  ** NEW LINE **
uint16_t pot_count;         //  ** NEW LINE **
float pot_input_voltage;    //  ** NEW LINE **
float high;                 //  ** NEW LINE **

float input_voltage;
/* Initial value of dac count which is midpoint = 1.65 V*/
uint16_t dac_count = 0x200;   

void switch_handler(uintptr_t context )
{
    /* Write next data sample */
    dac_count = dac_count + DAC_COUNT_INCREMENT;
    
    if (dac_count > DAC_COUNT_MAX)
            dac_count=0;    
    
    DAC_DataWrite(dac_count);
}

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    
    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    ADC Sample Demo                 ");
    printf("\n\r---------------------------------------------------------\n\r");
    
    ADC0_Enable();
    ADC1_Enable();  //  ** NEW LINE **
    SYSTICK_TimerStart();
    EIC_CallbackRegister(EIC_PIN_6, switch_handler, (uintptr_t) NULL);
    DAC_DataWrite(dac_count);
    while (1)
    {
        /* Start ADC conversion */
        ADC0_ConversionStart();
        
        
        /* Wait till ADC conversion result is available */
        while(!ADC0_ConversionStatusGet())
        {};
               
        /* Read the ADC result */
        adc_count = ADC0_ConversionResultGet();
        input_voltage = (float)adc_count * ADC_VREF / 4095U;

// ************************   NEW SECTION  *************************************
        
        ADC1_ConversionStart(); 
        
        while(!ADC1_ConversionStatusGet()) 
        {};
        
        pot_count = ADC1_ConversionResultGet(); 
        pot_input_voltage = (float) pot_count * ADC_VREF / 4095U;
        
        printf("---------------------------------------------------------\r\n");
        printf("ADC_AIN[4] - DAC Input Voltage = %d.%02d V\n \r", (int)input_voltage, (int)((input_voltage - (int)input_voltage)*100.0));
        printf("ADC_AIN[10] - POT Input Voltage = %d.%02d V\n \n \r", (int)pot_input_voltage, (int)((pot_input_voltage - (int)pot_input_voltage)*100.0));
        
        if (input_voltage > pot_input_voltage )
        {
            high = input_voltage;
            strcpy (PIN, A);
        }
        else
        {
           high = pot_input_voltage;
           strcpy (PIN, B);
        }
        
        printf ("Higher Voltage is on %s measuring %d.%02d \n \n \r", PIN, (int)high, (int)((high - (int)high)*100.0));
        
        
// ***************************************************************************** 
        
        SYSTICK_DelayMs(1000);
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

