#include <stdlib.h>

#define TEM_MAX 23
#define TEM_MIN 19
#define HUM_MIN 38
#define HUM_MAX 42
#define HUM_CNT 5
#define TEM_CNT 7

volatile double ADC_Temperature = 22;
volatile double ADC_Humidity = 39;
int ADC_TemperatureCount = HUM_CNT;
int ADC_HumidityCount = TEM_CNT;

void ADC_GetData() {
	// TO-DO : Humidity & Temperature Check
	//===========================================================
	//	GET TEMPERATURE
	//===========================================================
	if (ADC_TemperatureCount == 0) {
		srand(ADC_HumidityCount);

		// random range [-2 ~ 2]
		int random = (rand() % 5) - 2;
		ADC_TemperatureCount = TEM_CNT + random;

		double random2 = (ADC_Temperature <= (double) TEM_MIN) ? 0.1 :
							(ADC_Temperature >= (double) TEM_MAX) ? -0.1 :
							((rand() % 2) == 0) ? 0.1 : ((rand() % 2) == 0)? 0 : -0.1;
		ADC_Temperature = ADC_Temperature + random2;
	}

	//===========================================================
	//	GET HUMIDITY
	//===========================================================
	else if (ADC_HumidityCount == 0) {
		srand(ADC_TemperatureCount);

		// random range [-2 ~ 2]
		int random = (rand() % 5) - 2;
		ADC_HumidityCount = HUM_CNT + random;

		double random2 = (ADC_Humidity <= (double) HUM_MIN) ? 0.1 :
							(ADC_Humidity >= (double) HUM_MAX) ? -0.1 :
							((rand() % 2) == 0) ? 0.1 : ((rand() % 2) == 0)? 0 : -0.1;
		ADC_Humidity = ADC_Humidity + random2;
	}
}

double ADC_GetHumidity() {
	ADC_HumidityCount--;
	ADC_GetData();
	return ADC_Humidity;
}

double ADC_GetTemperature(){
	ADC_TemperatureCount--;
	ADC_GetData();
	return ADC_Temperature;
}
