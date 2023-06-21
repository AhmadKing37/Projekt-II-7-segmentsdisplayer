#ifndef DISPLAY_H_
#define DISPLAY_H_

/********************************************************************************
* Inkluderingsdirektiv:
********************************************************************************/
#include "misc.h"
#include "timer.h"

/********************************************************************************
* Enumeration f�r val av uppr�kningsriktning p� 7-segmentsdisplayer.
********************************************************************************/
enum display_count_direction
{
	DISPLAY_COUNT_DIRECTION_UP   = 1,  // Uppr�kning upp�t. 
	DISPLAY_COUNT_DIRECTION_DOWN = 0   // Uppr�kning ned�t. 
};

/********************************************************************************
* Initierar h�rdvara f�r 7-segmentsdisplayer.
********************************************************************************/
void display_init(void);

/********************************************************************************
* �terst�ller 7-segmentsdisplayer till startl�get.
********************************************************************************/
void display_reset(void);

/********************************************************************************
* Indikerar ifall 7-segmentsdisplayerna �r p�slagna.
* Vid p�slagna displayer returneras true, annars false.
********************************************************************************/
bool display_output_enabled(void);

/********************************************************************************
* Indikerar ifall upp- eller nedr�kning av tal p� 7-segmentsdisplayerna �r aktiverat. 
* Ifall uppr�kning �r aktiverat returneras true, annars false.
********************************************************************************/
bool display_count_enabled(void);

/********************************************************************************
* S�tter p� 7-segmentsdisplayer.
********************************************************************************/
void display_enable_output(void);

/********************************************************************************
* St�nger av 7-segmentsdisplayer.
********************************************************************************/
void display_disable_output(void);

/********************************************************************************
* Togglar aktivering av 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_output(void);

/********************************************************************************
* S�tter nytt heltal f�r utskrift p� 7-segmentsdisplayer.
* Om angivet heltal �verstiger maxv�rdet som kan skrivas ut p� tv� 7-segmentsdisplayer med aktuell
* talbas returneras felkod 1. Annars returneras heltalet 0 efter att heltalet
*  p� 7-segmentsdisplayerna har uppdaterats.
********************************************************************************/
int display_set_number(const uint8_t new_number);

/********************************************************************************
*  S�tter ny talbas till 2, 10 eller 16 utskrift av tal p�
*  7-segmentsdisplayer. D�rmed kan tal skrivas ut bin�rt
*  (00 - 11), decimalt (00 - 99) eller hexadecimalt (00 - FF).
*  Vid felaktigt angiven talbas returneras felkod 1. Annars
*  returneras heltalet 0 efter att anv�nd talbas har
*  uppdaterats.
********************************************************************************/
int display_set_radix(const uint8_t new_radix);

/********************************************************************************
* Skiftar aktiverad 7-segmentsdisplay f�r utskrift av
* tiotal och ental, vilket �r n�dv�ndigt, d� displayerna
* delar p� samma pinnar. Enbart en v�rdesiffra skrivs
* ut om m�jligt, exempelvis 9 i st�llet f�r 09. 
********************************************************************************/
void display_toggle_digit(void);

/********************************************************************************
* R�knar upp eller ned tal p� 7-segmentsdisplayer.
********************************************************************************/
void display_count(void);

/********************************************************************************
* S�tter ny uppr�kningsriktning f�r tal som skrivs ut p� 7-segmentsdisplayer.
********************************************************************************/
void display_set_count_direction(const enum display_count_direction new_direction);

/********************************************************************************
* Togglar uppr�kningsriktning f�r tal som skrivs ut p� 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_count_direction(void);

/********************************************************************************
*  St�ller in upp- eller nedr�kning av tal som skrivs ut p� 7-segmentsdisplayerna med godtycklig uppr�kningshastighet.
********************************************************************************/
void display_set_count(const enum display_count_direction direction,
const uint16_t count_speed_ms);

/********************************************************************************
* Aktiverar upp- eller nedr�kning av tal som skrivs ut p�
* 7-segmentsdisplayerna. Som default anv�nds en
* uppr�kningshastighet p� 1000 ms om inget annat angetts
* (via anrop av funktionen display_set_count).
********************************************************************************/
void display_enable_count(void);

/********************************************************************************
* Inaktiverar upp- eller nedr�kning av tal som skrivs ut
* p� 7-segmentsdisplayerna.
********************************************************************************/
void display_disable_count(void);

/********************************************************************************
* Togglar upp- eller nedr�kning av tal som skrivs ut p�
* 7-segmentsdisplayerna.
********************************************************************************/
void display_toggle_count(void);

#endif /* DISPLAY_H_ */