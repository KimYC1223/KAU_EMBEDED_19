/*
#include <system.h>

volatile int * video_in_ptr = (int*)0xff203060;
volatile int video_buffer_start;

extern int VGA_Cam;
extern void imageDraw(int x,int y, short * img);
extern volatile int * pixel_ctrl_ptr;

extern int VGA_Tem;
extern int VGA_Hum;
extern int VGA_Rec;
extern int VGA_Clock;
extern int VGA_Cam;

extern const int VGA_Tem_offset_x;
extern const int VGA_Tem_offset_y;
extern const int VGA_Hum_offset_x;
extern const int VGA_Hum_offset_y;
extern const int VGA_Rec_offset_x;
extern const int VGA_Rec_offset_y;
extern const int VGA_Clock_offset_x;
extern const int VGA_Clock_offset_y;

short VIDEO_Buffer[(320*240) +2 ];
int VIDEO_Size;

void VIDEO_View(int drawType) {
	if (VGA_MonitorPower == 1 && VGA_Cam == 1) {
		int cx = 0, cy = 0;
		if (drawType == 0) { cx = 5; cy = 5 + ((VGA_Rec==1)?VGA_Rec_offset_y:0)
											+ ((VGA_Tem==1)?VGA_Tem_offset_y:0)
											+ ((VGA_Hum==1)?VGA_Hum_offset_y:0)
											+ ((VGA_Clock==1)?VGA_Clock_offset_y:0);}
		video_buffer_start = *video_in_ptr;
		int dl = VIDEO_Size;
		int curx = (dl == 0)?80:(dl==1)?160:213;
		int cury = (dl == 0)?60:(dl==1)?120:160;
		int dx = (dl == 0)?4:2;
		int dy = (dl == 0)?4:2;

		int count = 2;
		for (int y = 0; y < 240; y = y + dy) {
			for (int x = 0; x < 320; x = x + dx) {
				VIDEO_Buffer[count++] = *(int *) ((video_buffer_start) + (y << 10) + (x << 1));
				if (dl == 2){
					if ((x % 3) == 0) dx = 1;
					else if ((x % 3) == 1) dx = 2;
				}
			}
			if (dl == 2) {
				if ((y % 3) == 0) dy = 1;
				else if ((y % 3) == 1) dy = 2;
			}
		}
		printf("%d\n",count);
		printf("%x\n",video_buffer_start);
		VIDEO_Buffer[0] = curx;
		VIDEO_Buffer[1] = cury;
		//printf("%d,%d,%d,%d,%d\n",VIDEO_Buffer[2],VIDEO_Buffer[7],*(int *) ((video_buffer_start) + (30 << 10) + (21 << 1)),VIDEO_Buffer[837],VIDEO_Buffer[2563]);
		imageDraw(cx,cy,VIDEO_Buffer);

	}
}

void VIDEO_Resize() {
	if ( VGA_Cam == 0 ) { VGA_Cam = 1; VIDEO_Size = 0; printf("video 1/4\n");}
	else if (VIDEO_Size == 0) { VIDEO_Size = 1; printf("video 1/2\n");}
	else if (VIDEO_Size == 1) { VIDEO_Size = 2; printf("video 2/3\n");}
	else if (VIDEO_Size == 2) { VGA_Cam = 0; printf("video OFF\n");}
}

*/
