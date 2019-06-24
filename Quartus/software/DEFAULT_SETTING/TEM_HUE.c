#include <stdlib.h>

#define TEM_MAX 80
#define TEM_MIN 10
#define HUM_MIN 0
#define HUM_MAX 100

volatile double ADC_Temperature = 0;
volatile double ADC_Humidity = 0;
volatile int * pADC = (int *)ADC_BASE;


void ADC_GetData() {
	// TO-DO : Humidity & Temperature Check
	//===========================================================
	//	GET TEMPERATURE
	//===========================================================
	ADC_Temperature = ((double) *(pADC))*0.07;

	//===========================================================
	//	GET HUMIDITY
	//===========================================================
	ADC_Humidity = ((double)*(pADC+1)) * 0.09;
}

double ADC_GetHumidity() {
	ADC_GetData();
	return ADC_Humidity;
}

double ADC_GetTemperature(){
	ADC_GetData();
	return ADC_Temperature;
}
