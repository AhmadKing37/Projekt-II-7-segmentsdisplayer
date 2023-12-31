#include "display.h"
#include "eeprom.h"

/********************************************************************************
* Makrodefinitioner:
********************************************************************************/
#define DISPLAY1_CATHODE PORTD7 // Katod f�r display 1. 
#define DISPLAY2_CATHODE PORTC3 // Katod f�r display 2. 

#define DISPLAY1_ON  PORTD &= ~(1 << DISPLAY1_CATHODE) // T�nder display 1. 
#define DISPLAY2_ON  PORTC &= ~(1 << DISPLAY2_CATHODE) // T�nder display 2. 
#define DISPLAY1_OFF PORTD |= (1 << DISPLAY1_CATHODE)  // Sl�cker display 1. 
#define DISPLAY2_OFF PORTC |= (1 << DISPLAY2_CATHODE)  // Sl�cker display 2. 

#define OFF 0x00   // Bin�rkod f�r sl�ckning av 7-segmentsdisplay. */
#define ZERO 0x3F  // Bin�rkod f�r utskrift av heltalet 0 p� 7-segmentsdisplay. 
#define ONE 0x06   // Bin�rkod f�r utskrift av heltalet 1 p� 7-segmentsdisplay. 
#define TWO 0x5B   // Bin�rkod f�r utskrift av heltalet 2 p� 7-segmentsdisplay. 
#define THREE 0x4F // Bin�rkod f�r utskrift av heltalet 3 p� 7-segmentsdisplay. 
#define FOUR 0x66  // Bin�rkod f�r utskrift av heltalet 4 p� 7-segmentsdisplay. 
#define FIVE 0x6D  // Bin�rkod f�r utskrift av heltalet 5 p� 7-segmentsdisplay. 
#define SIX 0x7D   // Bin�rkod f�r utskrift av heltalet 6 p� 7-segmentsdisplay. 
#define SEVEN 0x07 // Bin�rkod f�r utskrift av heltalet 7 p� 7-segmentsdisplay. 
#define EIGHT 0x7F // Bin�rkod f�r utskrift av heltalet 8 p� 7-segmentsdisplay. 
#define NINE 0x6F  // Bin�rkod f�r utskrift av heltalet 9 p� 7-segmentsdisplay. 
#define A 0x77     // Bin�rkod f�r utskrift av heltalet 0xA (10) p� 7-segmentsdisplay. 
#define B 0x7C     // Bin�rkod f�r utskrift av heltalet 0xB (11) p� 7-segmentsdisplay. 
#define C 0x39     // Bin�rkod f�r utskrift av heltalet 0xC (12) p� 7-segmentsdisplay. 
#define D 0x5E     // Bin�rkod f�r utskrift av heltalet 0xD (13) p� 7-segmentsdisplay. 
#define E 0x79     // Bin�rkod f�r utskrift av heltalet 0xE (14) p� 7-segmentsdisplay. 
#define F 0x71     // Bin�rkod f�r utskrift av heltalet 0xF (15) p� 7-segmentsdisplay. 

/********************************************************************************
Adresser i EEPROM f�r olika inst�llningar
********************************************************************************/
#define EEPROM_NUMBER          100	
#define EEPROM_OUTPUT_ENABLED  101	
#define EEPROM_COUNT_ENABLED   102
#define EEPROM_COUNT_DIRECTION 103
#define EEPROM_INITIALIZED     104

/********************************************************************************
* Enumeration f�r selektion av de olika displayerna.
********************************************************************************/
enum display_digit
{
	DISPLAY_DIGIT1, /* Display 1, som visar tiotal. */
	DISPLAY_DIGIT2  /* Display 2, som visar ental. */
};

/********************************************************************************
* Funktion f�r att uppdatera vilken siffra som ska visas p� en specifik display
********************************************************************************/
static inline void display_update_output(const uint8_t digit);

/********************************************************************************
* Funktion f�r att konvertera en siffra till dess bin�ra kod
********************************************************************************/
static inline uint8_t display_get_binary_code(const uint8_t digit);

/********************************************************************************
* Funktion f�r att kontrollera v�rdena i EEPROM
********************************************************************************/
static inline void check_eeprom_values(void);

/********************************************************************************
* Funktion f�r att �terst�lla v�rdena i EEPROM
********************************************************************************/
static inline void reset_eeprom_values(void);

/********************************************************************************
* Variabler f�r att h�lla reda p� olika tillst�nd och v�rden
*   - timer_digit      : Timerkrets f�r att skifta displayer (Timer 1).
*   - timer_count_speed: Timerkrets f�r uppr�kning av heltal (Timer 2).
********************************************************************************/
static uint8_t number = 0;
static uint8_t digit1 = 0;
static uint8_t digit2 = 0;
static uint8_t radix = 10;
static uint8_t max_val = 99;

static enum display_count_direction count_direction = DISPLAY_COUNT_DIRECTION_UP;
static enum display_digit current_digit = DISPLAY_DIGIT1;

static struct timer timer_digit;
static struct timer timer_count_speed;

/********************************************************************************
* display_init: Initierar h�rdvara f�r 7-segmentsdisplayer.
********************************************************************************/
void display_init(void)
{
	DDRD = 0xFF;
	DDRC |= (1 << DISPLAY2_CATHODE);
	DISPLAY1_OFF;
	DISPLAY2_OFF;

	timer_init(&timer_digit, TIMER_SEL_1, 1);
	timer_init(&timer_count_speed, TIMER_SEL_2, 1000);
	check_eeprom_values();
	return;
}

/********************************************************************************
* �terst�ller 7-segmentsdisplayer till startl�get.
********************************************************************************/
void display_reset(void)
{
	timer_reset(&timer_digit);
	timer_reset(&timer_count_speed);
	DISPLAY1_OFF;
	DISPLAY2_OFF;

	number = 0;
	digit1 = 0;
	digit2 = 0;
	radix = 10;
	max_val = 99;

	count_direction = DISPLAY_COUNT_DIRECTION_UP;
	current_digit = DISPLAY_DIGIT1;
	reset_eeprom_values();
	return;
}

/********************************************************************************
* Indikerar ifall 7-segmentsdisplayerna �r p�slagna.
* Vid p�slagna displayer returneras true, annars false.
********************************************************************************/
bool display_output_enabled(void)
{
	eeprom_write_byte(EEPROM_OUTPUT_ENABLED, 1);
	return timer_interrupt_enabled(&timer_digit);
}

/********************************************************************************
* Indikerar ifall upp- eller nedr�kning av tal p� 7-segmentsdisplayerna �r aktiverat.
* Ifall uppr�kning �r aktiverat returneras true, annars false.
********************************************************************************/
bool display_count_enabled(void)
{
	eeprom_write_byte(EEPROM_COUNT_ENABLED, 1);
	return timer_interrupt_enabled(&timer_count_speed);
}

/********************************************************************************
* S�tter p� 7-segmentsdisplayer.
********************************************************************************/
void display_enable_output(void)
{
	timer_enable_interrupt(&timer_digit);
	eeprom_write_byte(EEPROM_OUTPUT_ENABLED, 1);
	return;
}

/********************************************************************************
* St�nger av 7-segmentsdisplayer.
********************************************************************************/
void display_disable_output(void)
{
	timer_reset(&timer_digit);
	eeprom_write_byte(EEPROM_OUTPUT_ENABLED, 0);
	DISPLAY1_OFF;
	DISPLAY2_OFF;
	return;
}

/********************************************************************************
* Togglar aktivering av 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_output(void)
{
	if (display_output_enabled())
	{
		display_disable_output();
	}
	else
	{
		display_enable_output();
	}
	return;
}

/********************************************************************************
* Funktion f�r att s�tta ett nytt tal att visas p� displayen
********************************************************************************/
int display_set_number(const uint8_t new_number)
{
	if (new_number <= max_val)
	{
		number = new_number;
		digit1 = number / radix;
		digit2 = number - digit1 * radix;
		
		eeprom_write_byte(EEPROM_NUMBER, number);
		return 0;
	}
	else
	{
		return 1;
	}
}

/********************************************************************************
* S�tter ny talbas till 2, 10 eller 16 utskrift av tal p� 7-segmentsdisplayer. 
* D�rmed kan tal skrivas ut bin�rt (00 - 11), decimalt (00 - 99) eller hexadecimalt (00 - FF).
********************************************************************************/
int display_set_radix(const uint8_t new_radix)
{
	if (new_radix == 2 || new_radix == 10 || new_radix == 16)
	{
		radix = new_radix;
		max_val = radix * radix - 1;
		return 0;
	}
	else
	{
		return 1;
	}
}

/********************************************************************************
* Skiftar aktiverad 7-segmentsdisplay f�r utskrift av
* tiotal och ental, vilket �r n�dv�ndigt, d� displayerna
* delar p� samma pinnar. Enbart en v�rdesiffra skrivs
* ut om m�jligt, exempelvis 9 i st�llet f�r 09. Denna
* funktion b�r anropas en g�ng per millisekund f�r att
* ett givet tv�siffrigt tal ska upplevas skrivas ut kontinuerligt.
********************************************************************************/
void display_toggle_digit(void)
{
	timer_count(&timer_digit);

	if (timer_elapsed(&timer_digit))
	{
		current_digit = !current_digit;

		if (current_digit == DISPLAY_DIGIT1)
		{
			DISPLAY2_OFF;

			if (digit1 == 0)
			{
				DISPLAY1_OFF;
			}
			else
			{
				display_update_output(digit1);
				DISPLAY1_ON;
			}
		}
		else
		{
			DISPLAY1_OFF;
			display_update_output(digit2);
			DISPLAY2_ON;
		}
	}
	return;
}

/********************************************************************************
* display_count: R�knar upp eller ned tal p� 7-segmentsdisplayer.
*
*                1. R�knar upp l�pt tid via timern timer_count_speed.
*                2. N�r timern l�pt ut sker upp/nedr�kning.
*                3. Vid uppr�kning, inkrementera variabeln number upp till
*                   och med aktuellt maxv�rde max_count, annars nollst�ll.
*                4. Vid nedr�kning, dekrementera variabeln number till 0,
*                   d�refter s�tt den till max_val.
*                5. Uppdatera tiotal och ental via anrop av funktionen
*                   display_set_number.
********************************************************************************/
void display_count(void)
{
	timer_count(&timer_count_speed);
	
	if (timer_elapsed(&timer_count_speed))
	{
		if (count_direction == DISPLAY_COUNT_DIRECTION_UP)
		{
			if (number >= max_val) number = 0;
			else number++;
		}
		else
		{
			if (number == 0) number = max_val;
			else number--;
		}
		display_set_number(number);
	}
	return;
}

/********************************************************************************
* display_set_count_direction: S�tter ny uppr�kningsriktning f�r tal som skrivs
*                              ut p� 7-segmentsdisplayer.
*
*                              - new_direction: Ny uppr�kningsriktning.
********************************************************************************/
void display_set_count_direction(const enum display_count_direction new_direction)
{
	count_direction = new_direction;
	eeprom_write_byte(EEPROM_COUNT_DIRECTION, (uint8_t)(new_direction));
	return;
}

/********************************************************************************
* display_toggle_count_direction: Togglar uppr�kningsriktning f�r tal som skrivs
*                                 ut p� 7-segmentsdisplayer.
********************************************************************************/
void display_toggle_count_direction(void)
{
	count_direction = !count_direction;
	
	eeprom_write_byte(EEPROM_COUNT_DIRECTION, (uint8_t)(count_direction));
	return;
}

/********************************************************************************
* display_set_count: St�ller in upp- eller nedr�kning av tal som skrivs ut p�
*                    7-segmentsdisplayerna med godtycklig uppr�kningshastighet.
*
*                    - direction     : Uppr�kningsriktning.
*                    - count_speed_ms: Uppr�kningshastighet m�tt i ms.
********************************************************************************/
void display_set_count(const enum display_count_direction direction,
const uint16_t count_speed_ms)
{
	count_direction = direction;
	timer_set_new_time(&timer_count_speed, count_speed_ms);
	return;
}

/********************************************************************************
* display_enable_count: Aktiverar upp- eller nedr�kning av tal som skrivs ut p�
*                       7-segmentsdisplayerna. Som default anv�nds en
*                       uppr�kningshastighet p� 1000 ms om inget annat angetts
*                       (via anrop av funktionen display_set_count).
********************************************************************************/
void display_enable_count(void)
{
	timer_enable_interrupt(&timer_count_speed);
	eeprom_write_byte(EEPROM_COUNT_ENABLED, 1);
	return;
}

/********************************************************************************
* display_disable_count: Inaktiverar upp- eller nedr�kning av tal som skrivs ut
*                        p� 7-segmentsdisplayerna.
********************************************************************************/
void display_disable_count(void)
{
	timer_reset(&timer_count_speed);
	eeprom_write_byte(EEPROM_COUNT_ENABLED, 0);
	return;
}

/********************************************************************************
* display_toggle_count: Togglar upp- eller nedr�kning av tal som skrivs ut p�
*                       7-segmentsdisplayerna.
********************************************************************************/
void display_toggle_count(void)
{
	if (display_count_enabled())
	{
		display_disable_count();
	}
	else
	{
		display_enable_count();
	}
	return;
}

/********************************************************************************
* display_update_output: Skriver ny siffra till aktiverad 7-segmentsdisplay.
********************************************************************************/
static inline void display_update_output(const uint8_t digit)
{
	PORTD &= (1 << DISPLAY1_CATHODE);
	PORTD |= display_get_binary_code(digit);
	return;
}

/********************************************************************************
* display_get_binary_code: Returnerar bin�rkod f�r angivet heltal 0 - 15 f�r
*                          utskrift p� 7-segmentsdisplayer. Vid felaktigt
*                          angivet heltal (�ver 15) returneras bin�rkod f�r
*                          sl�ckning av 7-segmentsdisplayer.
*
*                          - digit: Heltal vars bin�rkod ska returneras.
********************************************************************************/
static inline uint8_t display_get_binary_code(const uint8_t digit)
{
	if (digit == 0)       return ZERO;
	else if (digit == 1)  return ONE;
	else if (digit == 2)  return TWO;
	else if (digit == 3)  return THREE;
	else if (digit == 4)  return FOUR;
	else if (digit == 5)  return FIVE;
	else if (digit == 6)  return SIX;
	else if (digit == 7)  return SEVEN;
	else if (digit == 8)  return EIGHT;
	else if (digit == 9)  return NINE;
	else if (digit == 10) return A;
	else if (digit == 11) return B;
	else if (digit == 12) return C;
	else if (digit == 13) return D;
	else if (digit == 14) return E;
	else if (digit == 15) return F;
	else                  return OFF;
}

/********************************************************************************
* check_eeprom_values: Kollar om det finns n�gra sparade v�rden i EEPROM.
					   Om det finns l�ses dessa in och displayerna f�rs�tts 
					   i samma tillst�nd som innan programmet avslutades. 
					  
					   1. Vi kollar om EEPROM �r initierat. D� ligger v�rdet 0 
					      p� EEPROM_INITIALIZED, annars ligger startv�rdet 0xFF.
						  
					   2. Om EEPROM �r initierat l�ses v�rderna in, annars 
					      s�tts addresserna i starttillst�nd
						
					   3. F�rst l�ses det senaste talet in och sparas i variabeln
					      number. Tiotalet och entalet sparas i variabeln digit1
						  samt digit2 via anrop av dispaly_set_number.
						
					   4. L�ser in uppr�kningsriktningen och sparas i variabeln 
					      count_direction. Det inl�sta talet typomvandlas till 
						  enum dispaly_count_direction. 
					   
					   5.  
********************************************************************************/
static inline void check_eeprom_values(void)
{
	if (eeprom_read_byte(EEPROM_INITIALIZED) == 0)
	{
		display_set_number(eeprom_read_byte(EEPROM_NUMBER));
		count_direction = (enum display_count_direction)(eeprom_read_byte(EEPROM_COUNT_DIRECTION));
		
		if (eeprom_read_byte(EEPROM_OUTPUT_ENABLED) == 1)
		{
			display_enable_output();
		}
		
		if (eeprom_read_byte(EEPROM_COUNT_ENABLED) == 1)
		{
			display_enable_count();
		}
	}
	else
	{
		reset_eeprom_values();
	}
	return;
}

/********************************************************************************
* reset_eeprom_values: Skriver startv�rden till EEPROM. 
					   number = 0
					   output_enabled = false
					   count_enabled = false
					   count_direction = up
					   eeprom_initialized = 0 (st�r f�r sant).
********************************************************************************/
static inline void reset_eeprom_values(void)
{
	eeprom_write_byte(EEPROM_NUMBER, 0);
	eeprom_write_byte(EEPROM_OUTPUT_ENABLED, 0);
	eeprom_write_byte(EEPROM_COUNT_ENABLED, 0);
	eeprom_write_byte(EEPROM_COUNT_DIRECTION, (uint8_t)(DISPLAY_COUNT_DIRECTION_UP));
	eeprom_write_byte(EEPROM_INITIALIZED, 0);
	return;
}