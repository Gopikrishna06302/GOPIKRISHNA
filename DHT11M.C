#include <lpc214x.h>

#include "delay.h"
#include "defines.h"
#include "lcd.h"
#include "dht11.h"

int main (void)
{
	unsigned char humidity_integer, humidity_decimal, temp_integer, temp_decimal, checksum;  
	LCD_Init();

	while(1)
	{
		dht11_request();
		dht11_response();
		humidity_integer = dht11_data();
		humidity_decimal = dht11_data();
		temp_integer = dht11_data();
		temp_decimal = dht11_data();
		checksum = dht11_data();
		if( (humidity_integer + humidity_decimal + temp_integer + temp_decimal) != checksum )
			Write_str_LCD("Checksum Error\r\n");
		else
		{
			Write_CMD_LCD(0x80);
			Write_str_LCD("H : ");
			Write_int_LCD(humidity_integer);
			Write_DAT_LCD('.');
			Write_int_LCD(humidity_decimal);
			Write_str_LCD(" % RH ");
			Write_CMD_LCD(0xC0);
			Write_str_LCD("T : ");
			Write_int_LCD(temp_integer);
			Write_DAT_LCD('.');
			Write_int_LCD(temp_decimal);
			Write_DAT_LCD(223);
			Write_str_LCD("C");
			Write_CMD_LCD(0xD4);
			Write_str_LCD("Checksum : ");
			Write_int_LCD(checksum);
			delay_ms(3000);
			Write_CMD_LCD(0x01);
		}
	}
}
