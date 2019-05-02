//*====================================================================================================
//	체크판 테스트
//*====================================================================================================

#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>


void Timer_ISR();
void wait_for_vsync(void);
void draw_square(int cx, int cy, int w, int h, int rgb);
void plot_pixel(int x, int y, short int line_color);
void clear_screen();
void pattern(int a,int b);

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
   *(pTime_low)     = 0xE100;
   *(pTime_high)    = 0x05F0;
   *(pTimer+1)      = 0x0007;
   alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);
   *(pixel_ctrl_ptr + 1) = front_buffer;
      wait_for_vsync();

      pixel_buffer_start = *pixel_ctrl_ptr;
      clear_screen(0x00, 0x00, 0x00);
      *(pixel_ctrl_ptr + 1) = back_buffer;
   while(1);

   return 0;
}

void Timer_ISR() {
   *pTimer = 0;
   count ++;
   int temp = count%3;
   pixel_buffer_start = *(pixel_ctrl_ptr + 1);
   clear_screen(0x00, 0x00, 0x00);
   if(temp==0) pattern(20,20);
   else if(temp==1)pattern(40,40);
   else if(temp==2)pattern(80,80);
   wait_for_vsync();
}

void draw_square(int cx, int cy, int w, int h, int rgb)
{
   for(int i=cx;i<cx+w;i++)
      for(int j=cy;j<cy+h;j++)
         plot_pixel(i, j, rgb);
}

void wait_for_vsync(void){
   register int status;
   *pixel_ctrl_ptr = 1;

   status = *(pixel_ctrl_ptr +3);
   while ((status & 0x01)!=0)
   {
      status = *(pixel_ctrl_ptr + 3);
   }
}
void plot_pixel(int x, int y, short int line_color)

{

   *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;

}
void clear_screen()

{

   for (int x = 0; x<320; x++)

      for (int y = 0; y<240; y++)

         plot_pixel(x, y, 0xffff);

}
void pattern(int a,int b)
{
   for(int i=0;i<320;i=i+a*2)
   {
      for(int j=0;j<240;j=j+b*2)
      {
         draw_square(i,j,a,b,0x0);
         draw_square(i+a,j+b,a,b,0x0);
      }
   }
}

//*====================================================================================================
//	체크판 테스트
//*====================================================================================================


//*====================================================================================================
//	드로우 라인 테스트
//*====================================================================================================
/*
#include <stdio.h>
#include <system.h>

volatile int pixel_buffer_start;

void plot_pixel(int x, int y, short int line_color);

void draw_line(int x0, int y0, int x1, int y2, short int line_color);

void clear_screen();
volatile int *pixel_ctrl_ptr = (int*)VGA_SUBSYSTEM_VGA_PIXEL_DMA_BASE;

int main(void)
{



   pixel_buffer_start = *pixel_ctrl_ptr;


   clear_screen();

   draw_line(0, 0, 319, 239, 0x001F);//b

   draw_line(0, 150, 250, 20, 0x07E0);//g

   draw_line(10, 135, 300, 140, 0xF800);//r

   //draw_line(319, 0, 0, 239, 0xF81F);pink

}


void clear_screen()

{

   for (int x = 0; x<320; x++)

      for (int y = 0; y<240; y++)

         plot_pixel(x, y, 0xffff);

}


void draw_line(int x0, int y0, int x1, int y1, short int line_color)

{

   int tempx, tempy, temp, y_step;

   tempx = (x1>x0) ? x1 - x0 : x0 - x1;

   tempy = (y1>y0) ? y1 - y0 : y0 - y1;

   int is_steep = (tempy>tempx) ? 1 : 0;

   if (is_steep)

   {

      temp = x0;

      x0 = y0;

      y0 = temp;

      temp = x1;

      x1 = y1;

      y1 = temp;

   }

   if (x0>x1)
   {

      temp = x0;

      x0 = x1;

      x1 = temp;

      temp = y0;

      y0 = y1;

      y1 = temp;

   }

   int deltax = x1 - x0;

   int deltay = (y1 > y0) ? y1 - y0 : y0 - y1;

   int error = -(deltax >> 1);

   int y = y0;

   if (y0<y1)  y_step = 1;

   else      y_step = -1;

   for (int x = x0; x < x1; x++)

   {

      if (is_steep) plot_pixel(y, x, line_color);

      else plot_pixel(x, y, line_color);

      error = error + deltay;

      if (error >= 0)

      {

         y = y + y_step;

         error = error - deltax;

      }

   }

}


void plot_pixel(int x, int y, short int line_color)

{

   *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;

}
*/
//*====================================================================================================
//	드로우 라인 테스트
//*====================================================================================================

//*====================================================================================================
//	픽셀 테스트
//*====================================================================================================
/*
#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>


void Timer_ISR();
void wait_for_vsync(void);
void fillColor(int rgb);
void plot_pixel(int x, int y, short int line_color);
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
   *(pTime_low)     = 0xE100;
   *(pTime_high)    = 0x05F0;
   *(pTimer+1)      = 0x0007;
   alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);
   *(pixel_ctrl_ptr + 1) = front_buffer;
      wait_for_vsync();

      pixel_buffer_start = *pixel_ctrl_ptr;
      clear_screen(0x00, 0x00, 0x00);
      *(pixel_ctrl_ptr + 1) = back_buffer;
   while(1);

   return 0;
}

void Timer_ISR() {
   *pTimer = 0;
   count ++;
   int temp = count%5;
   pixel_buffer_start = *(pixel_ctrl_ptr + 1);
   clear_screen(0x00, 0x00, 0x00);
   if(temp==0) fillColor(0xF800);
   else if(temp==1)fillColor(0x07E0);
   else if(temp==2)fillColor(0x001F);
   else if(temp==3)fillColor(0xFFFF);
   else if(temp==4)fillColor(0x0000);
   wait_for_vsync();
}

void fillColor(int rgb){
   for(int i=0;i<320;i++)
      for(int j=0;j<240;j++)
         plot_pixel(i, j, rgb);
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

void clear_screen(){
   for (int x = 0; x<320; x++)
      for (int y = 0; y<240; y++)
         plot_pixel(x, y, 0xffff);
}
*/
//*====================================================================================================
//	픽셀 테스트
//*====================================================================================================
