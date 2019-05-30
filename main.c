
/* Name: main.c
 * Project: V-USB MIDI device on Low-Speed USB
 * Author: Martin Homuth-Rosemann
 * Creation Date: 2008-03-11
 * Copyright: (c) 2008 by Martin Homuth-Rosemann.
 * License: GPL.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "usbdrv/usbdrv.h"
#include "usb.h"
#include "midi.h"

#define KEY (1 << PB0)
#define KEY_COUNT 4
#define FALSE 0
#define TRUE 1


volatile uint8_t sent = TRUE;
MIDI_EVENT event = {MIDI_CMD_NOTE_OFF, 0x00, 0x3D, 0x40};

enum
{
	NONE,
	PRESSED,
	DEPRESSED
};

uint8_t keyPressed(uint8_t *lock, volatile uint8_t *PINX, uint8_t pinNumber);

int main(void)
{
	uint8_t i;
	uint8_t keyLocks[KEY_COUNT];
	uint8_t keyNotes[KEY_COUNT] = {61, 62, 64, 66};

	PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); // przyciski
	DDRB |= (1 << PB1); // dioda

	wdt_enable(WDTO_1S);

	USB_Init();
	sei();

	/* main event loop */
	while (1)
	{
		wdt_reset();
		usbPoll();

		for (i = 0; i < KEY_COUNT; i++)
		{
			uint8_t keyState = keyPressed(&(keyLocks[i]), &PINC, i);

			if (PRESSED == keyState)
			{
				event.command = MIDI_CMD_NOTE_ON;
				event.val1 = keyNotes[i];
				sent = FALSE;
			}
			else if (DEPRESSED == keyState)
			{
				event.command = MIDI_CMD_NOTE_OFF;
				event.val1 = keyNotes[i];
				sent = FALSE;
			}
		}

		if (usbInterruptIsReady())
		{
			if (!sent)
			{
				sent = TRUE;
				MIDI_SendEvent(&event);
			}
		}
	}
	return 0;
}

/*---------------------------------------------------------------------------*/
/* usbFunctionWriteOut                                                       */
/*                                                                           */
/* this Function is called if a MIDI Out message (from PC) arrives.          */
/*                                                                           */
/*---------------------------------------------------------------------------*/

void usbFunctionWriteOut(uchar * data, uchar len)
{
//	event.channel = data[0] & 0x0F;
//	event.command = data[0] & 0xF0;
//	event.val1 = data[1];
//	event.val2 = data[2];

//	event.channel = 0;
//	event.command = MIDI_CMD_NOTE_ON;
//	event.val1 = 61;
//
//	sent = FALSE;
//	PORTB |= (1 << PB1);
}

uint8_t keyPressed(uint8_t *lock, volatile uint8_t *PINX, uint8_t pinNumber)
{
	register uint8_t isPressed = (*PINX & (1 << pinNumber));

	if (!isPressed && !*lock)
	{
		*lock = TRUE;

		return PRESSED;
	}
	else if (isPressed && *lock)
	{
		if (!++*lock)
		{
			return DEPRESSED;
		}
	}

	return NONE;
}
