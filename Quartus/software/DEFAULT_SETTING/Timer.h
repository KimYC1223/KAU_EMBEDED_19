#include <system.h>
#include <sys/alt_irq.h>

struct _Time {
	int hour;
	int min;
	int sec;
} typedef timeSet;

void Timer_ISR();
volatile int * pTimer       = (int *)0xFF202000;
volatile int * pTime_low    = (int *)0xFF202008;
volatile int * pTime_high    = (int *)0xFF20200C;
timeSet currentTime;

extern volatile int *pixel_ctrl_ptr;
extern volatile int pixel_buffer_start;
extern void clockDraw_L(int x,int y,timeSet currentTime);
extern void clear_screen();

void Timer_ISR() {
	*pTimer = 0;
	pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	clear_screen();
	currentTime.sec ++;

	if ( currentTime.sec == 60) {
		currentTime.sec = 0;
		currentTime.min++;
		if (currentTime.min==60)  {
			currentTime.min=0;
			currentTime.hour++;
			if (currentTime.hour == 25)
				currentTime.hour=0;
		}
	}
	clockDraw_L(5,5,currentTime);
	wait_for_vsync();
}

void Init_timer() {
	int pb_ctxt=0;
	*(pTime_low)     = 0xE0FF;
	*(pTime_high)    = 0x05F5;
	*(pTimer+1)      = 0x0007;
	alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);
	currentTime.hour = 24 ;
	currentTime.min = 0 ;
	currentTime.sec = 0 ;
}
