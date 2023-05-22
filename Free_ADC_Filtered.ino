#include<Arduino.h>
#define ADC_CLOCK_DIV 128
#define ADC_STARTUP_TIME 4
#define ADC_TRACKING_TIME 10
#define ADC_TRANSFER_TIME 1
#define FILTER_ORDER 4

volatile int16_t adcValue = 0;
volatile int16_t adcValue1=0;

int16_t temp;
volatile uint8_t lowByte; // Récupérer les 8 bits inférieurs
volatile uint8_t highByte; // Récupérer les 8 bits supérieurs
// attention, vous devez revoir la conception du filtre en fonction de la fréquence d'echantillonnage


int16_t filterBuffer[FILTER_ORDER + 1] = {0,0,0,0,0};
int16_t filterCoefficients[FILTER_ORDER + 1] = {8,35,53,35,8}; 

void setup() 
{ 
  Serial.begin(460800);
  PMC->PMC_PCER0 |= PMC_PCER0_PID27;
  ADC->ADC_CR = ADC_CR_SWRST; 
  ADC->ADC_MR |= ADC_MR_FREERUN_ON // Activation du mode de fonctionnement libre
              | ADC_MR_LOWRES_BITS_12 // résolution 12 bits
              | ADC_MR_TRACKTIM(ADC_TRACKING_TIME) // temps de suivi
              | ADC_MR_TRANSFER(ADC_TRANSFER_TIME) // temps de transfert
              | ADC_MR_STARTUP_SUT0 // temps de démarrage
              | ADC_MR_PRESCAL(ADC_CLOCK_DIV) ;// diviseur de fréquence
  ADC->ADC_CR=2; // Start converter- Demarrage convertisseur
  ADC->ADC_CHER = 0xC0; //Activation du canal ADC pour l'entrée du potentiomere
  ADC->ADC_COR = 0; // Désactivation de l'étalonnage
  ADC->ADC_IDR = 0xFFFFFFFF; // Désactivation de toutes les interruptions
}

// frequ fe=10.85*2=21160hz
void loop() 
{ 
  //Serial.print("A0--> ");
  adcValue = ADC->ADC_CDR[7];
  adcValue1=adcValue-2048;
  
  //Serial.println(adcValue);  
  //adcValue1=adcValue;
  applyFIRFilter(); // Appliquer le filtre
  //Serial.println(adcValue1);
  lowByte = adcValue1 & 0xFF; // Récupérer les 8 bits inférieurs
  highByte = (adcValue1 >> 8) & 0xFF; // Récupérer les 8 bits supérieurs
  
  // Envoyer les deux octets via le port série
  Serial.write(lowByte);
  Serial.write(highByte);
}

void applyFIRFilter()
{
  // Décaler les valeurs précédentes
  for(int i = FILTER_ORDER; i > 0; i--)
  {
    filterBuffer[i] = filterBuffer[i - 1];
  }

  // Ajouter la nouvelle valeur
  filterBuffer[0] = adcValue1;

  // Appliquer le filtre
  adcValue1 = 0;
  for(int i = 0; i <= FILTER_ORDER; i++)
  {
    temp=(int16_t) (filterBuffer[i] * filterCoefficients[i]);
    adcValue1 +=temp;
  }
}
