/*
 * atmega_slave.c
 *
 * Created on: 26.06.2011
 *     Author: Seweryn 'cadavre' Zeman
 */

#include <avr/interrupt.h>
#include <avr/io.h>
#include "atmega_slave_conf.h"

/*
 * Variables for stepping motors controls
 */
volatile uint8_t motor_current_step[2] = {1,1};		// 1-8
volatile uint16_t motor_pos[2] = {0,5};
volatile uint8_t set_speed = 0;
volatile uint8_t motor_speed[2] = {0,0};

/*
 * Variables for SPI
 */
volatile uint8_t drive_state[6] = {0,0,0,0,0,0};
volatile uint8_t btn_state[6] = {BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF};

volatile uint8_t j = 0;
volatile uint8_t btn_get_flag = 0;
volatile uint8_t stepper_flag[2] = {0,0};

/*
 * Prototypes
 */
void motor_make_step(uint8_t motor, uint8_t dir);
void motor_move(uint8_t motor, uint8_t dir);

/*
 * Initialize ports for stepping motors outputs
 */
void motor_ports_init(void) {
	DDRD |= MOTOR_PD;
	PORTD &= ~(MOTOR_PD);
	DDRC |= MOTOR_PC;
	PORTD &= ~(MOTOR_PC);
	// set ports to default STEP1
	M0_STEP1;
	M1_STEP1;
}

/*
 * Analog-Digital-Converter interface initialization
 */
void ADC_init(void) {
	ADMUX |= (1<<REFS0)|(1<<REFS1);								// 2,56V
	ADMUX |= (1<<ADLAR);										// ADCH only
																// ADC0 by default
	ADCSRA |= (1<<ADIE);										// ADC interrupt flag
	ADCSRA |= (1<<ADEN)|(1<<ADPS0)|(1<<ADPS1)|(1<<ADPS2);		// prescaler 128
	ADCSRA |= (1<<ADSC);										// initial measurement
}

/*
 * SPI transfer interface initialization
 */
void SPI_init(void) {
	DDRB |= MISO;
	SPCR |= (1<<SPIE)|(1<<SPE);
}

/*
 * Initialize Timer0 for switching steps on stepping motors
 */
void Timer0_init(void) {
	TCCR0 |= (1<<CS00);							// no prescaler
	TIMSK |= (1<<TOIE0);						// overflow IRQ
}

/*
 * SPI interrupt handle
 * - r/w
 */
ISR(SPI_STC_vect) {
	btn_state[j] = SPDR;
	if(j<5) {
		j++;
	} else {
		j = 0;
	}
	SPDR = drive_state[j];
}

/*
 * Timer0 interrupt handler
 */
ISR(TIMER0_OVF_vect) {
	if (stepper_flag[0] > (255 - motor_speed[0])) {
		stepper_flag[0] = 0;
	} else {
		stepper_flag[0]++;
	}
	if (stepper_flag[1] > (255 - motor_speed[1])) {
		stepper_flag[1] = 0;
	} else {
		stepper_flag[1]++;
	}

	if (stepper_flag[0] == 0) {
		if (1 || btn_state[0]==BTN_L) {
			//motor_move(0,0);
		} else if (btn_state[0]==BTN_R) {
			//motor_move(0,1);
		}
	}
	if (stepper_flag[1] == 0) {
		if (1 || btn_state[3]==BTN_L) {
			motor_move(1,0);
		} else if (btn_state[3]==BTN_R) {
			//motor_move(1,1);
		}
	}
}

/*
 * ADC interrupt handler
 * - take value as raw 8bit reading
 * - update SPI data of speed
 */
ISR(ADC_vect) {
	// 0 - 255
	set_speed = drive_state[5] = ADCH;
}

/************************************************* MAIN *************************************************/
int main(void)
{
	motor_ports_init();
	Timer0_init();
	SPI_init();
	ADC_init();
	sei();
	while(1)
	{
		if (btn_get_flag==BTN_GET_ON) {

			// refresh current drives positions
			//drive_state[0] = motor_pos[0] * M0_RATIO;
			//drive_state[1] = (servo_pos_raw[1] - SERVO_MIN+50) / SERVO_STEPS_PER_DEG;		// +50 dla wyrównania 0 stopni
			//drive_state[2] = motor_pos[1] / M1_RATIO;																					// TEMP: dzielenie zamiast mno¿enia!
			//drive_state[3] = (servo_pos_raw[2] - SERVO_MIN+50) / SERVO_STEPS_PER_DEG;

			// measure value for speed
			ADCSRA |= (1<<ADSC);

			// set motors speed
			if (set_speed > M0_SPD_H) {
				motor_speed[0] = M0_SPD_H;
			} else if (set_speed < M0_SPD_L) {
				motor_speed[0] = M0_SPD_L;
			} else {
				motor_speed[0] = set_speed;
			}
			if (set_speed > M1_SPD_H) {
				motor_speed[1] = M1_SPD_H;
			} else if (set_speed < M1_SPD_L) {
				motor_speed[1] = M1_SPD_L;
			} else {
				motor_speed[1] = set_speed;
			}

			// clear button flag
			btn_get_flag = 0;

		}
	}
}
/********************************************** END OF MAIN **********************************************/

/*
 *	Toggle ports states to switch coil set - make step
 */
void motor_make_step(uint8_t motor, uint8_t step) {
	if (step>4) {
		step = 1;
	} else if (step<1) {
		step = 4;
	}
	if (motor==0) {
		switch(step) {
			case 1:
				M0_STEP1;
				break;
			case 2:
				M0_STEP2;
				break;
			case 3:
				M0_STEP3;
				break;
			case 4:
				M0_STEP4;
				break;
			case 5:
				M0_STEP5;
				break;
			case 6:
				M0_STEP6;
				break;
			case 7:
				M0_STEP7;
				break;
			case 8:
				M0_STEP8;
				break;
		}
	} else if (motor==1) {
		switch(step) {
			case 1:
				M1_STEP1;
				break;
			case 2:
				M1_STEP2;
				break;
			case 3:
				M1_STEP3;
				break;
			case 4:
				M1_STEP4;
				break;
			case 5:
				//M1_STEP5;
				break;
			case 6:
				//M1_STEP6;
				break;
			case 7:
				//M1_STEP7;
				break;
			case 8:
				//M1_STEP8;
				break;
		}
	}
	motor_current_step[motor] = step;
}

/*
 * Move servo in particular directory by requesting step change
 */
void motor_move(uint8_t motor, uint8_t dir) {
	uint8_t next_step = (dir==1) ? motor_current_step[motor]+1 : motor_current_step[motor]-1;		// 0-8
	motor_make_step(motor, next_step);
	if (motor==0) {
		if (dir==0 && motor_pos[0]==M0_POS_MIN) {
			motor_pos[0] = M0_POS_MAX;
		} else if (dir==1 && motor_pos[0]==M0_POS_MAX) {
			motor_pos[0] = M0_POS_MIN;
		} else {
			motor_pos[0] = (dir==1) ? motor_pos[0]+1 : motor_pos[0]-1;
		}
	} else if (motor==1) {
		if (dir==0) {
			motor_pos[1]--;
		} else if (dir==1) {
			motor_pos[1]++;
		}
	}
}