/*
 * atmega_master_conf.h
 *
 * Created on: 02.07.2011 00:01:37
 *     Author: cadavre
 */

#ifndef ATMEGA_MASTER_CONF_H_
#define ATMEGA_MASTER_CONF_H_

// oznaczenia czujnik�w
#define SEN_C		(1<<PC0)	// centrum
#define SEN_L		(1<<PC1)	// lewy
#define SEN_R		(1<<PC2)	// prawy
#define SEN_LL		(1<<PC3)	// lewy brzegowy
#define SEN_RR		(1<<PC4)	// prawy brzegowy

// oznaczenia przycisk�w
#define BTN_START	(1<<PB0)	// start odkurzania
#define BTN_RESET	(1<<PB1)	// stop i reset odkurzania

// stany przycisk�w
#define BTN_OFF		0x00		// nic nie wci�ni�te
#define BTN_ON		0x01		// funkcyjny wci�ni�ty
#define BTN_L		0x01		// wci�ni�ty Lewy
#define BTN_R		0x02		// wci�ni�ty Prawy

// porty SPI
#define SS_CTRL		(1<<PB6)
#define SS			(1<<PB2)
#define MOSI		(1<<PB3)
#define MISO		(1<<PB4)
#define SCK			(1<<PB5)

// od�wie�anie
#define LCD_REFRESH_TICK		80
#define USART_REFRESH_TICK		252

// porty LCD skonfigurowane w lcd/lcd44780.h

// definicje stan�w flag
#define STATE_IDLE				0x00
#define STATE_RUNNING			0x01

#define MODE_STANDING			0x00	// stoi
#define MODE_PROGRESSING		0x01	// jazda w prz�d
#define MODE_REVERSING			0x02	// cofanie przy iteracji >8
#define MODE_BACK_TURNING		0x03	// obr�t

#endif /* ATMEGA_MASTER_CONF_H_ */