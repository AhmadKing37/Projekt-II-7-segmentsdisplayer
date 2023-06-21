#ifndef DISPLAY_H_
#define DISPLAY_H_

/********************************************************************************
* Inkluderingsdirektiv:
********************************************************************************/
#include "misc.h"
#include "timer.h"

/********************************************************************************
* Enumeration för val av uppräkningsriktning på 7-segmentsdisplayer.
********************************************************************************/
enum display_count_direction
{
	DISPLAY_COUNT_DIRECTION_UP   = 1,  // Uppräkning uppåt. 
	DISPLAY_COUNT_DIRECTION_DOWN = 0   // Uppräkning nedåt. 
};

/********************************************************************************
* Initierar hårdvara för 7-segmentsdisplayer.
********************************************************************************/
void display_init(void);

/********************************************************************************
* Återställer 7-segmentsdisplayer till startläget.
********************************************************************************/
void display_reset(void);

/********************************************************************************
* Indikerar ifall 7-segmentsdisplayerna är påslagna.
* Vid påslagna displayer returneras true, annars false.
********************************************************************************/
bool display_output_enabled(void);

/********************************************************************************
* Indikerar ifall upp- eller nedräkning av tal på 7-segmentsdisplayerna är aktiverat. 
* Ifall uppräkning är aktiverat returneras true, annars false.
********************************************************************************/
bool display_count_enabled(void);

/********************************************************************************
* Sätter på 7-segmentsdisplayer.
********************************************************************************/
void display_enable_output(void);

/********************************************************************************
* Stänger av 7-segmentsdisplayer.
********************************************************************************/
void display_disable_output(void);

/********************************************************************************
* Togglar aktivering av 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_output(void);

/********************************************************************************
* Sätter nytt heltal för utskrift på 7-segmentsdisplayer.
* Om angivet heltal överstiger maxvärdet som kan skrivas ut på två 7-segmentsdisplayer med aktuell
* talbas returneras felkod 1. Annars returneras heltalet 0 efter att heltalet
*  på 7-segmentsdisplayerna har uppdaterats.
********************************************************************************/
int display_set_number(const uint8_t new_number);

/********************************************************************************
*  Sätter ny talbas till 2, 10 eller 16 utskrift av tal på
*  7-segmentsdisplayer. Därmed kan tal skrivas ut binärt
*  (00 - 11), decimalt (00 - 99) eller hexadecimalt (00 - FF).
*  Vid felaktigt angiven talbas returneras felkod 1. Annars
*  returneras heltalet 0 efter att använd talbas har
*  uppdaterats.
********************************************************************************/
int display_set_radix(const uint8_t new_radix);

/********************************************************************************
* Skiftar aktiverad 7-segmentsdisplay för utskrift av
* tiotal och ental, vilket är nödvändigt, då displayerna
* delar på samma pinnar. Enbart en värdesiffra skrivs
* ut om möjligt, exempelvis 9 i stället för 09. 
********************************************************************************/
void display_toggle_digit(void);

/********************************************************************************
* Räknar upp eller ned tal på 7-segmentsdisplayer.
********************************************************************************/
void display_count(void);

/********************************************************************************
* Sätter ny uppräkningsriktning för tal som skrivs ut på 7-segmentsdisplayer.
********************************************************************************/
void display_set_count_direction(const enum display_count_direction new_direction);

/********************************************************************************
* Togglar uppräkningsriktning för tal som skrivs ut på 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_count_direction(void);

/********************************************************************************
*  Ställer in upp- eller nedräkning av tal som skrivs ut på 7-segmentsdisplayerna med godtycklig uppräkningshastighet.
********************************************************************************/
void display_set_count(const enum display_count_direction direction,
const uint16_t count_speed_ms);

/********************************************************************************
* Aktiverar upp- eller nedräkning av tal som skrivs ut på
* 7-segmentsdisplayerna. Som default används en
* uppräkningshastighet på 1000 ms om inget annat angetts
* (via anrop av funktionen display_set_count).
********************************************************************************/
void display_enable_count(void);

/********************************************************************************
* Inaktiverar upp- eller nedräkning av tal som skrivs ut
* på 7-segmentsdisplayerna.
********************************************************************************/
void display_disable_count(void);

/********************************************************************************
* Togglar upp- eller nedräkning av tal som skrivs ut på
* 7-segmentsdisplayerna.
********************************************************************************/
void display_toggle_count(void);

#endif /* DISPLAY_H_ */