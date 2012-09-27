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

// krokowe: stany prze��cze� krok�w
#define M0_SWITCH_FLAG			100
#define M1_SWITCH_FLAG			200

// krokowe: ograniczenie pr�dko�ci
#define M0_SPD_L				0
#define M0_SPD_H				220
#define M1_SPD_L				0
#define M1_SPD_H				220

// krokowe: ratio prze�o�e� na jednostk�
#define M0_RATIO				1
#define M1_RATIO				1

// krokowe: ograniczenie pozycji
#define M0_POS_MIN				0
#define M0_POS_MAX				400
#define M1_POS_MIN				1
#define M1_POS_MAX				40000

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
#define BTN_GET_ON	40

// definicje stan�w flag
#define STATE_IDLE				0x00
#define STATE_RUNNING			0x01
#define STATE_RESETTING			0x02

#define MODE_PROGRESSING		0x00	// jazda w prz�d
#define MODE_REVERSING			0x01	// cofanie przy iteracji >8
#define MODE_BACK45_TURNING		0x02	// zwrot do ty�u o 45deg
#define MODE_BACK90_TURNING		0x03	// zwrot do ty�u o 90deg

#endif /* ATMEGA_SLAVE_CONF_H_ */