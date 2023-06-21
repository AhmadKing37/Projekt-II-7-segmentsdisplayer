#include "button.h"

/********************************************************************************
* Initierar ny tryckknapp p� angiven pin.
********************************************************************************/
void button_init(struct button* self,
const uint8_t pin)
{
	// Kontrollera vilken I/O-port knappen tillh�r baserat p� pin-numret
	if (pin >= 0 && pin <= 7)
	{
		self->pin = pin;
		self->pullup = &PORTD;   // Dataregistret f�r intern pullup-resistor p� port D
		self->input = &PIND;     // Pinregistret f�r insignal p� port D
		self->pcmsk = &PCMSK2;   // Maskregistret f�r aktivering av PCI-avbrott p� PCINT23-16
		self->pcint = PCINT2;    // Bit f�r aktivering av avbrottsvektor p� port D (PCINT23-16)
	}
	else if (pin >= 8 && pin <= 13)
	{
		self->pin = pin - 8;
		self->pullup = &PORTB;   // Dataregistret f�r intern pullup-resistor p� port B
		self->input = &PINB;     // Pinregistret f�r insignal p� port B
		self->pcmsk = &PCMSK0;   // Maskregistret f�r aktivering av PCI-avbrott p� PCINT7-0
		self->pcint = PCINT0;    // Bit f�r aktivering av avbrottsvektor p� port B (PCINT7-0)
	}
	else if (pin >= 14 && pin <= 19)
	{
		self->pin = pin - 14;
		self->pullup = &PORTC;   // Dataregistret f�r intern pullup-resistor p� port C
		self->input = &PINC;     // Pinregistret f�r insignal p� port C
		self->pcmsk = &PCMSK1;   // Maskregistret f�r aktivering av PCI-avbrott p� PCINT14-8
		self->pcint = PCINT1;    // Bit f�r aktivering av avbrottsvektor p� port C (PCINT14-8)
	}

	// Aktivera intern pullup-resistor f�r knappen
	(*self->pullup) |= (1 << self->pin);
	return;
}

/********************************************************************************
* Funktion f�r att rensa en knapp
********************************************************************************/
void button_clear(struct button* self)
{
	button_disable_interrupt(self);
	(*self->pullup) &= ~(1 << self->pin);

	self->pin = 0;
	self->pullup = 0;
	self->input = 0;
	self->pcmsk = 0;
	self->pcint = 0;
	return;
}

/********************************************************************************
* Funktion f�r att aktivera avbrottet f�r knappen
********************************************************************************/
void button_enable_interrupt(struct button* self)
{
	asm("SEI");                          // Aktivera globala avbrott
	PCICR |= (1 << self->pcint);         // Aktivera PCI-avbrott f�r den aktuella porten
	*(self->pcmsk) |= (1 << self->pin);  // Aktivera avbrottet f�r den aktuella knappen
	return;
	return;
}

/********************************************************************************
* Funktion f�r att v�xla avbrottet f�r knappen.
* Kontrollera om avbrottet f�r knappen �r aktiverat
********************************************************************************/
void button_toggle_interrupt(struct button* self)
{
	if (button_interrupt_enabled(self))
	{
		button_disable_interrupt(self);   // Inaktivera avbrottet om det �r aktiverat
	}
	else
	{
		button_enable_interrupt(self);   // Aktivera avbrottet om det �r inaktiverat
	}

	return;
}