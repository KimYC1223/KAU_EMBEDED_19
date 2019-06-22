#include <system.h>
#include <sys/alt_irq.h>
#include <stdio.h>

extern volatile int * pAudio;
extern volatile int * pLED;
extern volatile int * pKEY;
extern volatile int * pSwitch;

extern void HEX_PrintHEX(int number1, int number2, int number3);


#define BUF_SIZE 500000
#define BUF_THRESHOLD 96

int record = 0, play = 0, buffer_index = 0;
unsigned int l_buf[2][BUF_SIZE];
unsigned int r_buf[2][BUF_SIZE];
volatile int sr = 0;
volatile int echo = 0;

void Audio_ISR()
{
   ////recored////
   if (record == 1 && mode == 4)
   {
      *(pLED) = 0x1;
      if (buffer_index < BUF_SIZE)
      {
         l_buf[sr][buffer_index] = *(pAudio + 2);
         r_buf[sr][buffer_index] = *(pAudio + 3);
         ++buffer_index;

      }
      else if (buffer_index >= BUF_SIZE)
      {
         *(pAudio) = 0;
         record = 0;
         *(pLED) = 0;
         HEX_PrintHEX(40,0,0);
      }
   }
   ////audio play////
   else if ((play==1))
   {
      *(pLED) = 0x2;

      if (buffer_index < BUF_SIZE)
      {
         if(echo == 0)
         {
            *(pAudio + 2) = l_buf[sr][buffer_index];
            *(pAudio + 3) = r_buf[sr][buffer_index];
         }
         else if(echo == 1)
         {
            int long audior_temp;
            int long audiol_temp;
            int audio_rst;
            if(buffer_index<8000)
            {
               *(pAudio + 2) = l_buf[sr][buffer_index];
               *(pAudio + 3) = r_buf[sr][buffer_index];
            }
            else if(buffer_index<16000)
            {
               audior_temp = r_buf[sr][buffer_index]*4+r_buf[sr][buffer_index-8000]*2;
               audior_temp = audior_temp>>3;
               audiol_temp = l_buf[sr][buffer_index]*4+l_buf[sr][buffer_index-8000]*2;
               audiol_temp = audiol_temp>>3;
               audio_rst = (int)audior_temp;
               *(pAudio + 3) = audio_rst;
               audio_rst = (int)audiol_temp;
               *(pAudio + 2) = audio_rst;

            }
            else
            {
               audior_temp = r_buf[sr][buffer_index]*4+r_buf[sr][buffer_index-8000]*2+r_buf[sr][buffer_index-16000];
               audior_temp = audior_temp>>3;
               audiol_temp = l_buf[sr][buffer_index]*4+l_buf[sr][buffer_index-8000]*2+l_buf[sr][buffer_index-16000];
               audiol_temp = audiol_temp>>3;
               audio_rst = (int)audior_temp;
               *(pAudio + 3) = audio_rst;
               audio_rst = (int)audiol_temp;
               *(pAudio + 2) = audio_rst;
            }


         }
         ++buffer_index;
      }
      else if (buffer_index >= BUF_SIZE)
      {
         *(pAudio) = 0;
         play = 0;
         *(pLED) = 0;
         if(mode ==4) HEX_PrintHEX(40,0,0);
      }
   }
}

void Audio_Record() {
	printf("audio record \n");
	buffer_index = 0;
	*(pAudio) = 0x1;
	record = 1;
	HEX_PrintHEX(41, 0, 0);
}

void Audio_Echo() {
	printf("audio play\n");
	echo = echo ^ 1;
	HEX_PrintHEX(43, (sr + 1), 0);
}

void Audio_Change() {
	printf("audio change 2\n");
	sr = (sr==0)? 1:0;
	HEX_PrintHEX(43, (sr + 1), 0);
}

void Audio_Mute() {
	printf("audio x\n");
}

void Audio_Play() {
	buffer_index = 0;
	*(pAudio) = 0x2;
	play = 1;
}
