#include<lpc21xx.h>

#include "types_project.h"

#include "uart_defines_project.h"

//#include "UART_decralarations.h"

#include "pin_connect_dclarations_project.h"


void Init_UART0(void)

{

//cfg p0.0 and p0.1 as txd0 and RxD0 pins

CfgPortPin(0,0, TxD0_PIN);

CfgPortPin(0,1,RxD0_PIN);

U0LCR=1<<DLAB_BIT|(WORLD_LEN_SEL);

U0DLL=DIVISOR;

U0DLM=DIVISOR>>8;

U0LCR&=~(1<<DLAB_BIT);

}


void U0_TXBYTE(u8 sByte)

{

U0THR=sByte;

while(((U0LSR>>TEMT_BIT)&1)==0);

}




u8 U0_RXBYTE(void)

{

while(((U0LSR>>DR_BIT)&1)==0);

return U0RBR;

}


void U0_TXStr(s8 *s)

{

while(*s)

{

U0_TXBYTE(*s++);

}

}


void U0_TXU32(u32 n)

{

s32 i=0;

u8 a[10];

if(n==0)

{

U0_TXBYTE('0');

}

else

{

while(n>0)

{

a[i]=(n%10)+48;

n/=10;

i++;

}

for(--i;i>=0;i--)

{

U0_TXBYTE(a[i]);

}

}

}


s8 * U0_RXStr(void)

{

static s8 rBuff[50];

s32 i=0;

while(1)

{

rBuff[i]=U0_RXBYTE();

U0_TXBYTE(rBuff[i]);

if(rBuff[i]=='\n')

{

rBuff[i]='\0';

break;

}

i++;

}

return rBuff;

}
uart main.c
