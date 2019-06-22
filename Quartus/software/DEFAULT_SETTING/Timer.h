#include <system.h>
#include <sys/alt_irq.h>

struct _Time {
	int hour;
	int min;
	int sec;
} typedef timeSet;

void Timer_ISR();
volatile int * pTimer       = (int *)0xFF202000;
volatile int * pTime_low    = (int *)0xFF202008;
volatile int * pTime_high    = (int *)0xFF20200C;
volatile int set_time[6] = { 0,0,0,0,0,0 };
volatile int alarm_time[6] = { 0,0,0,0,0,0 };

extern volatile int *pixel_ctrl_ptr;
extern volatile int pixel_buffer_start;
extern volatile int VGA_DrawMode;
extern void VGA_ClockDraw_L(int drawType, int cursorPos, timeSet currentTime);
extern void VGA_AlarmDraw(int drawType, int cursorPos, timeSet alarmTime);
extern void clear_screen();
extern volatile int VGA_IsAlarmSettingMode;
extern volatile int VGA_IsAlarmMode;
extern int mode;

timeSet currentTime;
timeSet alarmTime;
int alarm_cur = 0;
int set_cur = 0;
int small_sec = 0;
int LargeNumber = 0;

void Timer_ISR() {
	*pTimer = 0;
	pixel_buffer_start = *(pixel_ctrl_ptr + 1);
	clear_screen();

	small_sec++;
	if (small_sec == 2) {
		add_sec();
		small_sec = 0;
	}

	if (LargeNumber != 0) {
		if (VGA_IsAlarmSettingMode != 0)
			VGA_AlarmDraw_L(VGA_DrawMode,0,alarmTime);
		else VGA_ClockDraw_L(VGA_DrawMode,0,currentTime);
	} else {
		if (VGA_IsAlarmSettingMode != 0)
			VGA_AlarmDraw_S(VGA_DrawMode,0,alarmTime);
		else VGA_ClockDraw_S(VGA_DrawMode,0,currentTime);
	}
	wait_for_vsync();
}

void Init_timer() {
	int pb_ctxt=0;
	*(pTime_low)     = 0xF07F;
	*(pTime_high)    = 0x02FA;
	*(pTimer+1)      = 0x0007;
	alt_irq_register(0, (void *)&pb_ctxt, (void *)Timer_ISR);
	currentTime.hour = 24 ;
	currentTime.min = 0 ;
	currentTime.sec = 0 ;
}


int change_num(int max, int val, int ud)
{
	int temp;
	temp = val;
	if (ud == 1)
	{
		temp++;
		if (temp == max) temp = 0;
	}
	if (ud == 2)
	{
		if (temp == 0) temp = max - 1;
		else temp--;
	}
	return temp;
}

void add_sec() {
	currentTime.sec++;
	if (currentTime.sec >= 60){
		currentTime.sec = 0;
		add_min();
	}
}

void add_min() {
	currentTime.min++;
	if (currentTime.min >= 60) {
		currentTime.min = 0;
		add_hour();
	}
}

void add_hour() {
	currentTime.hour++;
	if (currentTime.hour >= 24)	{
		currentTime.hour = 0;
	}
}

void ALARM_SettingUp() {
	int max = 0;
	if (alarm_cur == 0) max = 10;
	else if (alarm_cur == 1) max = 6;
	else if (alarm_cur == 2) max = 10;
	else if (alarm_cur == 3) max = 6;
	else if (alarm_cur == 4)
	{
		if (alarm_time[5] == 2) max = 4;
		else max = 10;
	}
	alarm_time[alarm_cur] = change_num(max, alarm_time[alarm_cur], 1);
	alarmTime.sec = alarm_time[1] * 10 + alarm_time[0];
	alarmTime.min = alarm_time[3] * 10 + alarm_time[2];
	alarmTime.hour = alarm_time[5] * 10 + alarm_time[4];
	if (((alarmTime.sec > 0) || (alarmTime.min > 0)) && (alarmTime.hour > 23))alarmTime.hour = alarmTime.hour - 24;
}

void ALARM_SettingDown() {
	int max = 0;
	if (alarm_cur == 0) max = 10;
	else if (alarm_cur == 1) max = 6;
	else if (alarm_cur == 2) max = 10;
	else if (alarm_cur == 3) max = 6;
	else if (alarm_cur == 4)
	{
		if (alarm_time[5] == 2) max = 4;
		else max = 10;
	}
	alarm_time[alarm_cur] = change_num(max, alarm_time[alarm_cur], 2);
	alarmTime.sec = alarm_time[1] * 10 + alarm_time[0];
	alarmTime.min = alarm_time[3] * 10 + alarm_time[2];
	alarmTime.hour = alarm_time[5] * 10 + alarm_time[4];
	if (((alarmTime.sec > 0) || (alarmTime.min > 0)) && (alarmTime.hour > 23))alarmTime.hour = alarmTime.hour - 24;
}

void ALARM_SettingMove() {
	alarm_cur++;
	if (alarm_cur > 5) alarm_cur = 0;
}

void ALARM_SettingSave() {
	VGA_IsAlarmMode = 1;
	VGA_IsAlarmSettingMode = 0;
	mode = 0;
}

void CLOCK_SettingUp() {
	int max = 0;
	if (set_cur == 0) max = 10;
	else if (set_cur == 1) max = 6;
	else if (set_cur == 2) max = 10;
	else if (set_cur == 3) max = 6;
	else if (set_cur == 4)
	{
		if (set_time[5] == 2) max = 4;
		else max = 10;
	}
	set_time[set_cur] = change_num(max, set_time[set_cur], 1);
	currentTime.sec = set_time[1] * 10 + set_time[0];
	currentTime.min = set_time[3] * 10 + set_time[2];
	currentTime.hour = set_time[5] * 10 + set_time[4];
	if (((currentTime.sec > 0) || (currentTime.min > 0)) && (currentTime.hour > 23))currentTime.hour = currentTime.hour - 24;
}

void CLOCK_SettingDown() {
	int max = 0;
	if (set_cur == 0) max = 10;
	else if (set_cur == 1) max = 6;
	else if (set_cur == 2) max = 10;
	else if (set_cur == 3) max = 6;
	else if (set_cur == 4)
	{
		if (set_time[5] == 2) max = 4;
		else max = 10;
	}
	set_time[set_cur] = change_num(max, set_time[set_cur], 2);
	currentTime.sec = set_time[1] * 10 + set_time[0];
	currentTime.min = set_time[3] * 10 + set_time[2];
	currentTime.hour = set_time[5] * 10 + set_time[4];
	if (((currentTime.sec > 0) || (currentTime.min > 0)) && (currentTime.hour > 23))currentTime.hour = currentTime.hour - 24;
}

void CLOCK_SettingMove() {
	set_cur++;
	if (set_cur > 5) set_cur = 0;
}

void CLOCK_SettingSave() {
	mode = 0;
}

void Alarm_Checking() {
	if (VGA_IsAlarmMode == 0) return;
	VGA_IsAlarmMode = 0;
	Audio_Play();
}
