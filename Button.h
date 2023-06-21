#ifndef BUTTON_H_
#define BUTTON_H_

/* Inkluderingsdirektiv: */
#include "misc.h"

/********************************************************************************
* Strukt för implementering av tryckknappar och andra digitala inportar.
********************************************************************************/
struct button
{
	uint8_t pin;              // Tryckknappens pin-nummer på aktuell I/O-port. 
	volatile uint8_t* pullup; // Pekare till dataregister (för intern pullup-resistor).
	volatile uint8_t* input;  // Pekare till pinregister (för läsning av insignaler). 
	volatile uint8_t* pcmsk;  // Pekare till maskregister för aktivering av PCI-avbrott.
	volatile uint8_t pcint;   // Bit för aktivering av avbrottsvektor på aktuell I/O-port. 
};

/********************************************************************************
* Funktion för att initiera en knapp
********************************************************************************/
void button_init(struct button* self,
const uint8_t pin);

/********************************************************************************
* Funktion för att rensa en knapp
********************************************************************************/
void button_clear(struct button* self);

/********************************************************************************
* Funktion för att kontrollera om knappen är nedtryckt
********************************************************************************/
static inline bool button_is_pressed(const struct button* self)
{
	return (*(self->input) & (1 << self->pin));
}

/********************************************************************************
* Funktion för att kontrollera om avbrottet för given knapp är aktiverat
********************************************************************************/
static inline bool button_interrupt_enabled(const struct button* self)
{
	return (*(self->pcmsk) & (1 << self->pin));
}

/********************************************************************************
* Funktion för att aktivera avbrottet för knappen
*
*                          Nedan visas sambandet mellan använd I/O-port samt
*                          avbrottsvektorn för motsvarande avbrottsrutin:
*
*                          I/O-port     pin (Arduino Uno)     Avbrottsvektor
*                             B              8 - 13             PCINT0_vect
*                             C             A0 - A5             PCINT1_vect
*                             D              0 - 7              PCINT2_vect
*
*                          - self: Pekare till tryckknappen som PCI-avbrott
*                                  ska aktiveras på.
********************************************************************************/
void button_enable_interrupt(struct button* self);

/********************************************************************************
* Funktion för att inaktivera avbrottet för angiven knapp
********************************************************************************/
static inline void button_disable_interrupt(struct button* self)
{
	*(self->pcmsk) &= ~(1 << self->pin);
	return;
}

/********************************************************************************
* Funktion för att växla avbrottet för knappen
********************************************************************************/
void button_toggle_interrupt(struct button* self);

#endif /* BUTTON_H_ */