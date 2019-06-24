#include <stdio.h>
#include "VGA.h"
#include "Audio.h"
#include "Button.h"
#include "Video.h"

volatile int * pAudio 	= (int *)0xff203040;
volatile int * pLED 	= (int *)0xFF200000;
volatile int * pKEY 	= (int *)0xff200050;
volatile int * pSwitch 	= (int *)0xff200040;
volatile int * pHEX0_3  = (int *)0xFF200020;
volatile int * pHEX4_5  = (int *)0xFF200030;

int main() {
	*pLED = 0; *pHEX0_3 = 0; *pHEX4_5 = 0;
	Init_VGA();
	Init_KEY_SW();
	Init_Audio();

	while (1) {
		VGA_ClearScreen();
		VGA_ClockDraw_S(VGA_DrawMode, currentTime);
		VGA_HumDraw(VGA_DrawMode);
		VGA_TemDraw(VGA_DrawMode);
		VGA_Icons(VGA_DrawMode);
		//VIDEO_View(VGA_DrawMode);

		switch (VGA_Floating) {
			case 1: VGA_ClockSettingDraw(set_cur, dummyTime); 	break;
			case 2: VGA_AlarmDraw_S(alarm_cur, alarmTime); 		break;
			case 3: VGA_RecordA(); 								break;
			case 4: VGA_RecordB(); 								break;
			case 5: VGA_PlayerA(); 								break;
			case 6: VGA_PlayerB(); 								break;
		}

		if (Audio_Recording == 1) {
			*(pAudio) = 0x1;
		} else if (Audio_Playing == 1) {
			*(pAudio) = 0x2;
		}

		wait_for_vsync();

	}

	return 0;
}
