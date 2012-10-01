/*
 * atmega_master_conf.h
 *
 * Created on: 02.07.2011 00:01:37
 *     Author: cadavre
 */

#ifndef ATMEGA_MASTER_CONF_H_
#define ATMEGA_MASTER_CONF_H_

// oznaczenia czujników
#define SEN_C		(1<<PC0)	// centrum
#define SEN_L		(1<<PC1)	// lewy
#define SEN_R		(1<<PC2)	// prawy
#define SEN_LL		(1<<PC3)	// lewy brzegowy
#define SEN_RR		(1<<PC4)	// prawy brzegowy

// oznaczenia przycisków
#define BTN_START	(1<<PB0)	// start odkurzania
#define BTN_RESET	(1<<PB1)	// stop i reset odkurzania

// stany przycisków
#define BTN_OFF		0x00		// nic nie wciœniête
#define BTN_ON		0x01		// funkcyjny wciœniêty
#define BTN_L		0x01		// wciœniêty Lewy
#define BTN_R		0x02		// wciœniêty Prawy

// porty SPI
#define SS_CTRL		(1<<PB6)
#define SS			(1<<PB2)
#define MOSI		(1<<PB3)
#define MISO		(1<<PB4)
#define SCK			(1<<PB5)

// odœwie¿anie
#define LCD_REFRESH_TICK		80
#define USART_REFRESH_TICK		252

// porty LCD skonfigurowane w lcd/lcd44780.h

// definicje stanów flag
#define STATE_IDLE				0x00
#define STATE_RUNNING			0x01

#define MODE_STANDING			0x00	// stoi
#define MODE_PROGRESSING		0x01	// jazda w przód
#define MODE_REVERSING			0x02	// cofanie przy iteracji >8
#define MODE_BACK_TURNING		0x03	// obrót

#endif /* ATMEGA_MASTER_CONF_H_ */