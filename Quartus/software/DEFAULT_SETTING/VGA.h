#include "Timer.h"
#include "Img.h"

volatile int *pixel_ctrl_ptr = (int*)0xff203020;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];
volatile int pixel_buffer_start;

void wait_for_vsync(void);
void fillColor(short rgb);
void imageDraw(int x,int y, short * img);
void clear_screen();

extern void Timer_ISR();

void Init_VGA() {
	Init_timer();
	*(pixel_ctrl_ptr + 1) = front_buffer;
	wait_for_vsync();

	pixel_buffer_start = *pixel_ctrl_ptr;
	*(pixel_ctrl_ptr + 1) = back_buffer;
}

void clockDraw_L(int x,int y,timeSet currentTime) {
	if (currentTime.hour > 12)
		imageDraw(x,y,IMG_afternoon_L);
	else imageDraw(x,y,IMG_morning_L);

	int offset_y = (int)IMG_afternoon_L[1];
	int offset_num_x = (int)IMG_Num0_L[0];
	int offset_clone_x = (int)IMG_Clone_L[0];

	int hour = ( currentTime.hour < 25 && currentTime.hour > 0 ) ? ( currentTime.hour>12)?currentTime.hour-12:currentTime.hour : 0;
	int min = ( currentTime.min < 60 && currentTime.min > 0) ? currentTime.min : 0;
	int sec = ( currentTime.sec < 60 && currentTime.sec > 0) ? currentTime.sec : 0;

	imageDraw(x,y+offset_y,IMG_Number_L(hour/10));
	imageDraw(x+offset_num_x,y+offset_y,IMG_Number_L(hour%10));
	imageDraw(x+(offset_num_x*2),y+offset_y,IMG_Clone_L);
	imageDraw(x+(offset_num_x*2)+offset_clone_x,y+offset_y,IMG_Number_L(min/10));
	imageDraw(x+(offset_num_x*3)+offset_clone_x,y+offset_y,IMG_Number_L(min%10));
	imageDraw(x+(offset_num_x*4)+offset_clone_x,y+offset_y,IMG_Clone_L);
	imageDraw(x+(offset_num_x*4)+(offset_clone_x*2),y+offset_y,IMG_Number_L(sec/10));
	imageDraw(x+(offset_num_x*5)+(offset_clone_x*2),y+offset_y,IMG_Number_L(sec%10));
}

void imageDraw(int x,int y, short * img){
	int img_x = (int) img[0];
	int img_y = (int) img[1];

	int limit_x = (x+img_x > 320)? 320 : x+img_x;
	int limit_y = (y+img_y > 240)? 240 : y+img_y;
	int count = 2;

	for(int i=y;i<limit_y;i++)
		for(int j=x;j<limit_x;j++)
			plot_pixel(j, i, img[count++]);
}

void wait_for_vsync(void){
   register int status;
   *pixel_ctrl_ptr = 1;

   status = *(pixel_ctrl_ptr +3);
   while ((status & 0x01)!=0){
      status = *(pixel_ctrl_ptr + 3);
   }
}

void plot_pixel(int x, int y, short line_color){
   *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void clear_screen(){
   for (int x = 0; x<320; x++)
      for (int y = 0; y<240; y++)
         plot_pixel(x, y, 0x0000);
}
