/*
 * atmega_master_conf.h
 *
 * Created on: 02.07.2011 00:01:37
 *     Author: cadavre
 */

#ifndef ATMEGA_MASTER_CONF_H_
#define ATMEGA_MASTER_CONF_H_

// oznaczenia czujników
#define SEN_L		0			// lewy
#define SEN_C		1			// centrum
#define SEN_R		2			// prawy
#define SEN_LL		3			// lewy brzegowy
#define SEN_RR		4			// prawy brzegowy

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

#endif /* ATMEGA_MASTER_CONF_H_ */