#include "main.h"
#include "sys.h"

// static bufType rx;
// static msgType rxmsg;
// static bufType tx;
// static msgType txmsg;		
static timeType time;		
static u16 dout_timeout_sec[MAX_DIGITAL_PINS];
// static u16 dout_timeout_ms[MAX_DIGITAL_PINS];
// static u8 var128[128];
static bit flag_1sec;	//stores array of 40-bits 

void Main (void){		
	u16 i;
	
	/* hardware boot section*/
	{

		{//WatchDog Disable
				EA=0;
				WDTCN=0xDE;
				WDTCN=0xAD;
		}
		
		{//XOSC
			OSCXCN = 0x67;				//XOSC Mode=Crystal oscillator mode, freq>6.7 MHZ 
			
			for(i=2000; i>0 ;i--);		//1ms delay(2MHZ Clock)
				
			while(!(OSCXCN & 0x80)); 	//poll XTLVLD=>'1' 		
			OSCICN =0x08;				//Clock Select = external osc
		}	
			
		{//timers
		
			// TIMER3 - for dht22 and int_0	
			// TMR3=TMR3RL=0;
			// TMR3CN=0x00;	
			// TMR3CN=0x04;	//run timer3 when from inside int0 
			
			
			//TIMER4 - general clock, Timer4_ISR		
			TMR4=RCAP4=-22118;	//0x5666	//1ms cycle
			// TMR4=RCAP4=-SYSCLK/T4CLK;
			CKCON|=0x40;						//set T4M=1 (timer4 is timer function and uses system clk)
			
			T4CON|=0x04; //set TR4 bit - start timer4

			//TIMER1 - clock for uart0 
		
			CKCON|=0x10;						//set T1M=1 (timer1 uses system clk)			
			TMOD |=0x20;						//timer1 mode2 - 8bit autoreload .		
			TH1=-SYSCLK/16L/BAUDRATE;			// TH1=-12;			
			TL1=TH1;							//reload		
			TR1=1;								//run timer1	
		}
		
		{//UART0
			//(2^SMOD0/32)*(SYSCLK*12^(T1M-1))/(256-TH1)
			//TH1==256-(SYSCLK/16L)/BAUDRATE)
			// T2CON &=~0x30;			//set timer1 as a source clock (default)
			PCON  |= 0x80;				//set SMOD0=1, baud rate generator clock devider is 16
			SCON0=0x50; 				//mode1: 8-Bit UART, Variable Baud Rate.
			// UART_READ_DISABLE=UART_WRITE_ENABLE=FALSE;	//enable read
			Rx_init();
			Tx_init();
			
		}//UART0
		
		{//PORTS
		
		
			// #define PORT_IN_P69			P2		
			// #define PORT_IN_DIP_SWITCH 	P3
			// #define PORT_OUT_P70	 		P4					
			// #define PORT_OUT_P71	 		P5				
			// #define PORT_IN_P63			P6				
			// #define PORT_IN_P68			P7			//rotation switches port	

			
			// P0MDOUT |=0x0d;				//P0.0(TX0),P0.2(RE_),P0.3(DE) are push-pull. support GPRIO boards
			// P0MDOUT =0x0d;				//P0.0(TX0),P0.2(RE_),P0.3(DE) are push-pull. support GPRIO boards
			P0MDOUT |= 0x01;                    // Set TX0(P0.0) port pin to push-pull
		
			//analog inputs P1(ADC1)
			
			P1MDOUT=0x0;	//set open-drain mode on P1
			P1MDIN=0x0;		//set analog input mode
			
			// P1MDOUT |= 0x40;    //Enable P1.6 (LED) as push-pull output.					
			//*digital inputs P3,P6,P7
			//*digital outputs P2,P4,P5 			
			// P2MDOUT=0x0;	//set open-drain mode on P2
			// P2MDOUT|=0x01;	//set P2.0 push-pull others are open-drain,  DIG_PORT_3 [24..31]
			P3MDOUT=0x0;	//set open-drain mode on P3
			P74OUT=0x00;	//set open-drain mode on P4,P5,P6,P7 
			
			P2=0xff;
			P3=0xff;
			P4=0xff;
			P5=0xff;
			P6=0xff;
			P7=0xff;			
			
		}//PORTS
		
		{//PCA
			// PCA0CN = 0x00;                      // Stop counter; clear all flags
			// PCA0CPM0 = 0x00;                    // Module 0 = 16-bit PWM mode and in datasheet Table 23.2 -  page 252 xxx
			// PCA0CPM1 = 0x00;                    // Module 0 = 16-bit PWM mode and in datasheet Table 23.2 -  page 252 xxx
			// PCA0CPM2 = 0xC2;                    // Module 0 = 16-bit PWM mode and in datasheet Table 23.2 -  page 252
			// PCA0CPM3 = 0xC2;                    // Module 0 = 16-bit PWM mode and in datasheet Table 23.2 -  page 252
			// PCA0CPM4 = 0xC2;                    // Module 0 = 16-bit PWM mode and in datasheet Table 23.2 -  page 252
			
			//// pca0 = 65536 - (65536 * 0.5);
			//// PCA0CPL0 = (pca0 & 0x00FF);
			//// PCA0CPH0 = (pca0 & 0xFF00)>>8;
			
			// pca0 = 0;
			// PCA0CPL0 = 0;
			// PCA0CPH0 = 0;
			// PCA0CPL1 = 0;
			// PCA0CPH1 = 0;
			// PCA0CPL2 = 0;
			// PCA0CPH2 = 0;
			// PCA0CPL3 = 0;
			// PCA0CPH3 = 0;
			// PCA0CPL4 = 0;
			// PCA0CPH4 = 0;
			
			
			// CR = 1;	// Start PCA counter
			
		}//PCA
		
		{//INT0
			// IT0=1;	//int_0 edge triggered interrupt in TCON register
			// EX0=1;	//int_0	interrupt enabled	
			
		}//INT0
			
		{//cross-bar
			XBR0=0x04;	//route TX0,RX0 to P0.0, P0.1	//ENABLED FOR C8051F020			
			// XBR1=0x04;	//route INT0 to P0.2
			
			// XBR0=0x24;	//route TX0,RX0 to P0.0, P0.1 + CEX0 , CEX1 for PCA0 	
			// XBR0=0x1C;	//route TX0,RX0 to P0.0, P0.1 + CEX0 , CEX1 and CEX2 (for PCA0)  to  P0.2, P0.3, P0.4 	//DISABLED FOR GPRIO
			// XBR1=0x14;	//route INT0,INT1 to P0.4,P0.5 
			// XBR1=0x04;	//route INT0 to P0.4  
			// XBR2=0x40;	//enable cross-bar,enable week-pull-ups. 	
			XBR2=0xC0;	//enable cross-bar,disable week-pull-ups. 	
			
			//enable PCA0 pins 
			
			// XBR2=0x40;	//enable cross-bar and week-pull-ups. 	
			
		}/*cross-bar*/		
		
		{//ADC[0..1] 
			ADC0CN=0x80;	//set adc0 enable
			ADC1CN=0x80;	//set adc1 enable
			// ADC0CN = 0x00;		//00: ADC0 disabled(enabled when read), conversion initiated on AD0BUSY
			REF0CN=0x07;		//VREF0 must externally connect to VREF , BIASE=REFBE=TEMPE='1'	,ADC[0..1] voltage reference from VREF[0..1] pin accordingly.
			ADC0CF = (SYSCLK/SARCLK) << 3;     // ADC conversion clock = 2.5MHz
			ADC0CF |= 0x00;                     // PGA gain = 1 (default)
			ADC1CF=0xfa;
			// ADC1CF=(SYSCLK/SARCLK)<<3;
			// ADC0CF=(31)<<3;
			
			// for(i=0;i<8;i++){
				// ADC0_set_diff(i,TRUE);//set to differential
			// }
			
		}//ADC[0..1]
		
		{//DAC0,DAC1
			
			
			
			DAC0L=0x0;
			DAC0H=0x0;	//set output when write DAC0H
			DAC0CN=0x80;	
			
			DAC1L=0x0;
			DAC1H=0x0;	//set output when write DAC0H
			DAC1CN=0x80;
			

		}//DAC[0..1]
	
		// {//clock init
		// 	time.us=0L;
		// 	time.ms=0L;
		// 	time.sec=0x0L;
			
		// 	for(i=0;i<MAX_DIGITAL_PINS;i++){
		// 		dout_timeout_sec[i]=0;
		// 	}
		// }
		{//Interrupts and CPU stack 
			SP=0x30;	//stack initiale offset
			
			PS0=1;		//uart ISR gets high priority
			ES0=1;		//uart0_enable_interrupt
			EIE2|=0X04;	//ENABLE timer4 INTERRUPT
			EA=1;
		}	

		{//FLASH
			FLASH_Load();
		}		
	}
	while(1){
		WDTCN=0xAD;
		if(flag_1sec){
			flag_1sec=0;
		}		
	}
}

void INT16Clock(void) interrupt 16 using 3 {
	_INT16Clock();
}

void INT4Uart0 (void) interrupt 4 using 2 {
	_INT4Uart0();
}
