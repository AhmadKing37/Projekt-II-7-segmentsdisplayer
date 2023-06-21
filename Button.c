#include "button.h"

/********************************************************************************
* Initierar ny tryckknapp på angiven pin.
********************************************************************************/
void button_init(struct button* self,
const uint8_t pin)
{
	// Kontrollera vilken I/O-port knappen tillhör baserat på pin-numret
	if (pin >= 0 && pin <= 7)
	{
		self->pin = pin;
		self->pullup = &PORTD;   // Dataregistret för intern pullup-resistor på port D
		self->input = &PIND;     // Pinregistret för insignal på port D
		self->pcmsk = &PCMSK2;   // Maskregistret för aktivering av PCI-avbrott på PCINT23-16
		self->pcint = PCINT2;    // Bit för aktivering av avbrottsvektor på port D (PCINT23-16)
	}
	else if (pin >= 8 && pin <= 13)
	{
		self->pin = pin - 8;
		self->pullup = &PORTB;   // Dataregistret för intern pullup-resistor på port B
		self->input = &PINB;     // Pinregistret för insignal på port B
		self->pcmsk = &PCMSK0;   // Maskregistret för aktivering av PCI-avbrott på PCINT7-0
		self->pcint = PCINT0;    // Bit för aktivering av avbrottsvektor på port B (PCINT7-0)
	}
	else if (pin >= 14 && pin <= 19)
	{
		self->pin = pin - 14;
		self->pullup = &PORTC;   // Dataregistret för intern pullup-resistor på port C
		self->input = &PINC;     // Pinregistret för insignal på port C
		self->pcmsk = &PCMSK1;   // Maskregistret för aktivering av PCI-avbrott på PCINT14-8
		self->pcint = PCINT1;    // Bit för aktivering av avbrottsvektor på port C (PCINT14-8)
	}

	// Aktivera intern pullup-resistor för knappen
	(*self->pullup) |= (1 << self->pin);
	return;
}

/********************************************************************************
* Funktion för att rensa en knapp
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
* Funktion för att aktivera avbrottet för knappen
********************************************************************************/
void button_enable_interrupt(struct button* self)
{
	asm("SEI");                          // Aktivera globala avbrott
	PCICR |= (1 << self->pcint);         // Aktivera PCI-avbrott för den aktuella porten
	*(self->pcmsk) |= (1 << self->pin);  // Aktivera avbrottet för den aktuella knappen
	return;
	return;
}

/********************************************************************************
* Funktion för att växla avbrottet för knappen.
* Kontrollera om avbrottet för knappen är aktiverat
********************************************************************************/
void button_toggle_interrupt(struct button* self)
{
	if (button_interrupt_enabled(self))
	{
		button_disable_interrupt(self);   // Inaktivera avbrottet om det är aktiverat
	}
	else
	{
		button_enable_interrupt(self);   // Aktivera avbrottet om det är inaktiverat
	}

	return;
}