#include <stdio.h>
#include "VGA.h"
#include "Audio.h"
#include "Button.h"

volatile int * pAudio 	= (int *)0xff203040;
volatile int * pLED 	= (int *)0xFF200000;
volatile int * pKEY 	= (int *)0xff200050;
volatile int * pSwitch 	= (int *)0xff200040;
volatile int * pHEX0_3  = (int *)0xFF200020;
volatile int * pHEX4_5  = (int *)0xFF200030;

int main() {
	Init_VGA();
	Init_KEY_SW();

	while (1) {
		VGA_ClockDraw_S(VGA_DrawMode, currentTime);
		VGA_HumDraw(VGA_DrawMode);
		VGA_TemDraw(VGA_DrawMode);

		switch (VGA_Floating) {
		case 1:
			ClockSettingDraw(set_cur, dummyTime);
			break;
		case 2:
			VGA_AlarmDraw_S(alarm_cur, alarmTime);
			break;
		}

		wait_for_vsync();
	}

	return 0;
}
