#include <system.h>
#include <sys/alt_irq.h>
#include <stdio.h>

extern volatile int * pAudio;
extern volatile int * pLED;
extern volatile int * pKEY;
extern volatile int * pSwitch;
extern volatile int VGA_Floating;

extern void HEX_PrintHEX(int number1, int number2, int number3);
extern void progressDrawISR();
void Audio_ISR();
void Audio_Play();

#define BUF_SIZE 512000
#define BUF_SIZE_SMALL 32000
#define BUF_THRESHOLD 96

int Audio_Recording = 0, Audio_Playing = 0, buffer_index = 0;
unsigned int l_buf[2][BUF_SIZE];
unsigned int r_buf[2][BUF_SIZE];
volatile int sr = 0;
volatile int AUDIO_Check;
volatile int AUDIO_CNT;
volatile int echo = 0;

void Init_Audio(){
	int pb_ctxt = 0;
	 Audio_Recording = 0;
	 Audio_Playing = 0;
	* pAudio = 0;
	printf("Done\n");
	alt_irq_register(6, (void *)&pb_ctxt, (void *)Audio_ISR);
	printf("Done2\n");
}

void Audio_ISR()
{
   if (Audio_Recording == 1 && mode ==4)
   {

      if (buffer_index < BUF_SIZE)
      {
         l_buf[sr][buffer_index] = *(pAudio + 2);
         r_buf[sr][buffer_index] = *(pAudio + 3);
         ++buffer_index;
      }
      else if (buffer_index >= BUF_SIZE)
      {
         *(pAudio) = 0;
         buffer_index = 0;
		 Audio_Recording = 0;
		 *(pLED) = 0;
		 AUDIO_Check += (sr == 0)? (AUDIO_Check & 0x1 == 0 ) ? 1 : 0 :(AUDIO_Check & 0x2 == 0)? 2: 0;
      }
   }
   else if ((Audio_Playing==1) && mode ==4)
   {
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
         Audio_Playing = 0;
         *(pLED) = 0;
      }
   }
}

void Audio_Record() {
	buffer_index = 0;
	*(pAudio) = 0x1;
	Audio_Recording = 1;
	*(pLED) = 0x1;
	printf("audio record \n");
}

void Audio_Echo() {
	printf("audio play\n");
	echo = echo ^ 1;
}

void Audio_Change() {
	printf("audio change 2\n");
	sr = (sr==0)? 1:0;
	HEX_PrintHEX(43, (sr + 1), 0);

	if (VGA_Floating == 3) VGA_Floating =4;
	else if (VGA_Floating == 4) VGA_Floating =3;
	else if (VGA_Floating == 5) VGA_Floating =6;
	else if (VGA_Floating == 6) VGA_Floating =5;

}

void Audio_Mute() {
	if (sr == 0) { sr -= (sr & 0x1 == 1)?  1:0; }
	else { sr -= (sr&0x2 == 1)? 2:0;}
}

void Audio_Play() {
	buffer_index = 0;
	*(pAudio) = 0x2;
	Audio_Playing = 1;
	*(pLED) = 0x2;
}
