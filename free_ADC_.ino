#include<Arduino.h>
#define ADC_CLOCK_DIV 256
#define ADC_STARTUP_TIME 4
#define ADC_TRACKING_TIME 10
#define ADC_TRANSFER_TIME 1
volatile int16_t adcValue = 0;
volatile uint8_t lowByte; // Récupérer les 8 bits inférieurs
volatile uint8_t highByte; // Récupérer les 8 bits supérieurs
void setup() 
{ Serial.begin(250000);
   PMC->PMC_PCER0 |= PMC_PCER0_PID27;
  ADC->ADC_CR = ADC_CR_SWRST; 
  ADC->ADC_MR |= ADC_MR_FREERUN_ON// Activation du mode de fonctionnement libre
               | ADC_MR_LOWRES_BITS_12 // résolution 12 bits
               | ADC_MR_TRACKTIM(ADC_TRACKING_TIME) // temps de suivi
               | ADC_MR_TRANSFER(ADC_TRANSFER_TIME) // temps de transfert
               | ADC_MR_STARTUP_SUT0 // temps de démarrage
               | ADC_MR_PRESCAL(ADC_CLOCK_DIV) ;// diviseur de fréquence
  ADC->ADC_CR=2;// Start converter- Demarrage convertisseur
  ADC->ADC_CHER = 0xC0;//Activation du canal ADC pour l'entrée du potentiomere
  ADC->ADC_COR = 0;// Désactivation de l'étalonnage
  ADC->ADC_IDR = 0xFFFFFFFF;// Désactivation de toutes les interruptions
  }
void loop() 
{ Serial.print("A0--> ");
adcValue = ADC->ADC_CDR[7];
adcValue=adcValue-2048;
lowByte = adcValue & 0xFF; // Récupérer les 8 bits inférieurs
highByte = (adcValue >> 8) & 0xFF; // Récupérer les 8 bits supérieurs
// Envoyer les deux octets via le port série
Serial.write(lowByte);
Serial.write(highByte);
//Serial.write(adcValue);

//  Serial.print(adcValue-2048);    //AD7
//  Serial.print("     A1--> "); 
//  Serial.println(ADC->ADC_CDR[6]);  //AD6
}


 