/*
 * midi.h
 *
 *  Created on: 28 maj 2019
 *      Author: Grzegorz
 */

#ifndef MIDI_H_
#define MIDI_H_


#include "avr/io.h"


#define  MIDI_CMD_NOTE_OFF                0x80
#define  MIDI_CMD_NOTE_ON                 0x90
#define  MIDI_CMD_AFTERTOUCH              0xA0
#define  MIDI_CMD_CONTINUOUS_CONTROLLER   0xB0
#define  MIDI_CMD_PATCH_CHANGE            0xC0
#define  MIDI_CMD_CHANNEL_PRESSURE        0xD0
#define  MIDI_CMD_PITCH_BEND              0xE0


typedef struct
{
	uint8_t command;
	uint8_t channel;
	uint8_t val1;
	uint8_t val2;
} MIDI_EVENT;


void MIDI_Send(uint8_t command, uint8_t channel, uint8_t val1, uint8_t val2);
void MIDI_SendEvent(MIDI_EVENT * event);

#endif /* MIDI_H_ */
