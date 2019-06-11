#include <system.h>
#include <sys/alt_irq.h>
#include <stdio.h>

#define BUF_SIZE 500000
#define BUF_THRESHOLD 96

void Audio_ISR();

int Audio_Recording = 0, Audio_Play = 0, Audio_Buffer_index = 0;
extern volatile int * pAudio;
extern volatile int * pLED;

unsigned int l_buf[BUF_SIZE];
unsigned int r_buf[BUF_SIZE];

void Init_Audio() {
	int pb_ctxt = 0;
	alt_irq_register(6, (void *)&pb_ctxt, (void *)Audio_ISR);
}

void Audio_ISR()
{
   ////recored////
   if (Audio_Recording == 1)
   {
      *(pLED) = 0x1;
      if (Audio_Buffer_index < BUF_SIZE)
      {
         l_buf[Audio_Buffer_index] = *(pAudio + 2);
         r_buf[Audio_Buffer_index] = *(pAudio + 3);
         ++Audio_Buffer_index;

      }
      else if (Audio_Buffer_index >= BUF_SIZE)
      {
         *(pAudio) = 0;
         Audio_Recording = 0;
         *(pLED) = 0;
      }
   }
   ////audio play////
   else if ((Audio_Play==1))
   {
      *(pLED) = 0x2;

      if (Audio_Buffer_index < BUF_SIZE)
      {
         *(pAudio + 2) = l_buf[Audio_Buffer_index];
         *(pAudio + 3) = r_buf[Audio_Buffer_index];
         ++Audio_Buffer_index;
      }
      else if (Audio_Buffer_index >= BUF_SIZE)
      {
         *(pAudio) = 0;
         Audio_Play = 0;
         *(pLED) = 0;
      }
   }
}
