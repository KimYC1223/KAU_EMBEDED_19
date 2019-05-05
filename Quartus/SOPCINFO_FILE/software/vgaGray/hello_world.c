#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>


void Timer_ISR();
void wait_for_vsync(void);
void fillColor(char rgb);
void plot_pixel(int x, int y, char line_color);
void clear_screen();

volatile int * pTimer       = (int *)0xFF202000;
volatile int * pTime_low    = (int *)0xFF202008;
volatile int * pTime_high    = (int *)0xFF20200C;
volatile int count = 0;

volatile int *pixel_ctrl_ptr = (int*)VGA_SUBSYSTEM_VGA_PIXEL_DMA_BASE;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];
volatile int pixel_buffer_start;

int main() {
   int pb_ctxt=0;
   *(pTime_low)     = 0x2D00;
   *(pTime_high)    = 0x0131;
   *(pTimer+1)      = 0x0007;
   alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);

   *(pixel_ctrl_ptr + 1) = front_buffer;
      wait_for_vsync();
      pixel_buffer_start = *pixel_ctrl_ptr;
      clear_screen();
      *(pixel_ctrl_ptr + 1) = back_buffer;
   while(1){
   }

   return 0;
}

void Timer_ISR() {
   *pTimer = 0;
   count ++;
   if (count >= 255) count = 0;
   pixel_buffer_start = *(pixel_ctrl_ptr + 1);
   //clear_screen();
   fillColor((char)count);
   wait_for_vsync();
}

void fillColor(char rgb){
   for(int i=0;i<320;i++)
      for(int j=0;j<240;j++)
         plot_pixel(i, j, rgb);
}

void wait_for_vsync(void){
   register int status;
   *pixel_ctrl_ptr = 1;

   status = *(pixel_ctrl_ptr +3);
   while ((status & 0x01)!=1){
      status = *(pixel_ctrl_ptr + 3);
   }
}

void plot_pixel(int x, int y, char line_color){
   *(char *)(pixel_buffer_start + (y << 9) + (x)) = line_color;
}

void clear_screen(){
   for (int x = 0; x<320; x++)
      for (int y = 0; y<240; y++)
         plot_pixel(x, y, 0xffff);
}
