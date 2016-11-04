
//------------------------------------------
// TivaWare Header Files
//------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include <time.h>


#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F (*((volatile unsigned long *)0x40025524))

//----------------------------------------
// Prototypes
//----------------------------------------
void hardware_init(void);
void Timer2IntHandler(void);
void delay(void);
void delayms(void);
void LCDInit(void);
void LCD_ClearScreen(void);
void LCD_Go2Line2(void);
void LCD_Go2Line1(void);
void LCD_WriteCommand(unsigned char cmd);
void LCD_WriteChar(unsigned char data);
void enable(void);
void LCD_WriteString(char * string);

volatile uint8_t switch1_press=0;
uint8_t a=48;
uint8_t b=48;
char  a0[11];
uint8_t i=0;
uint8_t c=48;
uint8_t d=48;
uint8_t e=48;
uint8_t f=48;
uint8_t g=48;
uint8_t h=48;
uint8_t k=49;
uint8_t l=65;

uint32_t ui32Period;

//---------------------------------------------------------------------------
// main()
//---------------------------------------------------------------------------
void main(void)
{

   hardware_init();							// init hardware via Xware
   delayms();
   delayms();
   LCDInit();

     delayms();/*
     LCD_WriteString(" DARSHIT ");
     LCD_Go2Line2();
     LCD_WriteString(" NISHANT ");*/
     LCD_ClearScreen();
     LCD_WriteChar(a);
     LCD_WriteChar(b);
     LCD_WriteString(".");

     LCD_WriteChar(c);
     LCD_WriteChar(d);
     LCD_WriteString(".");

     LCD_WriteChar(e);
     LCD_WriteChar(f);
     LCD_WriteString(".");

     LCD_WriteChar(g);
     LCD_WriteChar(h);
     LCD_WriteCommand(0x80);
     while(1)
     {
     //	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, 0x02);
     	if(!GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0))
     				{
     		delay();
     					switch1_press = switch1_press + 1;
     						if(switch1_press == 9)
     						{
     							switch1_press = 0;

     						}

     	switch(switch1_press)
     	{
     	case 1:
     		{
     			LCD_WriteCommand(0x14);break;
     		}
     	case 2:
     		{
     			LCD_WriteCommand(0x14);
     			LCD_WriteCommand(0x14);break;
     		}
     	case 3:
     		{

     			LCD_WriteCommand(0x14);break;
     		}
     	case 4:
     		{LCD_WriteCommand(0x14);
     			LCD_WriteCommand(0x14);break;
     		}
     	case 5:
     		{

 				LCD_WriteCommand(0x14);break;
     		}
     	case 6:
     		{LCD_WriteCommand(0x14);
     			LCD_WriteCommand(0x14);break;

     		}
     	case 7:
     		{
     			LCD_WriteCommand(0x14);break;
     		}
     	case 8:
         		{
         			LCD_WriteCommand(0x80);break;
         		}

     	case 9:
         		{
         			LCD_WriteCommand(0x80);break;
         		}

     	}
     	}
     	if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4))
     		   {
     		delay();
     		switch(switch1_press)
     		     	{
     		case 0:
     		{
     			a++;
     			if(a==58)
     			{
     				a=65;
     			}
     			if(a==71)
     			{
     				a=48;
     			}

     			LCD_WriteChar(a);
     			LCD_WriteCommand(0x10);
     			break;
     		}
     		case 1:
     		{
     		b++;
 			if(b==58)
 			{
 				b=65;
 			}
 			if(b==71)
 			{
 				b=48;
 			}

     		LCD_WriteChar(b);
     		LCD_WriteCommand(0x10);
     		break;
     		}
   		     	case 2:
     		     		{
     		     			c++;
     		     			if(c==58)
     		     			{
     		     				c=65;
     		     			}
     		     			if(c==71)
     		     			{
     		     				c=48;
     		     			}

     		     			LCD_WriteChar(c);
     		     				LCD_WriteCommand(0x10);
     		     				break;
     		     		}
     		     	case 3:
     		     		{
d++;
	if(d==58)
	{
		d=65;
	}
	if(d==71)
	{
		d=48;
	}
	LCD_WriteChar(d);
	LCD_WriteCommand(0x10);
	break;
     		     		}
     		     	case 4:
     		     		{
     		        		e++;
     		     			if(e==58)
     		     			{
     		     				e=65;
     		     			}
     		     			if(e==71)
     		     			{
     		     				e=48;
     		     			}
     		        		LCD_WriteChar(e);
     		        		LCD_WriteCommand(0x10);
     		        		break;

     		     		}
     		     	case 5:
     		     		{
     		        		f++;
     		     			if(f==58)
     		     			{
     		     				f=65;
     		     			}
     		     			if(f==71)
     		     			{
     		     				f=48;
     		     			}
     		        		LCD_WriteChar(f);
     		        		LCD_WriteCommand(0x10);
     		        		break;

     		     		}
     		     	case 6:
     		     		{
     		     			g++;
     		     			if(g==58)
     		     			{
     		     				g=65;
     		     			}
     		     			if(g==71)
     		     			{
     		     				g=48;
     		     			}
 		        		LCD_WriteChar(g);
 		        		LCD_WriteCommand(0x10);
 		        		break;

     		     		}
     		     	case 7:
     		     		{
     		        		h++;
     		     			if(h==58)
     		     			{
     		     				h=65;
     		     			}
     		     			if(h==71)
     		     			{
     		     				h=48;
     		     			}
     		        		LCD_WriteChar(h);
     		        		LCD_WriteCommand(0x10);
     		        		break;
     		     		}
     		     	case 8:
     		         		{
     		         			a++;
     		         			     			if(a==58)
     		         			     			{
     		         			     				a=65;
     		         			     			}
     		         			     			if(a==71)
     		         			     			{
     		         			     				a=48;
     		         			     			}

     		         			     			LCD_WriteChar(a);
     		         			     			LCD_WriteCommand(0x10);
     		         			     			break;
     		         		}

     		     	}


     		   }
i=0;
if(a==48)
	{
	a0[0]=(char) b;
	a0[1]='0';
    i=2;

	}else
	{
	a0[0]=(char) a;
	a0[1]=(char) b;
	a0[2]='0';
	i=3;
	}

if(c==48)
	{
	a0[i]=(char) d;
	i++;    a0[i]='0';
    i++;

	}else
	{
	a0[i]=(char) c;
	i++;
	a0[i]=(char) d;
i++;    a0[i]='0';
i++;

	}

if(e==48)
	{
	a0[i]=(char) f;
	i++;
	a0[i]='0';
i++;
	}else
	{
	a0[i]=(char) e;
	i++;
	a0[i]=(char) f;
    i++;
    a0[i]='0';
    i++;
	}

if(g==48)
	{
	a0[i]=(char) h;

	}else
	{
	a0[i]=(char) g;
	i++;
	a0[i]=(char) h;

	}




     }//end while


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


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);



	LOCK_F=0x4C4F434BU;
	CR_F=GPIO_PIN_0|GPIO_PIN_4;

	GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0|GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_0|GPIO_PIN_4,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

}




//---------------------------------------------------------------------------
// delay()
//
// Creates a 500ms delay via TivaWare fxn
//---------------------------------------------------------------------------
void delay(void)
{
	 SysCtlDelay(670000);		// creates ~50ms delay - TivaWare fxn

}
void delayms(void)
{
	 SysCtlDelay(67000);		// creates ~50ms delay - TivaWare fxn

}


void LCDInit(void)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0);//(c) Set R/W = 1/0 for read and write operation.
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);//(g) Make En = 0.

	delay();
	LCD_WriteCommand(0x38); // function set command/ 8-bit interface, 2 display lines, 5x7 font
	delay();
	LCD_WriteCommand(0x06); // input set command // entry mode set: increment automatically, no display shift
	delay();
	LCD_WriteCommand(0x0F); // display switch command / turn display on, cursor on, no blinking
	delay();
	LCD_WriteCommand(0x01); // clear screen command // clear display, set cursor position to zero
	delay();
	LCD_WriteCommand(0x80); // clear screen command // clear display, set cursor position to zero
	delay();
}


void LCD_WriteCommand(unsigned char cmd)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0);//(b) Set RS = 0 for command write to LCD.
	delay();
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, cmd);//(d) Put your data on data lines.
	enable();
}

void LCD_WriteChar(unsigned char data)
{
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0xFF);//(b) Set RS = 1 for data write to LCD.
	//delay();
	GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, data);//(d) Put your data on data lines.
	enable();
}


void enable(void)
{
	delay();
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0xFF);//(a) Set En = 1.
	delay();
	GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0);//(g) Make En = 0.
	delay();
}



void LCD_ClearScreen(void)
{
	LCD_WriteCommand(0x01); // clear screen command // clear display, set cursor position to zero
    delay();
	LCD_WriteCommand(0x80); // clear screen command // clear display, set cursor position to zero
}

void LCD_Go2Line1(void)
{
	LCD_WriteCommand(0x02); // clear screen command // clear display, set cursor position to zero
}


void LCD_Go2Line2(void)
{
	LCD_WriteCommand(0x80 + 0x40); // Set cursor to second line starting
}


void LCD_WriteString(char * string)
{
	int c=0;
	while (string[c]!='\0')
	{
		LCD_WriteChar(string[c]);
		c++;
	}
}

