/*
 * midi.c
 *
 *  Created on: 28 maj 2019
 *      Author: Grzegorz
 */

#include "midi.h"

#include "avr/io.h"
#include "usbdrv/usbdrv.h"


void MIDI_Send(uint8_t command, uint8_t channel, uint8_t val1, uint8_t val2)
{
	static MIDI_EVENT event;

	event.command = command;
	event.channel = channel;
	event.val1 = val1;
	event.val2 = val2;

	MIDI_SendEvent(&event);
}

void MIDI_SendEvent(MIDI_EVENT * event)
{
	static uint8_t message[4];

	message[0] = (event->command) >> 4;
	message[1] = (event->command) | (event->channel);
	message[2] = event->val1;
	message[3] = event->val2;

	usbSetInterrupt(message, 4);
}
