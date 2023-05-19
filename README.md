# ADC Due Free Runnig  mode 


# Fréquence d'échantillonnage de l'ADC et transmission en série

La fréquence à laquelle l'ADC fonctionne est déterminée par la fréquence du système divisée par le diviseur de présélection (prescaler) défini dans le registre de mode ADC (ADC_MR).

L'Arduino Due fonctionne à une fréquence système de 84 MHz. Dans le code proposé, le diviseur de présélection est défini comme ADC_CLOCK_DIV, qui est égal à 256.
Par conséquent, la fréquence de l'ADC serait de 84 MHz / 256 = 328,125 KHz.

## ADC et fréquence d'échantillonnage

Cependant, il est important de noter que la fréquence de l'ADC n'est pas la même chose que la fréquence d'échantillonnage.
La fréquence d'échantillonnage est déterminée par la combinaison de la fréquence de l'ADC et des paramètres de temps de suivi, de temps de règlement et de temps de transfert de l'ADC.

- Ces paramètres sont définis par `ADC_TRACKING_TIME`, `ADC_SETTLING_TIME_0` et `ADC_TRANSFER_TIME` dans le fichier ADCREERUN.ino.

La configuration exacte de ces paramètres et comment ils affectent la fréquence d'échantillonnage peuvent dépendre de plusieurs facteurs, y compris la spécification du microcontrôleur et la conception du circuit.

## Calcul de la fréquence d'échantillonnage

La fréquence d'échantillonnage de l'ADC est déterminée par la fréquence de l'ADC elle-même ainsi que par les temps de suivi, de règlement et de transfert.
Pour calculer la fréquence d'échantillonnage, vous devez connaître le nombre total de cycles de l'ADC nécessaires pour effectuer une conversion complète. 
Dans le cas de l'Arduino Due, l'ADC a une résolution de 12 bits, ce qui signifie qu'il nécessite 20 cycles pour une conversion complète (d'après la datasheet).
De plus, vous avez défini les temps de suivi et de transfert à 10 et 1 cycles respectivement. Par conséquent, une conversion complète nécessite en fait 20 + 10 + 1 = 31 cycles. 

Ensuite, vous devez diviser la fréquence de l'ADC par le nombre total de cycles pour obtenir la fréquence d'échantillonnage. 
Dans ce cas, l'ADC fonctionne à 328,125 KHz et il faut 31 cycles pour effectuer une conversion complète. Par conséquent, la fréquence d'échantillonnage de l'ADC serait de 328,125 KHz / 31 = 10,58 KHz.

## ADC et transmission en série
si le port série est configuré à 250000 bauds, vous pouvez envoyer au maximum 250000 / (2*8) = 15625 échantillons par seconde. 
Dans ce cas, la fréquence d'échantillonnage de l'ADC est maintenue, puisqu'elle est inférieure à la bande passante du port série.
