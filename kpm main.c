#include<lpc21xx.h>

#include "types_project.h"

#include "delay_header.h"

#include "kpm_defines_project.h"

#include "read_write_defines_project.h"

#include "lcd_func_declarations_project.h"

#include "lcd_defines_project.h"

u32 keyv=0;

u32 kpmLUT[4][4]={

	{'7','8','9','/'},

	{'4','5','6','*'},

	{'1','2','3','-'},

  {'c','0','=','+'}

};

void InitKpm(void)

{

	//cfg row lines as out pins

	WRITENIBBLE(IODIR1,row0,0xf);

	//leave col lines as input lines

}

u32 ColScan(void)

{

	u32 t;

	t=READNIBBLE(IOPIN1,col0);

	return (t<15)?0:1;

}

u32 RowCheck(void)

{

	u32 r;

	//ground one row at a time

	for(r=0;r<=3;r++)

	{

	WRITENIBBLE(IOPIN1,row0,~(1<<r));

	//delay_ms(500);

	if(ColScan()==0)

	{

		break;

	}

	}

	WRITENIBBLE(IOPIN1,row0,0);

	return r;

}


u32 ColCheck(void)

{

	u32 c;

	for(c=0;c<=3;c++)

	{

		if(READBIT(IOPIN1,(col0+c))==0)

		{

			break;

		}

	}

	return c;

}	


int KeyScan(void)

{

	u32 r,c;  //keyv means key value

	while(ColScan());

	//proceed to identify row in which key was pressed

	r=RowCheck();

	//proceed to identify col in which key was pressed

	c=ColCheck();

	//extract the key value from look out table(LUT)

	keyv=kpmLUT[r][c];

	return keyv;

}


//clear the string if password is wrong

void ClearStr(s8 *str,s32 cnt)

{

	int i;

	for(i=0;i<(cnt);i++)

	{

		str[i]=' ';

	}

}


//to read as string from lcd

void ReadStr(void)

{

	char ch;

	char str[10]="\0";

	char str1[10]="\0";

	char pass[6]="123456";

	int cnt=0;

	int i;

	while(1)

	{

		CmdLCD(GOTO_LINE1_POS0);

		StrLCD("ENTER THE PASS");

		ch=KeyScan();

		if(ch>='0' && ch<='9')

		{

			str[cnt]=ch;

			str1[cnt]='*';

			cnt++;

			CmdLCD(GOTO_LINE2_POS0);

		  //StrLCD(str);

			StrLCD(str1);

			if(cnt>6)

			{

				CmdLCD(GOTO_LINE2_POS0);

				StrLCD("MIN 6 DIGITS");

			}

      while(ColScan()==0);

		}

		else if(ch=='=')

		{

			for(i=0;i<6;i++)

			{

				if(str[i]!=pass[i])

				{

					CmdLCD(GOTO_LINE2_POS0);

					StrLCD("WRONG PASSWORD");

					delay_ms(500);

					CmdLCD(CLEAR_LCD);

					//ClearStr(str,cnt);

					ClearStr(str1,cnt);

					cnt=0;

					break;

				}

			}

			if(cnt!=0)

			{

				//ClearStr(str,cnt);

				ClearStr(str1,cnt);

				CmdLCD(CLEAR_LCD);

			  break;

			}

		}

		if(ch=='-')

		{

			if(cnt>6)

			{

				CmdLCD(GOTO_LINE2_POS0);

				StrLCD("                 ");

			}

			if(cnt>0)

			{

			str[cnt-1]=' ';

			str1[cnt-1]=' ';	

			CmdLCD(GOTO_LINE2_POS0);

			//StrLCD(str);

			StrLCD(str1);

		  cnt--;

			}				

			while(ColScan()==0);

		}

	}	

}
kpm main.c
