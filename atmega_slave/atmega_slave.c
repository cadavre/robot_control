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
volatile uint8_t memory_state[6] = {0,0,0,0,0,0};
volatile uint8_t btn_state[6] = {BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF,BTN_OFF};

uint8_t iteration = 0;					// iteration number - one round is 8 (90deg=2, 45deg=1)
uint8_t course = 0;						// course number - first is zero
uint8_t max_dimension = 0;				// dm, max 25.5m
uint8_t total_dist = 0;					// m, max 255m

uint16_t i = 0;
volatile uint8_t j = 0;
volatile uint8_t btn_get_flag = 0;
volatile uint8_t stepper_flag = 0;

uint8_t current_state = STATE_IDLE;
uint8_t current_mode = MODE_STANDING;

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
	TCCR0 |= (1<<CS01);							// no prescaler
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
	SPDR = memory_state[j];
}

/*
 * Timer0 interrupt handler
 */
ISR(TIMER0_OVF_vect) {
	if (stepper_flag > (255 - motor_speed[0])) {
		stepper_flag = 0;
	} else {
		stepper_flag++;
	}

	if (stepper_flag == 0) {
		if (current_mode == MODE_PROGRESSING) {
			// both same speed forward
			motor_move(0,0);
			motor_move(1,0);
		} else if (current_mode == MODE_REVERSING) {
			// both same speed reverse
			motor_move(0,1);
			motor_move(1,1);
		} else if (current_mode == MODE_BACK90_TURNING) {
			// turning 90deg to back
			//motor_move(0,1);
		} else if (current_mode == MODE_BACK45_TURNING) {
			// turning 45deg to back
			//motor_move(0,1);
		}
	}

	btn_get_flag++;
}

/*
 * ADC interrupt handler
 * - take value as raw 8bit reading
 * - update SPI data of speed
 */
ISR(ADC_vect) {
	// 0 - 255
	set_speed = memory_state[5] = ADCH;
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

		/*
		 * Handle signals from sensors
		 */
		if (current_state == STATE_RUNNING) {
			if (btn_state[0] == BTN_ON || btn_state[1] == BTN_ON) {
				if (i == 0) {
					current_mode = MODE_REVERSING;
					course = iteration/8;
					if (btn_state[0] == BTN_ON) {
						// jeb³ œrodek
					} else if (btn_state[1] == BTN_ON) {
						// jeb³ lewy
					}
				} else if (i == SENS_IDLE) {
					i = 0;
				} else {
					i++;
				}
				btn_state[0] = BTN_OFF;
				btn_state[1] = BTN_OFF;
			}
			/*
			* 1. course = iteration/8		[floor]
			*	  (course > 1) { reverse o (course*VACUM_WIDTH) }
			* 2. (œrodek) { odwróæ o 90deg }
			*    (lewy) { odwróc o 45deg }
			* 3. jedŸ
			*/
		}

		/*
		 * Handle signals from buttons
		 */
		if (btn_get_flag == BTN_GET_ON) {

			if (btn_state[5] == BTN_L && current_state == STATE_IDLE) {
				// wciœniêto start podczas gdy w idle
				current_state = STATE_RUNNING; // todo move to method
				current_mode = MODE_PROGRESSING; // todo
				btn_state[5] = BTN_OFF;
			} else if (btn_state[5] == BTN_R && current_state == STATE_RUNNING) {
				// wciœniêto reset podczas gdy odkurza³
				current_state = STATE_IDLE; // todo move to method
				current_mode = MODE_STANDING; // todo
				btn_state[5] = BTN_OFF;
			}

			// refresh current memory
			memory_state[0] = current_state;
			memory_state[1] = current_mode;
			//memory_state[2] = ;
			memory_state[3] = course;
			memory_state[4] = total_dist;

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
				//M0_STEP5;
				break;
			case 6:
				//M0_STEP6;
				break;
			case 7:
				//M0_STEP7;
				break;
			case 8:
				//M0_STEP8;
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
	uint8_t next_step = (dir==1) ? motor_current_step[motor]+1 : motor_current_step[motor]-1;	// just increment, checking if step available in make_step();
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