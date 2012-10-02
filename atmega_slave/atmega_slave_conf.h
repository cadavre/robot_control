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
#define M0_AP					(1<<PD4)
#define M0_BP					(1<<PD5)
#define M0_AM					(1<<PD6)
#define M0_BM					(1<<PD7)

#define M1_AP					(1<<PC2)
#define M1_BP					(1<<PC3)
#define M1_AM					(1<<PC4)
#define M1_BM					(1<<PC5)

// krokowe: kroki
#define M0_STEP1				PORTD |= M0_BP|M0_AM; PORTD &= ~(M0_AP|M0_BM)
#define M0_STEP2				PORTD |= M0_AP|M0_BP; PORTD &= ~(M0_AM|M0_BM)
#define M0_STEP3				PORTD |= M0_AP|M0_BM; PORTD &= ~(M0_AM|M0_BP)
#define M0_STEP4				PORTD |= M0_AM|M0_BM; PORTD &= ~(M0_AP|M0_BP)

#define M1_STEP1				PORTC |= M1_BP|M1_AM; PORTC &= ~(M1_AP|M1_BM)
#define M1_STEP2				PORTC |= M1_AP|M1_BP; PORTC &= ~(M1_AM|M1_BM)
#define M1_STEP3				PORTC |= M1_AP|M1_BM; PORTC &= ~(M1_AM|M1_BP)
#define M1_STEP4				PORTC |= M1_AM|M1_BM; PORTC &= ~(M1_AP|M1_BP)

// krokowce - specyfikacja
#define M_ROTATE_STEPS			48

// ograniczenie pr�dko�ci krokowc�w
#define M0_SPD_L				0
#define M0_SPD_H				220
#define M1_SPD_L				0
#define M1_SPD_H				220

// oznaczenia czujnik�w
#define SEN_L		(1<<PC0)	// lewy
#define SEN_C		(1<<PC1)	// centrum
#define SEN_R		(1<<PC2)	// prawy
#define SEN_LL		(1<<PC3)	// lewy brzegowy
#define SEN_RR		(1<<PC4)	// prawy brzegowy

// stany przycisk�w
#define BTN_OFF		0x00		// nic nie wci�ni�te
#define BTN_ON		0x01		// funkcyjny wci�ni�ty
#define BTN_L		0x01		// wci�ni�ty Lewy
#define BTN_R		0x02		// wci�ni�ty Prawy

// flaga reakcji na wci�ni�cie
#define BTN_GET_ON	4
#define SENS_IDLE	300000		// czas zw�oki do nast�pnego pomiaru z sensor�w (1-4000000)
#define CHNG_IDLE	30

// definicje stan�w flag
#define STATE_IDLE				0x00
#define STATE_RUNNING			0x01

#define MODE_STANDING			0x00	// stoi
#define MODE_PROGRESSING		0x01	// jazda w prz�d
#define MODE_REVERSING			0x02	// cofanie przy iteracji >8
#define MODE_BACK_TURNING		0x03	// obr�t

// poruszanie si� odkurzacza
#define VACU_STEPS_TO_REVERSE			20		// ile krok�w si� cofn��
#define VACU_STEPS_TO_TURN90			62		// ile krok�w jednego silnika by obr�ci� o 90deg

#endif /* ATMEGA_SLAVE_CONF_H_ */