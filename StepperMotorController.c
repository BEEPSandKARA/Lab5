// StepperMotorController.c starter file EE319K Lab 5
// Runs on TM4C123
// Finite state machine to operate a stepper motor.  
// Jonathan Valvano
// 1/17/2020

// Hardware connections (External: two input buttons, five outputs to stepper motor, and one LED)
// PA5 is Wash input (1 means pressed, 0 means not pressed)
// PA4 is Wiper input (1 means pressed, 0 means not pressed)
// PE5 is Water pump output LED (toggle means washing)
// PE4-0 are stepper motor outputs
// PF1 PF2 or PF3 control the LED on Launchpad used as a heartbeat


#include "SysTick.h"
#include "TExaS.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void EnableInterrupts(void);
// edit the following only if you need to move pins from PA4, PE3-0      
// logic analyzer on the real board
#define PA4       (*((volatile unsigned long *)0x40004040))
#define PE50      (*((volatile unsigned long *)0x400240FC))

#define home	0
#define S1_2	1
#define S1_4	2
#define S1_8	3
#define S1_16	4
#define S2_1	5
#define S2_2	6
#define S2_4	7
#define S2_8	8
#define S2_16	9
#define S3_1	10
#define SB0_16	11
#define SB1_8	12
#define SB1_4	13
#define SB1_2 14
#define SB1_1	15
#define SB2_16	16
#define SB2_8	17
#define SB2_4	18
#define SB2_2 19
#define S1_2M	20
#define S1_8M	21
#define S2_1M	22
#define S2_4M	23
#define S2_16M	24
#define SB0_16M	25
#define SB1_4M 26
#define SB1_1M 27
#define SB2_8M 28
#define SB2_2M 29





void SendDataToLogicAnalyzer(void){
  UART0_DR_R = 0x80|(PA4<<2)|PE50;
}
struct State{
	
	uint8_t Next[4];
	uint8_t output;
	uint8_t wait;
	
};

typedef struct State SType_t;



uint8_t CS = 0;

int main(void){ 
  TExaS_Init(&SendDataToLogicAnalyzer);    // activate logic analyzer and set system clock to 80 MHz
  SysTick_Init();   
EnableInterrupts();
	
	
// you initialize your system here
// INITLIAZE THE CLOCK
	SYSCTL_RCGCGPIO_R = 0x31;
//nop
	volatile uint16_t x;
	x = 69;
// set direction
	GPIO_PORTA_DIR_R &= (~0x30);
	GPIO_PORTE_DIR_R |= 0x3F;
	GPIO_PORTF_DIR_R |= 0x02;

	
// digital enable
	GPIO_PORTA_DEN_R |= 0x30;
	GPIO_PORTE_DEN_R |= 0x3F;
	GPIO_PORTF_DEN_R |= 0x02;
	
	
	
	
	
SType_t FSM[30] = {
	{{home, S1_2, S1_2M, S1_2}, 0x01, 5},
	{{S1_4, S1_4, S1_4, S1_4}, 0x02, 5},
	{{S1_8, S1_8, S1_8M, S1_8}, 0x04, 5},
	{{S1_16, S1_16, S1_16, S1_16}, 0x08, 5},
	{{S2_1, S2_1, S2_1M, S2_1}, 0x10, 5},
	{{S2_2, S2_2, S2_2, S2_2}, 0x01, 5},
	{{S2_4, S2_4, S2_4M, S2_4}, 0x02, 5},
	{{S2_8, S2_8, S2_8, S2_8}, 0x04, 5},
	{{S2_16, S2_16, S2_16M, S2_16}, 0x08, 5},
	{{S3_1, S3_1, S3_1, S3_1}, 0x10, 5},
	{{SB0_16, SB0_16, SB0_16M, SB0_16}, 0x01, 5},
	{{SB1_8, SB1_8, SB1_8, SB1_8}, 0x10, 5},
	{{SB1_4, SB1_4, SB1_4M, SB1_4}, 0x08, 5},
	{{SB1_2, SB1_2, SB1_2, SB1_2}, 0x04, 5},
	{{SB1_1, SB1_1, SB1_1M, SB1_1}, 0x02, 5},
	{{SB2_16, SB2_16, SB2_16, SB2_16}, 0x01, 5},
	{{SB2_8, SB2_8, SB2_8M, SB2_8}, 0x10, 5},
	{{SB2_4, SB2_4, SB2_4, SB2_4}, 0x08, 5},
	{{SB2_2, SB2_2, SB2_2M, SB2_2}, 0x04, 5},
	{{home, home, home, home}, 0x02, 5},
	{{S1_4, S1_4, S1_4, S1_4}, 0x22, 5},
	{{S1_16, S1_16, S1_16, S1_16}, 0x28, 5},
	{{S2_2, S2_2, S2_2, S2_2}, 0x21, 5},
	{{S2_8, S2_8, S2_8, S2_8}, 0x24, 5},
	{{S3_1, S3_1, S3_1, S3_1}, 0x30, 5},
	{{SB1_8, SB1_8, SB1_8, SB1_8}, 0x30, 5},
	{{SB1_2, SB1_2, SB1_2, SB1_2}, 0x24, 5},
	{{SB2_16, SB2_16, SB2_16, SB2_16}, 0x21, 5},
	{{SB2_4, SB2_4, SB2_4, SB2_4}, 0x28, 5},
	{{home, home, home, home}, 0x02, 5}
	
	};

		
    


	
while(1){
	
// output
	GPIO_PORTE_DATA_R = ((FSM[CS]).output);
	
// wait
	SysTick_Wait10ms(FSM[CS].wait);
// HEARTBEAT
	GPIO_PORTF_DATA_R ^= 0x02;
// input
	uint8_t input = ((GPIO_PORTA_DATA_R >> 4) &0x03);
// next	
	CS = FSM[CS].Next[input];
				}		
		}

