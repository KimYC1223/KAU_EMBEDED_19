#include <stdio.h>
#include <system.h>

volatile int *video_ctrl_ptr = (int*)VIDEO_IN_SUBSYSTEM_VIDEO_IN_DMA_BASE;


int main() {
   *(video_ctrl_ptr+3) = 4;
   return 0;
}
