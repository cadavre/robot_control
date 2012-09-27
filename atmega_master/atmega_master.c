/*
 * atmega_master.c
 *
 * Created on: 26.06.2011
 *     Author: Seweryn 'cadavre' Zeman
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "atmega_master_conf.h"
#include "lcd/lcd44780.h"
#include "usart/usart.h"

volatile uint8_t btn_state[6] = {BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF};
volatile uint8_t drive_state[6] = {0,0,0,0,0,0};

uint8_t deg_sign[] = {6,9,9,6,0,0,0,0};
volatile char btn_sign;

volatile uint8_t refresh_flag = 0;
volatile uint8_t i = 0;
volatile uint8_t j = 0;

/*
 * Prototypes
 */
void lcd_drive(int value, char *str);
void lcd_refresh(void);
void USART_send_report(void);

/*
 * Timer2 initialization
 */
void Timer2_init(void) {
	TCCR2 |= (1<<WGM21);						// CTC
	TCCR2 |= (1<<CS21)|(1<<CS22);				// prescaler 256
	OCR2 = 0x24;								// 0x24 = 36 = ~1.15ms
	TIMSK |= (1<<OCIE2);						// compare match IRQ
}

/*
 * SPI transfer interface initialization
 */
void SPI_init(void) {
	DDRB |= MOSI|SCK|SS|SS_CTRL;				// master out
	PORTB |= SS;								// SS=H
	SPCR |= (1<<SPE)|(1<<MSTR);					// no-prescaler
}

/*
 * Set SS line in transfer mode
 */
void SPI_SS_low(void) {
	PORTB &= ~SS_CTRL;
}

/*
 * Set SS line idle
 */
void SPI_SS_high(void) {
	PORTB |= SS_CTRL;
}

/*
 * SPI transfer execution
 */
uint8_t SPI_transfer(uint8_t byte) {
	SPDR = byte;
	while( !(SPSR & (1<<SPIF)) );
	return SPDR;
}

/*
 * Timer2 interrupt handle
 * - Makes SPI connection and transfers data
 */
ISR(TIMER2_COMP_vect) {
	SPI_SS_low();
	drive_state[j] = SPI_transfer(btn_state[j]);
	SPI_SS_high();
	if(j<5) { // send 6 bytes
		j++;
	} else {
		j = 0;
	}
	refresh_flag++;
}

/************************************************* MAIN *************************************************/
int main(void)
{
	Timer2_init();
	SPI_init();
	USART_init(__UBRR);
	OSCCAL = 152; // oscilator calibration for USART communication
	sei();
	lcd_init();
	lcd_defchar(0x80, deg_sign);
	while(1)
	{
		// TODO obs³uga klawiszy i przypisanie ich do rejestru stanu przycisków
		if ( (refresh_flag % LCD_REFRESH_TICK) == 0 ) {
			lcd_refresh();
		}
		if ( (refresh_flag % USART_REFRESH_TICK) == 0 ) {
			USART_send_report();
		}
		if (refresh_flag==255) {
			refresh_flag = 0;
		}
	}
	return 0;
}
/********************************************** END OF MAIN **********************************************/

/*
 * Refresh LCD display
 */
void lcd_refresh(void) {
	lcd_cls();

	// J1
	lcd_locate(0,0);
	lcd_str("1:");
	lcd_locate(0,2);
	lcd_int(drive_state[0]);
	lcd_locate(0,5);
	lcd_str("\x80");
	// J2
	lcd_locate(0,6);
	lcd_str("2:");
	lcd_locate(0,8);
	lcd_int(drive_state[1]);
	lcd_locate(0,11);
	lcd_str("\x80");
	// D3
	lcd_locate(1,0);
	lcd_str("3:");
	lcd_locate(1,2);
	lcd_int(drive_state[2]);
	// J4
	lcd_locate(1,6);
	lcd_str("4:");
	lcd_locate(1,8);
	lcd_int(drive_state[3]);
	lcd_locate(1,11);
	lcd_str("\x80");

	lcd_locate(0,12);
	lcd_str("SPD");
	lcd_locate(1,12);
	lcd_int( 20+((drive_state[5]*100)/255) );	// +20 dla prêdkoœci 20-100% bo pomiar wychodzi 0-80%
	lcd_locate(1,15);
	lcd_char('%');

	/*
	lcd_locate(0,0);
	btn_sign = (btn_state[0]==BTN_L)  ? '-'  : ( (btn_state[0]==BTN_R) ? '+' : ' ' ) ;
	lcd_char(btn_sign);
	lcd_locate(0,5);
	btn_sign = (btn_state[1]==BTN_L)  ? '-'  : ( (btn_state[1]==BTN_R) ? '+' : ' ' ) ;
	lcd_char(btn_sign);
	lcd_locate(1,0);
	btn_sign = (btn_state[2]==BTN_L)  ? '-'  : ( (btn_state[2]==BTN_R) ? '+' : ' ' ) ;
	lcd_char(btn_sign);
	lcd_locate(1,5);
	btn_sign = (btn_state[3]==BTN_L)  ? '-'  : ( (btn_state[3]==BTN_R) ? '+' : ' ' ) ;
	lcd_char(btn_sign);

	lcd_locate(0,15);
	lcd_int(btn_state[4]);
	lcd_locate(1,15);
	lcd_int(btn_state[5]);
	*/
}

/*
 * Send drive states report on RS232
 */
void USART_send_report(void) {
	USART_string("\r\n#### Report ####");
	USART_string("\r\nDrive J1: ");
	USART_int(drive_state[0]);
	USART_string("\r\nDrive J2: ");
	USART_int(drive_state[1]);
	USART_string("\r\nDrive D3: ");
	USART_int(drive_state[2]);
	USART_string("\r\nDrive J4: ");
	USART_int(drive_state[3]);
	USART_string("\r\nSpeed: ");
	USART_int(drive_state[5]);
}