// Psp Sound library ( FileName: soundlib.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __soundlib_h
#define __soundlib_h

#ifdef __cplusplus
extern "C" {
#endif

// Include C header files.
#include <stdbool.h>

// Include psp sound library Header file.
#include "pspsoundlib.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define SOUNDLIB_TYPE_NONE (0) // no sound stream.
#define SOUNDLIB_TYPE_RAW  (1) // raw sound stream.
#define SOUNDLIB_TYPE_POL  (2) // polyphonic sound stream.
#define SOUNDLIB_TYPE_WAV  (3) // wav sound stream.

#define SOUNDLIB_NOTE_PAUSE (0)
#define SOUNDLIB_NOTE_C     (1)
#define SOUNDLIB_NOTE_CS    (2)
#define SOUNDLIB_NOTE_D     (3)
#define SOUNDLIB_NOTE_DS    (4)
#define SOUNDLIB_NOTE_E     (5)
#define SOUNDLIB_NOTE_F     (6)
#define SOUNDLIB_NOTE_FS    (7)
#define SOUNDLIB_NOTE_G     (8)
#define SOUNDLIB_NOTE_GS    (9)
#define SOUNDLIB_NOTE_A     (10)
#define SOUNDLIB_NOTE_AS    (11)
#define SOUNDLIB_NOTE_B     (12)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structure, unions and pointers.
typedef struct {
	short l, r;
} soundBuf_;
typedef soundBuf_* soundBuf;

typedef struct
{
	int type;     // The sound stream type.
	void *pdata;  // Pointer to the sound stream object.
	int memCount; // The total amount of memory links.
} sound_object_;
typedef sound_object_* sound_object;

typedef struct
{
	unsigned int channel; // The channel of the sound stream.
	bool playing;          // Play the sound stream.
	bool paused;           // Pause the sound stream.
	bool loop;             // Loop the sound stream.
	bool stop;             // Stop the sound stream.
	sound_object object;   // Pointer to the sound object.
} sound_userdata_;
typedef sound_userdata_* sound_userdata;


typedef struct
{
	soundBuf data;
	int dataLength;
	int dataPos;
	float amp;
} sound_raw_;
typedef sound_raw_* sound_raw;


typedef struct {
	unsigned char note, octave;
	short volume;
	unsigned int duration;
} sound_note_;
typedef sound_note_* sound_note;

typedef struct
{
	sound_note data;
	int dataLength;
	int dataPos;
	unsigned int time;
	float increment;
	float index;
	float amp;
} sound_pol_;
typedef sound_pol_* sound_pol;


typedef struct
{
	char *data;
	int dataLength;
	int dataPos;
	short sigBytes;
	short numChannels;
	int sampleRate;
	int bytesPerSecond;
	float amp;
} sound_wav_;
typedef sound_wav_* sound_wav;

// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the psp sound library.
//
// @param: channels - the number of channels to initializes.
//
// @return: void.
//
void SoundInit(unsigned int channels);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the sound library.
//
// @param: void.
//
// @return: void.
//
void SoundTerm();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// create a new sound object.
//
// @param: type - The sound object type to create.
// @param: len - The size of the sound buffer.
//
// @return: sound_object - This function returns NULL on allocation fail..
//
sound_object SoundCreateObject(int type, int len);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a sond object & set its pointer to NULL.
//
// @param: psobj - Pointer to the variable which contains the sound_object_ pointer.
//
// @return: void.
//
void SoundFreeObject(sound_object *psobj);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save a sound object to a file. (This function dosn't work for wav objects)
//
// @param: sobj - The sound object to save.
// @param: path - The file to path.
//
// @return: void.
//
void SoundSaveObject(sound_object sobj, const char *path);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a sound object from a file.
//
// @param: path - the file to load.
//
// @return: sound_object - This function returns NULL on allocation fail.
//
sound_object SoundLoadObject(const char *path);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get the buffer from a sound object. (This function dosn't work for wav objects)
//
// @param: sobj - The sound object.
// @param: pos - The buffer posistion.
//
// @return: void* - The sound buffer object or NULL.
//
void* SoundGetBufferObject(sound_object sobj, unsigned int pos);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the buffer from a sound object. (This function dosn't work for wav objects)
//
// @param: sobj - The sound object.
// @param: pos - The buffer posistion.
// @param: buf - The new buffer data.
//
// @return: void.
//
void SoundSetBufferObject(sound_object sobj, unsigned int pos, void* buf);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Start playing a sound object.
//
// @param: sobj - The sound object.
// @param: rewind - Rewind the sound object.
// @param: loop - Enable or disable the sound loop.
//
// @return: void.
//
void SoundPlayObject(sound_object sobj, bool rewind, bool loop);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the loop value of a Playing a sound object.
//
// @param: sobj - The sound object.
// @param: loop - Enable or disable the sound loop.
//
// @return: void.
//
void SoundLoopObject(sound_object sobj, bool loop);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop Playing a sound object.
//
// @param: sobj - The sound object.
//
// @return: void.
//
void SoundStopObject(sound_object sobj);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Pause a playing sound object.
//
// @param: sobj - The sound object.
//
// @return: void.
//
void SoundPauseObject(sound_object sobj);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// the the time of a sound object.
//
// @param: sobj - The sound object.
// @param: time - The new sound play time.
//
// @return: void.
//
void SoundSeekObject(sound_object sobj, int time);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Rewind a sound object.
//
// @param: sobj - The sound object.
//
// @return: void.
//
void SoundRewindObject(sound_object sobj);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the amplification value of a sound object.
//
// @param: sobj - The sound object.
// @param: amp - The amplification value.
//
// @return: void.
//
void SoundAmplification(sound_object sobj, float amp);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop Playing all sound objects.
//
// @param: void.
//
// @return: void.
//
void SoundStopAllObject();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Wav management.
// Base code from: 'pspaalib' - Created by Arshia001
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a wav from a file.
//
// @param: path - the wav file to load.
//
// @return: sound_object - This function returns NULL on allocation fail.
//
sound_object SoundLoadWav(const char *path);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Raw management.
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a raw from a file.
//
// @param: path - the raw file to load.
//
// @return: sound_object - This function returns NULL on allocation fail.
//
sound_object SoundLoadRaw(const char *path);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// polyphonic management.
// Base code from: 'Basic audio sample' - Copyright (c) 2005 Frank Buss <fb@frank-buss.de> (aka Shine)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a polyphon from a file.
//
// @param: path - the polyphon file to load.
//
// @return: sound_object - This function returns NULL on allocation fail.
//
sound_object SoundLoadPol(const char *path);
// -----------------------------------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif

#endif
