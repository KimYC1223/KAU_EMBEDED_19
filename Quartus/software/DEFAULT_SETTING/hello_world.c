#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>


void Timer_ISR();
void wait_for_vsync(void);
void fillColor();
void plot_pixel(int x, int y, short int line_color);

volatile int * pTimer       = (int *)0xFF202000;
volatile int * pTime_low    = (int *)0xFF202008;
volatile int * pTime_high    = (int *)0xFF20200C;
volatile int count = 0;

volatile int *pixel_ctrl_ptr = (int*)VGA_SUBSYSTEM_VGA_PIXEL_DMA_BASE;
volatile int *video_ctrl_ptr = (int*)VIDEO_IN_SUBSYSTEM_VIDEO_IN_DMA_BASE;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];
volatile int pixel_buffer_start;
volatile int video_buffer_start;

int main() {
   int pb_ctxt=0;
   printf("test");
   *(pTime_low)     = 0xE100;
   *(pTime_high)    = 0x05F0;
   *(pTimer+1)      = 0x0007;

   alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);
   *(video_ctrl_ptr+3) = 0;
   *(pixel_ctrl_ptr + 1) = front_buffer;
      wait_for_vsync();

      pixel_buffer_start = *pixel_ctrl_ptr;
      *(pixel_ctrl_ptr + 1) = back_buffer;
   while(1);

   return 0;
}

void Timer_ISR() {
   *pTimer = 0;

   pixel_buffer_start = *(pixel_ctrl_ptr + 1);
   video_buffer_start = *(video_ctrl_ptr);
   fillColor();

   wait_for_vsync();
}

void fillColor(){
   for(int i=0;i<320;i++)
      for(int j=0;j<240;j++)
         plot_pixel(i, j, 0xFFFF);
}

void wait_for_vsync(void){
   register int status;
   *pixel_ctrl_ptr = 1;

   status = *(pixel_ctrl_ptr +3);
   while ((status & 0x01)!=0){
      status = *(pixel_ctrl_ptr + 3);
   }
}

void plot_pixel(int x, int y, short int line_color){
   *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}
