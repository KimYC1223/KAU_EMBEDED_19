#include "Timer.h"
#include "Img.h"

void HEX_PrintHEX(int number1, int number2, int number3);
char printHEX(int number);

volatile int *pixel_ctrl_ptr = (int*)0xff203020;
volatile int pixel_buffer_start;
volatile int VGA_MonitorPower = 1;
volatile int VGA_IsAlarmSettingMode = 0;
volatile int VGA_IsAlarmMode = 0;
short int front_buffer[512 * 256];
short int back_buffer[512 * 256];

void wait_for_vsync(void);
void imageDraw(int x,int y, short * img);
void clear_screen();

extern volatile int VGA_DrawMode = 0;
extern volatile int * pHEX4_5;
extern volatile int * pHEX0_3;
extern void Timer_ISR();

void Init_VGA() {
	Init_timer();
	*(pixel_ctrl_ptr + 1) = front_buffer;
	wait_for_vsync();

	pixel_buffer_start = *pixel_ctrl_ptr;
	*(pixel_ctrl_ptr + 1) = back_buffer;
}

void VGA_AlarmDraw_S(int drawType, int cursorPos, timeSet alarmTime) {
	if (VGA_MonitorPower == 1) {
		int x = 0, y = 0, offset_alarm_x = 0;
		if (drawType == 0) { x = 5; y = 5; }

		if (VGA_IsAlarmMode != 0) {
			imageDraw(x, y, IMG_Alarm);
			offset_alarm_x = IMG_Alarm[0];
		}

		imageDraw(x + offset_alarm_x, y, IMG_SetAlarm);
		offset_alarm_x += IMG_SetAlarm[0];

		if (alarmTime.hour > 12)
			imageDraw(x + offset_alarm_x, y, IMG_afternoon_L);
		else
			imageDraw(x + offset_alarm_x, y, IMG_morning_L);

		int offset_y = (int) IMG_afternoon_L[1];
		int offset_num_x = (int) IMG_Num0_S[0];
		int offset_clone_x = (int) IMG_Clone_S[0];

		int hour = ( alarmTime.hour < 25 && alarmTime.hour > 0 ) ? ( alarmTime.hour>12)?alarmTime.hour-12:alarmTime.hour : 0;
		int min = ( alarmTime.min < 60 && alarmTime.min > 0) ? alarmTime.min : 0;
		int sec = ( alarmTime.sec < 60 && alarmTime.sec > 0) ? alarmTime.sec : 0;

		imageDraw(x, y + offset_y, IMG_Number_S(hour / 10));
		imageDraw(x + offset_num_x, y + offset_y, IMG_Number_S(hour % 10));
		imageDraw(x + (offset_num_x * 2), y + offset_y, IMG_Clone_S);
		imageDraw(x + (offset_num_x * 2) + offset_clone_x, y + offset_y,IMG_Number_S(min / 10));
		imageDraw(x + (offset_num_x * 3) + offset_clone_x, y + offset_y,IMG_Number_S(min % 10));
		imageDraw(x + (offset_num_x * 4) + offset_clone_x, y + offset_y,IMG_Clone_S);
		imageDraw(x + (offset_num_x * 4) + (offset_clone_x * 2), y + offset_y,IMG_Number_S(sec / 10));
		imageDraw(x + (offset_num_x * 5) + (offset_clone_x * 2), y + offset_y,IMG_Number_S(sec % 10));
	}
}

void VGA_AlarmDraw_L(int drawType, int cursorPos, timeSet alarmTime) {
	if (VGA_MonitorPower == 1) {
		int x = 0, y = 0, offset_alarm_x = 0;
		if (drawType == 0) { x = 5; y = 5; }

		if (VGA_IsAlarmMode != 0) {
			imageDraw(x, y, IMG_Alarm);
			offset_alarm_x = IMG_Alarm[0];
		}

		imageDraw(x + offset_alarm_x, y, IMG_SetAlarm);
		offset_alarm_x += IMG_SetAlarm[0];

		if (alarmTime.hour > 12)
			imageDraw(x + offset_alarm_x, y, IMG_afternoon_L);
		else
			imageDraw(x + offset_alarm_x, y, IMG_morning_L);

		int offset_y = (int) IMG_afternoon_L[1];
		int offset_num_x = (int) IMG_Num0_L[0];
		int offset_clone_x = (int) IMG_Clone_L[0];

		int hour = ( alarmTime.hour < 25 && alarmTime.hour > 0 ) ? ( alarmTime.hour>12)?alarmTime.hour-12:alarmTime.hour : 0;
		int min = ( alarmTime.min < 60 && alarmTime.min > 0) ? alarmTime.min : 0;
		int sec = ( alarmTime.sec < 60 && alarmTime.sec > 0) ? alarmTime.sec : 0;

		imageDraw(x, y + offset_y, IMG_Number_L(hour / 10));
		imageDraw(x + offset_num_x, y + offset_y, IMG_Number_L(hour % 10));
		imageDraw(x + (offset_num_x * 2), y + offset_y, IMG_Clone_L);
		imageDraw(x + (offset_num_x * 2) + offset_clone_x, y + offset_y,IMG_Number_L(min / 10));
		imageDraw(x + (offset_num_x * 3) + offset_clone_x, y + offset_y,IMG_Number_L(min % 10));
		imageDraw(x + (offset_num_x * 4) + offset_clone_x, y + offset_y,IMG_Clone_L);
		imageDraw(x + (offset_num_x * 4) + (offset_clone_x * 2), y + offset_y,IMG_Number_L(sec / 10));
		imageDraw(x + (offset_num_x * 5) + (offset_clone_x * 2), y + offset_y,IMG_Number_L(sec % 10));
	}
}

void VGA_ClockDraw_L(int drawType, int cursorPos, timeSet currentTime) {
	if (VGA_MonitorPower == 1) {
		int x=0, y=0, offset_alarm_x=0;
		if (drawType == 0) { x = 5; y = 5; }

		if(VGA_IsAlarmMode != 0) {
			imageDraw(x,y,IMG_Alarm);
			offset_alarm_x = IMG_Alarm[0];
		}

		if (currentTime.hour > 12)
			imageDraw(x+offset_alarm_x ,y,IMG_afternoon_L);
		else imageDraw(x+offset_alarm_x,y,IMG_morning_L);

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
}

void VGA_ClockDraw_S(int drawType, int cursorPos, timeSet currentTime) {
	if (VGA_MonitorPower == 1) {
		int x=0, y=0, offset_alarm_x=0;
		if (drawType == 0) { x = 5; y = 5; }

		if(VGA_IsAlarmMode != 0) {
			imageDraw(x,y,IMG_Alarm);
			offset_alarm_x = IMG_Alarm[0];
		}

		if (currentTime.hour > 12)
			imageDraw(x+offset_alarm_x ,y,IMG_afternoon_L);
		else imageDraw(x+offset_alarm_x,y,IMG_morning_L);

		int offset_y = (int)IMG_afternoon_L[1];
		int offset_num_x = (int)IMG_Num0_S[0];
		int offset_clone_x = (int)IMG_Clone_S[0];

		int hour = ( currentTime.hour < 25 && currentTime.hour > 0 ) ? ( currentTime.hour>12)?currentTime.hour-12:currentTime.hour : 0;
		int min = ( currentTime.min < 60 && currentTime.min > 0) ? currentTime.min : 0;
		int sec = ( currentTime.sec < 60 && currentTime.sec > 0) ? currentTime.sec : 0;

		imageDraw(x,y+offset_y,IMG_Number_S(hour/10));
		imageDraw(x+offset_num_x,y+offset_y,IMG_Number_S(hour%10));
		imageDraw(x+(offset_num_x*2),y+offset_y,IMG_Clone_S);
		imageDraw(x+(offset_num_x*2)+offset_clone_x,y+offset_y,IMG_Number_S(min/10));
		imageDraw(x+(offset_num_x*3)+offset_clone_x,y+offset_y,IMG_Number_S(min%10));
		imageDraw(x+(offset_num_x*4)+offset_clone_x,y+offset_y,IMG_Clone_S);
		imageDraw(x+(offset_num_x*4)+(offset_clone_x*2),y+offset_y,IMG_Number_S(sec/10));
		imageDraw(x+(offset_num_x*5)+(offset_clone_x*2),y+offset_y,IMG_Number_S(sec%10));
	}
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

void HEX_PrintHEX(int number1, int number2, int number3) {
   int printNum1 = 0;
   int printNum2 = 0;
   int firstNum1 = printHEX(number1 % 10);
   int secondNum1 = (printHEX(number1 / 10)) << 8;
   int firstNum2 = printHEX(number2 % 10) << 16;
   int secondNum2 = (printHEX(number2 / 10)) << 24;
   int firstNum3 = printHEX(number3 % 10);
   int secondNum3 = (printHEX(number3 / 10)) << 8;
   printNum1 |= secondNum1 | firstNum1 | firstNum2 | secondNum2;
   printNum2 |= secondNum3 | firstNum3;
   *pHEX0_3 = printNum1;
   *pHEX4_5 = printNum2;
}

char printHEX(int number) {
   char result;
   switch (number) {
   case 0: result = 0b00111111; break;
   case 1: result = 0b00000110; break;
   case 2: result = 0b01011011; break;
   case 3: result = 0b01001111; break;
   case 4: result = 0b01100110; break;
   case 5: result = 0b01101101; break;
   case 6: result = 0b01111101; break;
   case 7: result = 0b00100111; break;
   case 8: result = 0b01111111; break;
   case 9: result = 0b01101111; break;
   }
   return result;
}
