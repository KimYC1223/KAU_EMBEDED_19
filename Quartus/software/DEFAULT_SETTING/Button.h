#include <system.h>
#include <sys/alt_irq.h>
#include <stdio.h>

extern volatile int * pKEY;
extern volatile int * pSwitch;
extern volatile int * pLED;
extern volatile int VGA_IsAlarmSettingMode;
extern volatile int VGA_IsAlarmMode;

//==========================================================================
//  다른 C 파일에서 가져오는 부분
//==========================================================================
extern void Audio_Record();
extern void Audio_Echo();
extern void Audio_Change();
extern void ALARM_SettingUp();
extern void ALARM_SettingDown();
extern void ALARM_SettingMove();
extern void ALARM_SettingSave();
extern void CLOCK_SettingUp();
extern void CLOCK_SettingDown();
extern void CLOCK_SettingMove();
extern void CLOCK_SettingSave();
//==========================================================================

void Pushbutton_ISR();
int mode = 0;

void Init_KEY_SW() {
	*(pKEY+2) = 0b1111;
	int pb_ctxt = 0;
	alt_irq_register(1, (void *)&pb_ctxt, (void *)Pushbutton_ISR);
}

void Pushbutton_ISR()
{
   ////update and reset key value////
   int key_value;
   key_value = *(pKEY + 3);
   *(pKEY + 3) = key_value;
   int swval = *pSwitch;


   //========================================================================================
   //	MODE 99 CONTROL
   //========================================================================================
   if (mode == 99) {
	   if ((swval == 0) && (key_value == 8))      mode = 0;
	   else if ((swval == 1) && (key_value == 8)) mode = 1;
	   else if ((swval == 2) && (key_value == 8)) mode = 2;
	   else if ((swval == 4) && (key_value == 8)) mode = 3;
	   else if ((swval == 8) && (key_value == 8)) mode = 4;

	   if (mode == 2) VGA_IsAlarmSettingMode = 1;
	   return;
   }

   //========================================================================================
   //	MODE 0 - MODE 4 CONTROL
   //========================================================================================
   else if ( mode >= 0 ) {

	   //====================================================================================
	   //	MODE 0 : 제너럴 모드
       //====================================================================================
	   if(mode == 0 ) {
		   if (key_value == 1)
			   VGA_MonitorPower = VGA_MonitorPower ^ 1;
		  else if(key_value == 2) {
			 // TO-DO : Tem / Hum read
			 printf("common key 1\n");
		  }
		  else if(key_value == 4)
			 VGA_IsAlarmMode = VGA_IsAlarmMode ^ 1;
		  else if(key_value == 8)
			  mode = 99;
	   }

	   //====================================================================================
	   //	MODE 1 : 시계 설정 모드
	   //====================================================================================
	   else if(mode == 1 ) {
		  if(key_value == 1)
			  CLOCK_SettingUp();
		  else if(key_value == 2)
			  CLOCK_SettingDown();
		  else if(key_value == 4)
			  CLOCK_SettingMove();
		  else if(key_value == 8)
			  CLOCK_SettingSave();
	   }

	   //====================================================================================
	   //	MODE 2 : 알람 설정 모드
	   //====================================================================================
	   else if(mode == 2 ) {
		   if (key_value == 1)
			   ALARM_SettingUp();
		   else if (key_value == 2)
			   ALARM_SettingDown();
		   else if (key_value == 4)
			   ALARM_SettingMove();
		   else if (key_value == 8)
			   ALARM_SettingSave();
	   } 
	   
	   //====================================================================================
	   //	MODE 3 : 카메라 설정 모드 ( 레이아웃 설정 모드 )
	   //====================================================================================
	   else if (mode == 3) {
		   if (key_value == 1) {}
		   else if (key_value == 2) {}
		   else if (key_value == 4) {}
		   else if (key_value == 8) {}
	   }
	   
	   //====================================================================================
	   //	MODE 4 : 오디오 설정 모드
	   //====================================================================================
	   else if(mode == 3 ) {
		   if (key_value == 1)
			   Audio_Record();
		   else if (key_value == 2)
			   Audio_Play();
		   else if (key_value == 4)
			   Audio_Change();
		   else if (key_value == 8) {
			   Audio_Mute();
			   mode = 0;
		   }
	   } else printf("change sw"); 
   }
}

