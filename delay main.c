void delay_ms(int dlyms)

{

	dlyms*=12000;

 	while(dlyms--);

}

void delay_us(int dlyus)

{

	dlyus*=12;

	while(dlyus--);

}

void delay_s(int dlys)

{

	dlys*=12000000;

	while(dlys--);

}
delay main.c
