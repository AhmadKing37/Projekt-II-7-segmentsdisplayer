#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* Strukt f�r implementering av tryckknappar och andra digitala inportar.
********************************************************************************/
struct button
{
	uint8_t pin;              // Tryckknappens pin-nummer p� aktuell I/O-port. 
	volatile uint8_t* pullup; // Pekare till dataregister (f�r intern pullup-resistor).
	volatile uint8_t* input;  // Pekare till pinregister (f�r l�sning av insignaler). 
	volatile uint8_t* pcmsk;  // Pekare till maskregister f�r aktivering av PCI-avbrott.
	volatile uint8_t pcint;   // Bit f�r aktivering av avbrottsvektor p� aktuell I/O-port. 
};

/********************************************************************************
* Funktion f�r att initiera en knapp
********************************************************************************/
void button_init(struct button* self,
const uint8_t pin);

/********************************************************************************
* Funktion f�r att rensa en knapp
********************************************************************************/
void button_clear(struct button* self);

/********************************************************************************
* Funktion f�r att kontrollera om knappen �r nedtryckt
********************************************************************************/
static inline bool button_is_pressed(const struct button* self)
{
	return (*(self->input) & (1 << self->pin));
}

/********************************************************************************
* Funktion f�r att kontrollera om avbrottet f�r given knapp �r aktiverat
********************************************************************************/
static inline bool button_interrupt_enabled(const struct button* self)
{
	return (*(self->pcmsk) & (1 << self->pin));
}

/********************************************************************************
* Funktion f�r att aktivera avbrottet f�r knappen
*
*                          Nedan visas sambandet mellan anv�nd I/O-port samt
*                          avbrottsvektorn f�r motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras p�.
********************************************************************************/
void button_enable_interrupt(struct button* self);

/********************************************************************************
* Funktion f�r att inaktivera avbrottet f�r angiven knapp
********************************************************************************/
static inline void button_disable_interrupt(struct button* self)
{
	*(self->pcmsk) &= ~(1 << self->pin);
	return;
}

/********************************************************************************
* Funktion f�r att v�xla avbrottet f�r knappen
********************************************************************************/
void button_toggle_interrupt(struct button* self);

#endif /* BUTTON_H_ */