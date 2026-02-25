#include<lpc214x.h>
#include "lcd.h"
#include "delay.h"
#include "uart0.h"
#include "gsm.h"
#include "adc_defines.h"
#include "adc.h"
#include "i2c.h"
#include "defines.h"
unsigned char keyscan(void);																

void Backspace(void);

u8 temp_set,cur_temp;
f32 temp;
u8 i_flag=0;

void edit_set_temp(void)
{
   unsigned char ch,i;
   u32 var=0;
   while(1)
   {
   	LCD_cmd(0x80);
   	LCD_str("edit set temp");
   	for(i=0;i<4;i++)
   	{
   	ch=keyscan();
   		if(ch>='0' && ch<='9')
   		{
   			var=var*10+(ch-48);
   			LCD_char(ch);
   		}
		if(ch=='A')
		{
           i--;
           Backspace();
           i--;
           var/=10;
        }
        if(ch=='B')
		{
	      i2cDevWrite(0xa0,0x0010,var);
		  break;
		}
		if(ch=='C')
		   break;
      }

	  }
}


void eint0_isr(void) __irq
{
	i_flag=1;
	SSETBIT(EXTINT,0);//clear flag
	VICVectAddr=0;//dummy write;
}	

void Enable_EINT0(void)
{
	CFGPIN(PINSEL1,0,FUNC2);
	//CFGPIN(PINSEL0,0,FUNC4);
	//SETBIT(IODIR0,EINT0_LED);
	SSETBIT(VICIntEnable,14);
	VICVectCntl0=0x20|14;
	VICVectAddr0=(unsigned)eint0_isr;
	
	//Enable EINT 0 
	SSETBIT(EXTINT,0); //default
	
	//EINT0 as EDGE_TRIG
    SETBIT(EXTMODE,0);
	//EINT0 as REDGE
  //SETBIT(EXTPOLAR,0);	
}

int main()
{
	Enable_EINT0();
	LCD_Init( );
	LCD_str("I.F.I.S.S.A");
	InitUART0();
	GSM_init();
	Init_ADC();
	
	
	while(1)
	{
		LCD_cmd(0x01);
		LCD_cmd(0x80);
		LCD_str("C.T: ");
		temp=Read_ADC(CH1);
	    cur_temp=(int)(temp*100);
		U32LCD(cur_temp);
		temp_set = i2cDevRead(0xa0,0x0010);
		LCD_cmd(0xC0);
		LCD_str("S.P: ");
		U32LCD(temp_set);
		delay_ms(1000);
		if(cur_temp>temp_set)
		{
					send_sms("6309162622", "temperature crossed setpoint value");
		}
		if(i_flag==1)
		{
		i_flag=0;
		edit_set_temp();
		temp_set = i2cDevRead(0xa0,0x0010);
		}
	 }
}


