//---------------------------------------------------------------------------
// Project: Experiment - 2 : Lab Assignment : Interrupts And Timers
//
// Date: 07/August/2016
//
//---------------------------------------------------------------------------


//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"


//----------------------------------------
// Defines
//----------------------------------------
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
char abc;
uint32_t ui32Period;


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
volatile uint32_t sw2status = 0;

//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
   ui32Period = (SysCtlClockGet() / 2);  // for 10ms delay


   hardware_init();							// init hardware via Xware

   while(1)									// forever loop
   {
	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4) || !GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
	   {
		   TimerEnable(TIMER0_BASE, TIMER_A);
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

	//Timer Initialisation
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER0_BASE, TIMER_A, ui32Period -1);
	TimerLoadSet(TIMER1_BASE, TIMER_A, ui32Period -1);
	TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER2_BASE, TIMER_A, ui32Period -1);

	//Timer Interrupt Enable
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER1A);
	IntEnable(INT_TIMER2A);
	TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	TimerIntEnable(TIMER2_BASE, TIMER_TIMA_TIMEOUT);
	IntMasterEnable();

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

//-----------------------------------------------------------------------------
// Detect Key Press -- Timer 0 Interrupt Handler
//-----------------------------------------------------------------------------

void detectKeyPress()
{

	//Timer Disable
	TimerDisable(TIMER0_BASE, TIMER_A);

	//Clear the timer interrupt
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	//Check switch is still pressed or not
	if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
	{
		TimerEnable(TIMER1_BASE, TIMER_A);
	}
	else if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
	{
		TimerEnable(TIMER2_BASE, TIMER_A);
	}
}

//----------------------------------------------------------------------------
// Detect Key Press Confirmation
//----------------------------------------------------------------------------

void detectKeyConfirm_sw2()
{
	//Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

	//Timer Disable
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
		{
		TimerDisable(TIMER1_BASE, TIMER_A);
		ledToggle_rgb();
		}
}

void detectKeyConfirm_sw1(void)
{
	//Clear the timer interrupt
	TimerIntClear(TIMER2_BASE, TIMER_TIMA_TIMEOUT);

	//Timer Disable
	if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
		{
		TimerDisable(TIMER2_BASE, TIMER_A);
		sw2status = sw2status + 1;
		}
}


