#include "system.h"
#include "stdio.h"
#define BUF_SIZE 500000 // about 10 seconds of buffer (@ 48K samples/sec)#define BUF_THRESHOLD 96 // 75% of 128 word buffer
#define BUF_THRESHOLD 96

int check_KEYs(int * record, int * play, volatile int * pKEY) {
	int temp = 0;
	while (*pKEY != 0) {
		temp = *pKEY;
		if (*pKEY == 0)
			break;
	}

	if (temp != 0) {
		if (temp == 2) {
			*record = 1;
			*play = 0;
		} else if (temp == 4) {
			*record = 0;
			*play = 1;
		}
		return temp;
	} else
		return 0;
}

int main(void) {
	volatile int * pKEY = (int *) 0xff200050;
	volatile int * pLED = (int *) LEDS_BASE; // red LED address
	volatile int * pAudio = (int *) AUDIO_SUBSYSTEM_AUDIO_BASE; // audio port address

	volatile unsigned char * RARC = 0xFF203044;
	volatile unsigned char * RALC = 0xFF203045;
	volatile unsigned char * WSRC = 0xFF203046;
	volatile unsigned char * WSLC = 0xFF203047;
	volatile int * LeftData = 0xFF203048;
	volatile int * RightData = 0xFF20304C;

	int left_buffer[BUF_SIZE];
	int right_buffer[BUF_SIZE];

	int record = 0, play = 0;
	*(pLED) = 0;

	while (1) {
		int check = check_KEYs(&record, &play, pKEY);

		if (record) {
			*pLED = 0x1;
			int recSec = 0;
			int offset = 0;
			printf("recording Start\n");
			while (offset < BUF_SIZE) {
				if (*RARC >= BUF_THRESHOLD || *RALC >= BUF_THRESHOLD) {
					right_buffer[offset] = *RightData;
					left_buffer[offset] = *LeftData;
					offset++;
				}
				if (offset % 48000 == 0)
					printf("%d...\n", recSec++);
			}
			record = 0;
			printf("recording Done\n");
			*pLED = 0x0;
		} else if (play) {
			*pLED = 0x2;
			int playSec = 0;
			int offset = 0;
			printf("Play Start\n");
			while (offset < BUF_SIZE) {
				if (*WSLC >= BUF_THRESHOLD || *WSRC >= BUF_THRESHOLD){
					*RightData = right_buffer[offset];
					*LeftData = left_buffer[offset];
					offset++;

					if (offset % 48000 == 0)
						printf("%d...\n", playSec++);
				}
			}
			play = 0;
			printf("Play Done\n");
			*pLED = 0x0;
		}
	}
}

