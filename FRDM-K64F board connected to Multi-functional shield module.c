#include "MK64F12.h"
void delayMs(int n);
void segment(unsigned data, unsigned pos);
void setupLed(void);
void setupswitch(void);
void setupinterrupt(void);
void Timer(void);
void clk(void);

int main (void) {
	clk();
	setupLed();
	setupswitch();
	setupinterrupt();
	
	while (1){
		
	 segment(0x91,0x40);/*(data = H ,pos = 3)*/
	 segment(0xF3,0x20);/*(data = I ,pos = 2)*/
	 
		
	}
}

void PORTB_IRQHandler(void){
	if(PTB->PDIR & 0x08){
		PTD->PSOR = 0xF;
		segment(0xFF,0xF0);
		PORTB->ISFR = 0x08;
	}		
	else{
	 
	 for(int i = 0;i < 100; i++){
	 segment(0x085,0x80);/*(data = D ,pos = 4)*/
	 segment(0x61,0x40);/*(data = E ,pos = 3)*/
	 segment(0x63,0x20);/*(data = C ,pos = 2)*/
	 PTD->PSOR = 0x4;PTD->PSOR = 0x2;PTD->PCOR = 0x8;PTD->PCOR = 0x1;
	 }

	 
		
	}
	if(PTB->PDIR & 0x400){
		PTD->PSOR = 0xF;
		segment(0xFF,0xF0);
		PORTB->ISFR = 0x400;
	}		
	else{
		
		for(int i = 0;i < 100; i++){
		PTD->PCOR = 0x4;
		segment(0x025,0x80);/*(data = 2 ,pos = 4)*/
		PTD->PCOR = 0x4;PTD->PCOR = 0x2;PTD->PSOR = 0x8;PTD->PSOR = 0x1;
		segment(0x03,0x40);/*(data = 0 ,pos = 3)*/
		segment(0x25,0x20);/*(data = 2 ,pos = 2)*/
		segment(0x0D,0x10);/*(data = 3 ,pos = 1)*/
		}
		
	}
	if(PTB->PDIR & 0x800){
		PTD->PSOR = 0xF;
		segment(0xFF,0xF0);
		PORTB->ISFR = 0x800;
	}		
	else{
	  
	 segment(0x6D,0xF0);/*(data = - ,pos = 1-4)*/	
	
	 Timer();
        

	}
}	



void Timer(void){
	int c;
	SysTick->LOAD = 0xFFFF00; /* initial SYST_RVR with 23 value */
	SysTick->CTRL = 5; /* initial SYST_CLR reg.
						- enable counter, - no interrupt,
						- use system clock */
	c = SysTick->VAL;
	PTD->PCOR = c&1;
	delayMs(10);
	PTD->PCOR = c&2;
	delayMs(10);
	PTD->PCOR = c&4;
	delayMs(10);
	PTD->PCOR = c&8;
	delayMs(10);
}
void clk(void){
	SIM->SCGC5 |= 0x200; /*PORT A*/
	SIM->SCGC5 |= 0x400; /*PORT B*/
	SIM->SCGC5 |= 0x800; /*PORT C*/
	SIM->SCGC5 |= 0x1000;/*PORT D*/
	SIM->SCGC5 |= 0x2000;/*PORT E*/
}
void setupLed(void){
	PORTD->PCR[0] = 0x0100;   /*PORTD->PCR[0] = 0x100 */
	PORTD->PCR[1] = 0x0100;   /*PORTD->PCR[1] = 0x100 */
	PORTD->PCR[2] = 0x0100;   /*PORTD->PCR[2] = 0x100 */
	PORTD->PCR[3] = 0x0100;   /*PORTD->PCR[3] = 0x100 */
	PTD->PDDR |= 0x01;
	PTD->PDDR |= 0x02;
	PTD->PDDR |= 0x04;
	PTD->PDDR |= 0x08;
	PTD->PSOR = 0x0F;
}

void segment(unsigned data, unsigned pos){
	SIM->SCGC5 |= 0x1E00; /* enable clock to Port A, B, C, and D*/
	PORTC->PCR[12] = 0x00100; /* make it GPIO Data*/
	PORTC->PCR[3] = 0x00100; /* make it GPIO Clock*/
	PORTB->PCR[23] = 0x00100; /* make it GPIO LCH*/
	PTC->PDDR |= 0x1000; /* output data*/
	PTC->PDDR |= 0x08; /* output clock*/
	PTB->PDDR |= 0x800000; /* output LCH*/
	PTC->PDOR &= ~0x8;   /* close CLK*/
	PTB->PDOR &= ~0x800000; /* close LCH*/
	for(int i = 0; i < 8; i++){ /*data 0-8*/
	PTC->PDOR = ((data >> i) & 1) <<12; /*data PTC12*/
	PTC->PDOR |= 0x8; /*open data*/
	PTC->PDOR &= ~0x8; /*close data*/
}
  for(int x = 0; x < 8; x++){ /*pos 0-8*/
	PTC->PDOR = ((pos >> x) & 1) <<12; /*pos PTC12*/
	PTC->PDOR |= 0x8; /*open pos*/
	PTC->PDOR &= ~0x8; /*close pos*/
}
	PTB->PDOR |= 0x800000; /* open LCH */
}

void setupswitch(void){
		PORTB->PCR[3] = 0x00100; /* make it GPIO */
		PTB->PDDR &= ~0x80; /* make pin input */
		PORTB->PCR[10] = 0x00100; /* make it GPIO */
		PTB->PDDR &= ~0x400; /* make pin input */
		PORTB->PCR[11] = 0x00100; /* make it GPIO */
		PTB->PDDR &= ~0x800; /* make pin input */
}

void setupinterrupt(void){
	/* configure PTB3 for interrupt */
	__disable_irq(); /* Step 6.2: disable all IRQs */
		PORTB->PCR[3] &= ~0xF0000; /* Step 6.3: clear interrupt selection */
		PORTB->PCR[3] |= 0xA0000; /* Step 6.4: enable rising edge interrupt */		
		NVIC->ISER[1] |= 0x10000000; /* Step 6.5: enable interrupt for PORTB at NVIC */

		PORTB->PCR[10] &= ~0xF0000; /* Step 6.3: clear interrupt selection */
		PORTB->PCR[10] |= 0xA0000; /* Step 6.4: enable rising edge interrupt */		
		NVIC->ISER[1] |= 0x10000000; /* Step 6.5: enable interrupt for PORTB at NVIC */

		PORTB->PCR[11] &= ~0xF0000; /* Step 6.3: clear interrupt selection */
		PORTB->PCR[11] |= 0xA0000; /* Step 6.4: enable rising edge interrupt */		
		NVIC->ISER[1] |= 0x10000000; /* Step 6.5: enable interrupt for PORTB at NVIC */
	__enable_irq(); /* Step 6.6: global enable IRQs */
}


void delayMs(int n){
	int i;
	int j;
	for(i = 0 ; i < n; i++)
		for (j = 0; j < 3500; j++) {
		}
}





