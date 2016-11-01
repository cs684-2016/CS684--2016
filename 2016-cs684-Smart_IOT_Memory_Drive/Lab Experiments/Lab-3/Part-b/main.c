//---------------------------------------------------------------------------
// Project: Experiment - 3 : Lab Assignment : PWM And Servo Motor
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
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"


//----------------------------------------
// Defines
//----------------------------------------
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))
#define PWM_FREQUENCY 55
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
volatile uint32_t ui32Load;
volatile uint32_t ui32PWMClock;
volatile uint8_t ui8Adjust;


//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{
   ui32Period = (SysCtlClockGet() / 2);  // for 10ms delay
   ui8Adjust = 83;
   ui32PWMClock = SysCtlClockGet() / 64;
   ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;


   hardware_init();							// init hardware via Xware

   while(1)									// forever loop
   {
	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
	   {
		   TimerEnable(TIMER0_BASE, TIMER_A);

	   }

	   if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0))
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
	//SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);

	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	// ADD Tiva-C GPIO setup - enables port, sets pins 1-3 (RGB) pins for output
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

	//Make Port D pin 0 as PWM pin
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinConfigure(GPIO_PD0_M1PWM0);

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

	//PWM Configuration
	PWMGenConfigure(PWM1_BASE, PWM_GEN_0, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, ui32Load);

	//PWM Generator Enable
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
	PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_0);

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
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 10);
		}
		else if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_3))
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 3);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 1);
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
		ui8Adjust = ui8Adjust + 10;
				   if (ui8Adjust > 111)
				   {
				   ui8Adjust = 111;
				   }
				   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
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
		ui8Adjust = ui8Adjust - 10;
				   if (ui8Adjust < 56)
				   {
				   ui8Adjust = 56;
				   }
				   PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, ui8Adjust * ui32Load / 360);
		}
}


