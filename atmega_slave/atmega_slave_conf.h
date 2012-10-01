/*
 * atmega_slave_conf.h
 *
 * Created on: 02.07.2011 02:11:02
 *     Author: cadavre
 */

#ifndef ATMEGA_SLAVE_CONF_H_
#define ATMEGA_SLAVE_CONF_H_

// porty SPI
#define MISO					(1<<PB4)

// krokowe: porty
#define MOTOR_PD				(1<<PD4)|(1<<PD5)|(1<<PD6)|(1<<PD7)
#define MOTOR_PC				(1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5)

// krokowe: cewki
#define M0_A1					(1<<PD4)
#define M0_B1					(1<<PD5)
#define M0_A2					(1<<PD6)
#define M0_B2					(1<<PD7)

#define M1_AP					(1<<PC2)
#define M1_BP					(1<<PC3)
#define M1_AM					(1<<PC4)
#define M1_BM					(1<<PC5)

// krokowe: kroki
#define M0_STEP1				PORTD |= M0_A1|M0_B1; PORTD &= ~(M0_A2|M0_B2)
#define M0_STEP2				PORTD |= M0_B1; PORTD &= ~(M0_A1|M0_A2|M0_B2)
#define M0_STEP3				PORTD |= M0_A2|M0_B1; PORTD &= ~(M0_A1|M0_B2)
#define M0_STEP4				PORTD |= M0_A2; PORTD &= ~(M0_A1|M0_B1|M0_B2)
#define M0_STEP5				PORTD |= M0_A2|M0_B2; PORTD &= ~(M0_A1|M0_B1)
#define M0_STEP6				PORTD |= M0_B2; PORTD &= ~(M0_A1|M0_A2|M0_B1)
#define M0_STEP7				PORTD |= M0_A1|M0_B2; PORTD &= ~(M0_A2|M0_B1)
#define M0_STEP8				PORTD |= M0_A1; PORTD &= ~(M0_A2|M0_B1|M0_B2)

#define M1_STEP1				PORTC |= M1_BP|M1_AM; PORTC &= ~(M1_AP|M1_BM)
#define M1_STEP2				PORTC |= M1_AP|M1_BP; PORTC &= ~(M1_AM|M1_BM)
#define M1_STEP3				PORTC |= M1_AP|M1_BM; PORTC &= ~(M1_AM|M1_BP)
#define M1_STEP4				PORTC |= M1_AM|M1_BM; PORTC &= ~(M1_AP|M1_BP)

/*#define M1_STEP1				PORTC |= M1_AP|M1_BM; PORTC &= ~(M1_AM|M1_BP)
#define M1_STEP2				PORTC |= M1_AP; PORTC &= ~(M1_AM|M1_BP|M1_BM)
#define M1_STEP3				PORTC |= M1_AP|M1_AM; PORTC &= ~(M1_BP|M1_BM)
#define M1_STEP4				PORTC |= M1_AM; PORTC &= ~(M1_AP|M1_BP|M1_BM)
#define M1_STEP5				PORTC |= M1_AM|M1_BP; PORTC &= ~(M1_AP|M1_BM)
#define M1_STEP6				PORTC |= M1_BP; PORTC &= ~(M1_AP|M1_AM|M1_BM)
#define M1_STEP7				PORTC |= M1_BP|M1_BM; PORTC &= ~(M1_AP|M1_AM)
#define M1_STEP8				PORTC |= M1_BM; PORTC &= ~(M1_AP|M1_AM|M1_BP)*/

// krokowce - specyfikacja
#define M_ROTATE_STEPS			48

// ograniczenie prêdkoœci krokowców
#define M0_SPD_L				0
#define M0_SPD_H				220
#define M1_SPD_L				0
#define M1_SPD_H				220

// oznaczenia czujników
#define SEN_L		(1<<PC0)	// lewy
#define SEN_C		(1<<PC1)	// centrum
#define SEN_R		(1<<PC2)	// prawy
#define SEN_LL		(1<<PC3)	// lewy brzegowy
#define SEN_RR		(1<<PC4)	// prawy brzegowy

// stany przycisków
#define BTN_OFF		0x00		// nic nie wciœniête
#define BTN_ON		0x01		// funkcyjny wciœniêty
#define BTN_L		0x01		// wciœniêty Lewy
#define BTN_R		0x02		// wciœniêty Prawy

// flaga reakcji na wciœniêcie
#define BTN_GET_ON	4
#define SENS_IDLE	3000		// czas zw³oki do nastêpnego pomiaru z sensorów (1-60000)

// definicje stanów flag
#define STATE_IDLE				0x00
#define STATE_RUNNING			0x01

#define MODE_STANDING			0x00	// stoi
#define MODE_PROGRESSING		0x01	// jazda w przód
#define MODE_REVERSING			0x02	// cofanie przy iteracji >8
#define MODE_BACK45_TURNING		0x03	// zwrot do ty³u o 45deg
#define MODE_BACK90_TURNING		0x04	// zwrot do ty³u o 90deg

// poruszanie siê odkurzacza
#define VACU_LENGTH						40		// d³. odkurzacza [cm]									TODO
#define VACU_DIST_PER_ROTATE			40		// dyst. na jeden pe³ny obrót [cm]						TODO

#endif /* ATMEGA_SLAVE_CONF_H_ */