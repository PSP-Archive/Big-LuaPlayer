/*
 * PSP Software Development Kit - http://www.pspdev.org
 * -----------------------------------------------------------------------
 * Licensed under the BSD license, see LICENSE in PSPSDK root for details.
 *
 * pspaudiolib.h - Audio library build on top of sceAudio, but to provide
 *                 multiple thread usage and callbacks.
 *
 * Copyright (c) 2005 Adresd
 * Copyright (c) 2005 Marcus R. Brown <mrbrown@ocgnet.org>
 *
 * $Id: pspaudiolib.h 1874 2006-04-18 13:20:58Z tyranid $
 */

// ------------------------------------------------------------------------
// Psp Sound library ( FileName: pspsoundlib.h )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2013 M4MSOFT
// ------------------------------------------------------------------------

#ifndef __PSPSOUNDLIB_H__
#define __PSPSOUNDLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

// -----------------------------------------------------------------------------------------------------
// Defines
#define PSP_NUM_SOUND_CHANNELS (4)
#define PSP_NUM_SOUND_SAMPLES  (1024)
#define PSP_VOLUME_MAX         (0x8000)
#define PSP_SAMPLE_RATE        (44100)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Structure and pointers.
typedef void (*pspSoundCallback_t)(void *buf, unsigned int reqn, void *pdata);

typedef struct {
  int threadhandle;
  int handle;
  int volumeleft;
  int volumeright;
  pspSoundCallback_t callback;
  void *pdata;
  int threadactive;
  void* bufptr;
  int sema;
} psp_sound_channelinfo;

typedef int (*pspSoundThreadfunc_t)(int args, void *argp);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Initializes the psp sound library.
//
// @param: channels - the number of channels to use.
//
// @return: int - 0 on succes.
//
int pspSoundInit(unsigned int channels);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Prepare a shutdown.
//
// @param: void.
//
// @return: void.
//
void pspSoundEndPre();
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the psp sound library.
//
// @param: void.
//
// @return: void.
//
void pspSoundEnd();
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Set the volume of a channel.
//
// @param: channel - the sound channel.
// @param: left - the left speaker volume.
// @param: right - the right speaker volume.
//
// @return: void.
//
void pspSoundSetVolume(unsigned int channel, int left, int right);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Callback the sound channel.
//
// @param: channel - the sound channel.
// @param: buf - the sound buffer.
// @param: reqn - the sound buffer size.
//
// @return: void.
//
void pspSoundChannelThreadCallback(unsigned int channel, void *buf, unsigned int reqn);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Callback the sound channel.
//
// @param: channel - the sound channel.
// @param: buf - the sound buffer.
// @param: reqn - the sound buffer size.
//
// @return: void.
//
void pspSoundSetChannelCallback(unsigned int channel, pspSoundCallback_t callback, void *pdata);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Out Blocking the sound channel.
//
// @param: channel - the sound channel.
// @param: vol1 - the minimum sound volume.
// @param: vol2 - the maximum sound volume.
// @param: buf - the sound buffer.
//
// @return: void.
//
int pspSoundOutBlocking(unsigned int channel, unsigned int vol1, unsigned int vol2, void *buf);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Resere a sound channel.
//
// @param: freq - the frequency of the channel.
//
// @return: void.
//
int pspSoundSetFrequency(unsigned short freq);
// -----------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif
