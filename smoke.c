
#include<avr/io.h>

#define	F_CPU	1000000

#include<util/delay.h>

#define		LCD_DATA_PORT		PORTB

#define 	LCD_CONT_PORT		PORTD

#define 	LCD_RS 		PD0
#define 	LCD_RW 		PD1
#define 	LCD_EN 		PD2

#define		MQ2_SENSOR_REF		200

#include<stdlib.h>

//adc functions
void adc_init(void);
int read_adc_channel(unsigned char channel);

//lcd functions
void lcd_data_write(char data);
void lcd_command_write( char command);
void lcd_init();
void lcd_string_write( char *string);
void lcd_number_write(int number,unsigned char radix);


int main(void)
{
	DDRB=0xff;	//lcd data pins

	DDRD=0x07;	//lcd control pins
	
	DDRC=0b00000001;	//buzzer pin
	
	int mq2_smoke_sensor_output=0;

	lcd_init();

	adc_init();
	
	lcd_string_write("Smoke sensing");

	lcd_command_write(0xc0);	//go to next line
	
	lcd_string_write("L-3 T-1 Project");
	
	
	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	_delay_ms(500);
	

	while(1)
	{
		lcd_command_write(0x01);

		if(mq2_smoke_sensor_output > MQ2_SENSOR_REF)
		{
			lcd_string_write("High Smoke");
			
			PORTC=0b00000001;
		}
		else
		{
			lcd_string_write("MQ-2 Value:-");
			
			PORTC=0x00;
		}

		lcd_command_write(0xc0);

		mq2_smoke_sensor_output=read_adc_channel(0);

		lcd_number_write(mq2_smoke_sensor_output,10);
		

		_delay_ms(500);
		
	}
}



void adc_init(void)
{
	ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADATE)|(1<<ADPS2);
	SFIOR=0x00;
}

int read_adc_channel(unsigned char channel)
{
	int adc_value;
	unsigned char temp;
	ADMUX=(1<<REFS0)|channel;
	_delay_ms(1);
	temp=ADCL;
	adc_value=ADCH;
	adc_value=(adc_value<<8)|temp;
	return adc_value;
}


void lcd_data_write(char data)
{
	LCD_CONT_PORT=_BV(LCD_EN)|_BV(LCD_RS);
	LCD_DATA_PORT=data;
	_delay_ms(1);
	LCD_CONT_PORT=_BV(LCD_RS);
	_delay_ms(1);
}

void lcd_command_write(char command)
{
	LCD_CONT_PORT=_BV(LCD_EN);
	LCD_DATA_PORT=command;
	_delay_ms(1);
	LCD_CONT_PORT=0x00;
	_delay_ms(1);
}

void lcd_init()
{
	lcd_command_write(0x38);
	lcd_command_write(0x01);
	lcd_command_write(0x06);
	lcd_command_write(0x0e);
}

void lcd_string_write(char *string)
{
	while (*string)
	lcd_data_write(*string++);
}

void lcd_number_write(int number,unsigned char radix)
{
	char *number_string="00000";
	itoa(number,number_string,radix);
	lcd_string_write(number_string);
}
