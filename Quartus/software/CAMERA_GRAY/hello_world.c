#include <stdio.h>
#include <system.h>


void Timer_ISR();
void wait_for_vsync(void);
void fillColor();
void plot_pixel(int x, int y, short int line_color);


volatile int *pixel_ctrl_ptr = (int*)VGA_SUBSYSTEM_VGA_PIXEL_DMA_BASE;
volatile int *video_ctrl_ptr = (int*)VIDEO_IN_SUBSYSTEM_VIDEO_IN_DMA_BASE;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];
volatile int pixel_buffer_start;
volatile int video_buffer_start;

int main() {
   int pb_ctxt=0;
   *(video_ctrl_ptr+3) = 0;
   printf("test\n");
   while(1){
	   pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	   video_buffer_start = *(video_ctrl_ptr);
	   fillColor();

	   wait_for_vsync();
   }

   return 0;
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
