//---------------------------------------------------------------------------
// Project: Experiment - 1 : Home Assignment : LED_TOGGLE_PLUS_SWITCH_CASE
//
// Date: 17/January/2016
//
//---------------------------------------------------------------------------


//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>

//----------------------------------------
// Defines
//----------------------------------------
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
char abc;

//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void ledToggle(void);
void ledToggle_rgb(void);
void delay(char abc);


//---------------------------------------
// Globals
//---------------------------------------
volatile int16_t i16ToggleCount = 0;


//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{

   hardware_init();							// init hardware via Xware
   abc='A';
   int i=0;
   while(1)									// forever loop
   {
	   if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
	   {
			while(1)
			{
			   ledToggle();
			   delay(abc);
			       if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) && i==0)
			   	   {
			    	   SysCtlDelay(26800000);
			   		   abc='A';
			   		   i=1;
			   	   }
			   	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) && i==1)
			   	   {
			   		   SysCtlDelay(26800000);
			   	   		   abc='B';
			   	   		   i=2;
			   	   }
			   	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) && i==2)
			   	   {
			   		   SysCtlDelay(26800000);
			   	   		   abc='C';
			   	   		   i=0;
			   	   }
			   if(~GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
				   {break;}
			}
	   }
	   else
	   {
		ledToggle_rgb();

		delay('A');								// create a delay of ~1/2sec

		i16ToggleCount += 1;					// keep track of #toggles
	   }
   }

}


//---------------------------------------------------------------------------
// hardware_init()
//
// inits GPIO pins for toggling the LED
//---------------------------------------------------------------------------
void hardware_init(void)
{
	//Set CPU Clock to 40MHz. 400MHz PLL/2 = 200 DIV 5 = 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	// ADD Tiva-C GPIO setup - enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	//Removing Locks From Switch
			LOCK_F=0x4C4F434B;
			CR_F=GPIO_PIN_0|GPIO_PIN_4;

	// Turn on the LED
	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4);

		//Switch Padding
		GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

}


//---------------------------------------------------------------------------
// ledToggle()
//
// toggles LED on Tiva-C LaunchPad
//---------------------------------------------------------------------------
void ledToggle_rgb(void)
{
	// LED values - 2=RED, 4=BLUE, 8=GREEN
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_1))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 8);
		}
		else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 2);
		}
}

void ledToggle(void)
{
	// LED values - 2=RED, 4=BLUE, 8=GREEN
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_2))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 4);
		}
}



//---------------------------------------------------------------------------
// delay()
//
// Creates a 500ms delay via TivaWare fxn
//---------------------------------------------------------------------------
void delay(char var)
{
	 switch(var) {
	       case 'A' :
	    	  SysCtlDelay(6700000);		// creates ~500ms delay - TivaWare fxn
	          break;
	       case 'B' :
	    	  SysCtlDelay(13400000);		// creates ~1s delay - TivaWare fxn
	    	  break;
	       default :
	    	  SysCtlDelay(26800000);		// creates ~2s delay - TivaWare fxn
	          break;

	    }

}




