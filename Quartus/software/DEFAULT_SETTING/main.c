#include <stdio.h>
#include "VGA.h"
#include "Audio.h"
#include "Button.h"

volatile int * pAudio 	= (int *)0xff203040;
volatile int * pLED 	= (int *)0xFF200000;
volatile int * pKEY 	= (int *)0xff200050;
volatile int * pSwitch 	= (int *)0xff200040;

int main() {
	Init_VGA();
	Init_KEY_SW();

	while(1);

	return 0;
}
