#include "header.h"


void clkInt (void){ 
	u8 i;

	T4CON&=~0x80; 	//clear TF4 interrupt flag

	WDTCN=0xAD;
	WDTCN=0xFF;
	
	{//time_test=90us 
		time.ms++;			
		for(i=0;i<MAX_DIGITAL_PINS;i++){	//process dout timeout 
			if(dout_timeout_sec[i]>0){		//check if digital pin have a timeout counter
				dout_timeout_ms[i]++;
				if(dout_timeout_ms[i]>999){
					dout_timeout_ms[i]=0;
					dout_timeout_sec[i]--;
					if(dout_timeout_sec[i]==0){
						set_dout(i,FALSE);	//set digital pin to high-z (OFF)
					}				
				}	
			}
		}
		if(time.ms>999){		//increas timer_sec
			time.ms=0;		
			time.sec++;
			flag_1sec=1;
		}
	}
	
	

	
}


