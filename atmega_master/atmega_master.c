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
volatile uint8_t memory_state[6] = {0,0,0,0,0,0};

uint8_t separator_sign[] = {4,4,4,4,4,4,4,4};			// separator sign for lcd

volatile uint8_t refresh_flag = 0;
volatile uint8_t j = 0;

/*
 * Prototypes
 */
void lcd_drive(int value, char *str);
void lcd_refresh(void);
void lcd_welcome(void);
void lcd_debug(void);
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
 * Initialization of input ports for sensors
 */
void sensors_init(void) {
	DDRC &= ~(SEN_L|SEN_C|SEN_R|SEN_LL|SEN_RR);
	PORTC |= SEN_L|SEN_C|SEN_R|SEN_LL|SEN_RR;
}

/*
 * Initialization of input ports for control buttons
 */
void controls_init(void) {
	DDRB &= ~(BTN_START|BTN_RESET);
	PORTB |= BTN_START|BTN_RESET;
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
	memory_state[j] = SPI_transfer(btn_state[j]);
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
	controls_init();
	sensors_init();
	Timer2_init();
	SPI_init();
	USART_init(__UBRR);
	OSCCAL = 152; // oscilator calibration for USART communication
	sei();
	lcd_init();
	lcd_defchar(0x80, separator_sign);
	lcd_welcome();
	while(1)
	{
		if (!(PINB&BTN_START)) {
			btn_state[5] = BTN_L;
		} else if (!(PINB&BTN_RESET)) {
			btn_state[5] = BTN_R;
		} else {
			btn_state[5] = BTN_OFF;
		}

		if (!(PINC&SEN_C)) {
			btn_state[0] = BTN_ON;
		} else if (!(PINC&SEN_L)) {
			btn_state[1] = BTN_ON;
		} else if (!(PINC&SEN_R)) {
			// no sensor
			btn_state[2] = 0;
		} else if (!(PINC&SEN_LL)) {
			// no sensor
			btn_state[3] = 0;
		} else if (!(PINC&SEN_RR)) {
			// no sensor
			btn_state[4] = 0;
		} else {
			for( int k=0; k<5; k++ ) {
				btn_state[k] = BTN_OFF;
			}
		}

		if ( (refresh_flag % LCD_REFRESH_TICK) == 0 ) {
			if ( memory_state[0] == STATE_RUNNING ) {
				lcd_refresh();
				//lcd_debug();
			} else {
				lcd_welcome();
				//lcd_debug();
			}
		}
		if ( (refresh_flag % USART_REFRESH_TICK) == 0 ) {
			// USART_send_report();
		}
		if (refresh_flag==255) {
			refresh_flag = 0;
		}
	}
	return 0;
}
/********************************************** END OF MAIN **********************************************/

/*
 * Show welcome message on boot
 */
void lcd_welcome(void) {
	lcd_cls();
	lcd_locate(0,2);
	lcd_str("Mechatronika");
	lcd_locate(1,3);
	lcd_str("AiR, gr.IV");
}

void lcd_debug(void) {
	lcd_cls();
	lcd_locate(0,0);
	lcd_int(btn_state[0] == 1);
	lcd_locate(0,1);
	lcd_int(btn_state[1] == 1);
	lcd_locate(0,2);
	lcd_int(btn_state[2] == 1);
	lcd_locate(0,3);
	lcd_int(btn_state[3] == 1);
	lcd_locate(0,4);
	lcd_int(btn_state[4] == 1);
	lcd_locate(0,5);
	lcd_int(btn_state[5] == BTN_L ? 1 : btn_state[5] == BTN_R ? 2 : 0);
	lcd_locate(0,7);
	lcd_str("ST");
	lcd_locate(0,9);
	lcd_int(memory_state[0]);
	lcd_locate(0,11);
	lcd_str("MO");
	lcd_locate(0,13);
	lcd_int(memory_state[1]);

	lcd_locate(1,0);
	lcd_int(memory_state[2]);
	lcd_locate(1,3);
	lcd_int(memory_state[3]);
	lcd_locate(1,6);
	lcd_int(memory_state[4]);
	lcd_locate(1,9);
	lcd_int(memory_state[5]);
}

/*
 * Refresh LCD display
 */
void lcd_refresh(void) {
	lcd_cls();

	// ON info
	lcd_locate(0,0);
	lcd_str("ON");
	// MODE name
	lcd_locate(0,3);
	if (memory_state[1] == MODE_PROGRESSING) {
		lcd_str("PROGRESSING");
	} else if (memory_state[1] == MODE_REVERSING) {
		lcd_str("REVERSING");
	} else if (memory_state[1] == MODE_BACK_TURNING) {
		lcd_str("TURNING");
	}
	// ITERATION number
	lcd_locate(1,0);
	lcd_str("ITE");
	lcd_locate(1,3);
	lcd_int(memory_state[3]);
	// COURSE number
	lcd_locate(1,7);
	lcd_str("COU");
	lcd_locate(1,10);
	lcd_int(memory_state[2]);
	// BUTTON indicator
	lcd_locate(0,15);
	if (btn_state[5] != 0) {
		lcd_str("B");
	}
	// SENSOR indicator
	lcd_locate(1,15);
	if (btn_state[0] != 0) {
		lcd_str("C");
	} else if (btn_state[1] != 0) {
		lcd_str("L");
	}
}

/*
 * Send drive states report on RS232
 */
void USART_send_report(void) {
	USART_string("\r\n#### Report ####");
}