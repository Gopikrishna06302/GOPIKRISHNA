#include<lpc21xx.h>

#include"types_project.h"

void CfgPortPin(u32 PortNo,s32 pinN0,u32 pinFunc)

{

	if(PortNo==0)

	{

		if((pinN0>=(0))&&(pinN0<=15))

		{

			PINSEL0|=(((PINSEL0&~(3<<((pinN0)*2)))|(pinFunc<<((pinN0)*2))));

		}

		else if(pinN0>15)

		{

			PINSEL1|=(((PINSEL1&~(3<<((pinN0-16)*2)))|(pinFunc<<((pinN0-16)*2))));

		}

	}

	//generally no need to select the function from port1 all of those are GPIO PINS ONLY

	

}pin connect block.c
