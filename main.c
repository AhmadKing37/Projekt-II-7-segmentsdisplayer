#include "setup.h"

struct button b1, b2, b3;
struct timer timer0;

/********************************************************************************
* setup: Initierar knappar och interrupts
********************************************************************************/
static inline void setup(void)
{
	
	button_init(&b1, 11);
	button_init(&b2, 12);
	button_init(&b3, 13);
	
	button_enable_interrupt(&b1);
	button_enable_interrupt(&b2);
	button_enable_interrupt(&b3);
	
	wdt_init(WDT_TIMEOUT_1024_MS);
	wdt_enable_system_reset();
	
	timer_init(&timer0, TIMER_SEL_0, 300);

	display_init();
	return;
}

/********************************************************************************
* main: Initierar systemet vid start. Uppräkning sker sedan kontinuerligt
*       av talet på 7-segmentsdisplayerna en gång per sekund.
********************************************************************************/
int main(void)
{
	setup();
	
	while (1)
	{
		wdt_reset();
	}

	return 0;
}