#include <system.h>
#include <stdio.h>
#include <sys/alt_irq.h>
#include "altera_avalon_pio_regs.h"
#include "altera_up_avalon_audio.h"

#define BUF_SIZE 500000 // about 10 seconds of buffer (@ 48K samples/sec)
#define BUF_THRESHOLD 96 // 75% of 128 word buffer

volatile int *Pkey =(int*) 0xff200050;
volatile int *PLed =(int*) 0xff200000;

struct alt_up_dev{
   alt_up_audio_dev *audio_dev;
   int* Pkey;
};

struct alt_up_dev up_dev;
volatile int buf_record, buf_play;
volatile int * PLED = (int *)0xff200000; // red LED address
unsigned int left_buffer[BUF_SIZE];
unsigned int right_buffer[BUF_SIZE];

void audio_ISR(struct alt_up_dev*, unsigned int );
void pushbutton_ISR(struct alt_up_dev* , unsigned int );

int main()
{
   alt_up_audio_dev *audio_dev;
   audio_dev = alt_up_audio_open_dev("/dev/Audio_Subsystem_Audio");
   up_dev.audio_dev = audio_dev;
   up_dev.Pkey = Pkey;
   *(Pkey + 2) = 0b1111;        //인터럽트 허용
   alt_irq_register(PUSHBUTTONS_IRQ,(void*)&up_dev,(void*)pushbutton_ISR);
   alt_irq_register(6,(void*)&up_dev,(void*)audio_ISR);
}

void pushbutton_ISR(struct alt_up_dev *up_dev, unsigned int id) {
   alt_up_audio_dev *audio_dev;
   audio_dev = up_dev-> audio_dev;
   int Key_value=*(Pkey+3);
   *(Pkey+3)=Key_value;
   printf(Key_value);

   if(Key_value==0x2){  //이거 누르면 녹음 ㅅ기받
	   buf_record = 0; // reset counter to start recording
	   alt_up_audio_reset_audio_core (audio_dev); // reset audio port
	   alt_up_audio_enable_read_interrupt (audio_dev); }
   else if (Key_value== 0x4) {  //재생 시작
	   buf_play= 0; // reset counter to start playback
	   alt_up_audio_reset_audio_core (audio_dev); // reset audio port
	   alt_up_audio_enable_write_interrupt (audio_dev);
   }

   return ;
}

void audio_ISR(struct alt_up_dev *up_dev, unsigned int id){
   int num_read;
   int num_written;
   if (alt_up_audio_read_interrupt_pending(up_dev->audio_dev)){
	   *PLED=0x1;
	   if (buf_record < BUF_SIZE){
		   num_read = alt_up_audio_record_r (up_dev->audio_dev, &(left_buffer[buf_record]),BUF_SIZE - buf_record);
		   (void) alt_up_audio_record_l (up_dev->audio_dev, &(left_buffer[buf_record]), num_read);
		   buf_record += num_read;
		   if (buf_record == BUF_SIZE){
			   alt_up_audio_disable_read_interrupt(up_dev->audio_dev);
			   *PLED=0xb00;
		   }
	   }
	}
   else if (alt_up_audio_write_interrupt_pending(up_dev->audio_dev)) {
      *PLED=0b11;
      if (buf_play < BUF_SIZE)   {
    	  num_written = alt_up_audio_play_r (up_dev->audio_dev, &(right_buffer[buf_play ]),BUF_SIZE -buf_play );
    	  (void) alt_up_audio_play_l (up_dev->audio_dev, &(left_buffer[buf_play ]), num_written);
    	  buf_play += num_written;
    	  if (buf_play == BUF_SIZE) // done playback
    	  {
    		  *(PLED)=0b00;
    		  alt_up_audio_disable_write_interrupt(up_dev->audio_dev);
    	  }
      }
   }
   return;
}
