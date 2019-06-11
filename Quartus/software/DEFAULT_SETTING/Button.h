#include <system.h>
#include <sys/alt_irq.h>
#include <stdio.h>

extern volatile int * pKEY;
extern volatile int * pSwitch;
extern volatile int * pLED;

void Pushbutton_ISR();

void Init_KEY_SW() {
	*(pKEY+2) = 0b1111;
	int pb_ctxt = 0;
	alt_irq_register(1, (void *)&pb_ctxt, (void *)Pushbutton_ISR);
}


void Pushbutton_ISR()
{
   ////update and reset key value////
   int key_value;
   key_value = *(pKEY + 3);
   *(pKEY + 3) = key_value;
   int swval = *pSwitch;

   *pLED = ( swval << 4 ) | (key_value);
   ////interrupt each mode////
   ////switch 0 common mode
   ////switch 1 time mode
   ////switch 2 camera mode
   ////switch 4 audio mode

   if(swval == 0 )
   {
      if(key_value == 1)
      {
         printf("common key 0\n");
      }
      else if(key_value == 2)
      {
         printf("common key 1\n");
      }
      else if(key_value == 4)
      {
         printf("common key 2\n");
      }
      else if(key_value == 8)
      {
         printf("common key 3\n");
      }
   }
   if(swval == 1 )
   {
      if(key_value == 1)
      {
         printf("time key 0\n");
      }
      else if(key_value == 2)
      {
         printf("time key 1\n");
      }
      else if(key_value == 4)
      {
         printf("time key 2\n");
      }
      else if(key_value == 8)
      {
         printf("time key 3\n");
      }
   }
   if(swval == 2 )
   {
      if(key_value == 1)
      {
         printf("camera key 0\n");
      }
      else if(key_value == 2)
      {
         printf("camera key 1\n");
      }
      else if(key_value == 4)
      {
         printf("camera key 2\n");
      }
      else if(key_value == 8)
      {
         printf("camera key 3\n");
      }
   }
   if(swval == 4 )
   {
      if(key_value == 1)
      {
         printf("audio key 0\n");
      }
      else if(key_value == 2)
      {
         printf("audio key 1\n");
      }
      else if(key_value == 4)
      {
         printf("audio key 2\n");
      }
      else if(key_value == 8)
      {
         printf("audio key 3\n");
      }
   }


}

