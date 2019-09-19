#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define CLK_CYCLE_TIME 10


void toggle_led(int *gpio_led_addr, int *led_time, int num);

void timer(int ms) {


int gpio_led_addr[] = {0x100,0x200,0x300,0x400,0x500};
int led_time[] = {1,2,3,4,5}; //pulse widths of total cycle time CLK_CYCLE_TIME

	while(1) { 
    toggle_led(gpio_led_addr,led_time,5);
    usleep(ms);
	}
}

int *timer_arry;
void toggle_led(int *gpio_led_addr, int *led_time, int num) {


    static int cnt = 0; 
    printf("\n****** time_sec:%d START ****** \n",cnt );

    if(cnt == 0)
    	timer_arry  = (int *)malloc(sizeof(int)*num);
    

    if(cnt == 0 || cnt == CLK_CYCLE_TIME)
    	memcpy(timer_arry,led_time,num*sizeof(int));
	
  
    for(int i = 0; i<num; i++) { 

	    if(timer_arry[i] > 0 ) {
		printf("gpio_led: %x\n", gpio_led_addr[i]);
		timer_arry[i]--;
    }
  
  }

   cnt = (cnt < CLK_CYCLE_TIME) ? cnt+1 : 1;
   printf("****** time_sec:%d END ****** \n\n\n",cnt);  
}






int main() {

timer(1000*1000);


}