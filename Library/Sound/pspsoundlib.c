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
// Psp Sound library ( FileName: pspsoundlib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2013 M4MSOFT
// ------------------------------------------------------------------------

// Include C header files.
#include <stdlib.h>
#include <string.h>

// Include psp Header files.
#include <pspthreadman.h>
#include <pspaudio.h>

// Include the PSP soundlib Header files.
#include "pspsoundlib.h"

// -----------------------------------------------------------------------------------------------------
// Defines
#define THREAD_STACK_SIZE      (256*1024)
#define PSP_MAx_SOUND_CHANNELS (1)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Private variables.
static volatile int sound_terminate = 1;
static int sound_ready = 0;
static int sound_channels = 0;

static short sound_sndbuf[PSP_NUM_SOUND_CHANNELS][2][PSP_NUM_SOUND_SAMPLES][2];
static psp_sound_channelinfo SoundStatus[PSP_NUM_SOUND_CHANNELS];

//static SceUID sound_sema[];
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static int _soundSetFrequency(unsigned short samples, unsigned short freq, char car);
static int _soundRelease(void);
static int _soundOutputBlocking(int volume, void *buffer);
static void _soundMixBuffer(short *dest, short *src);
static int _soundChannelThread(int args, void *argp);
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
static int _soundSetFrequency(unsigned short samples, unsigned short freq, char car)
{
	return sceAudioSRCChReserve(samples, freq, car);
}
// -----------------------------------------------------------------------------------------------------
static int _soundRelease(void)
{
	while(sceAudioOutput2GetRestSample() > 0);
	return sceAudioSRCChRelease();
}
// -----------------------------------------------------------------------------------------------------
static int _soundOutputBlocking(int volume, void *buffer)
{
	return sceAudioSRCOutputBlocking(volume, buffer);
}
// -----------------------------------------------------------------------------------------------------
// (00000001 = 1, 0111111 = 127, 10000000 = -128, 10000001 = -127, 11111111 = -1)
static void _soundMixBuffer(short *dest, short *src)
{
	int i;
	for (i = 0; i < 2*PSP_NUM_SOUND_SAMPLES; i++)
		*(dest+i) = (((int)(dest[i]))+((int)(src[i]))) & 0xffff;
}
// -----------------------------------------------------------------------------------------------------
static int _soundChannelThread(int args, void *argp)
{
	//volatile int bufidx = 0;
	int bufidx = 0;
    int channel = *(int*)argp;

	SoundStatus[channel].threadactive = 1;
    while (sound_terminate == 0)
	{
        void *bufptr = &sound_sndbuf[channel][bufidx];
        pspSoundCallback_t callback;
        callback = SoundStatus[channel].callback;
        if (callback)
            callback(bufptr, PSP_NUM_SOUND_SAMPLES, SoundStatus[channel].pdata);
		else
			memset(bufptr, 0, 4*PSP_NUM_SOUND_SAMPLES);

		if (channel)
		{
			// Mix the sound buffer with the lower sound thread.
			sceKernelWaitSema(SoundStatus[channel-1].sema, 1, 0); // Wait for the signal from the lower sound thread.
			if (callback) _soundMixBuffer(SoundStatus[0].bufptr, bufptr);
			SoundStatus[channel].bufptr = bufptr;
			sceKernelSignalSema(SoundStatus[channel].sema, 1); // Signal the next sound thread.
		}
		else
		{
			// Play the sound buffer.
			SoundStatus[channel].bufptr = bufptr;
			sceKernelSignalSema(SoundStatus[channel].sema, 1); // Signal the next sound thread.
			sceKernelWaitSema(SoundStatus[sound_channels-1].sema, 1, 0); // Wait for the signal from the last sound thread.
			_soundOutputBlocking(SoundStatus[channel].volumeright, bufptr);
		}

        bufidx = (bufidx ? 0 : 1);
    }
    SoundStatus[channel].threadactive = 0;
    sceKernelExitThread(0);
    return 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Initializes the psp sound library.
int pspSoundInit(unsigned int channels)
{
    int i,ret;
    int failed = 0;
    char str[32];

    sound_terminate = 0;
    sound_ready = 0;
	sound_channels = channels > PSP_NUM_SOUND_CHANNELS ? PSP_NUM_SOUND_CHANNELS: channels;
    memset(sound_sndbuf, 0, sizeof(sound_sndbuf));

    //sound_sema = sceKernelCreateSema("sound_sema", 6, sound_channels, PSP_NUM_SOUND_CHANNELS, 0);
	for (i = 0; i < sound_channels; i++)
	{
		SoundStatus[i].handle = -1;
        SoundStatus[i].threadhandle = -1;
        SoundStatus[i].threadactive = 0;
        SoundStatus[i].volumeright = PSP_VOLUME_MAX;
        SoundStatus[i].volumeleft  = PSP_VOLUME_MAX;
        SoundStatus[i].callback = 0;
        SoundStatus[i].pdata = 0;
		SoundStatus[i].bufptr = &sound_sndbuf[i][1];
		SoundStatus[i].sema = -1;
    }

	strcpy(str,"sound_sema0");
	for (i = 0; i < sound_channels; i++)
	{
		// Create the sound sema
		str[10] = '0'+i;
		SoundStatus[i].sema = sceKernelCreateSema(str, 6, 0, 1, 0);
		if (SoundStatus[i].sema < 0)
		{
			SoundStatus[i].sema = -1;
            failed = 1;
            break;
		}
	}
	if (failed)
	{
		sound_terminate = 1;
		for (i = 0; i < sound_channels; i++)
		{
			if (SoundStatus[i].sema != -1)
				sceKernelDeleteSema(SoundStatus[i].sema);
			SoundStatus[i].sema = -1;
		}
		sound_channels = 0;
        return -1;
	}

	for (i = 0; i < PSP_MAx_SOUND_CHANNELS; i++)
	{
		// Reserve the aodio input.
		_soundRelease();
		if(_soundSetFrequency(PSP_NUM_SOUND_SAMPLES, 44100, 2)<0)
			failed = 1;
		else
			SoundStatus[i].handle = 0;
    }
    if (failed)
	{
		sound_terminate = 1;
		for (i = 0; i < sound_channels; i++)
		{
			if (SoundStatus[i].sema != -1)
				sceKernelDeleteSema(SoundStatus[i].sema);
			SoundStatus[i].sema = -1;
		}
		for (i = 0; i < PSP_MAx_SOUND_CHANNELS; i++)
		{
			if (SoundStatus[i].handle != -1)
			{
			    _soundRelease();
			    SoundStatus[i].handle = -1;
			}
		}
		sound_channels = 0;
        return -1;
    }

	strcpy(str,"soundt0");
    for (i = 0; i < sound_channels; i++)
	{
		// Create the sound thread.
		str[6] = '0'+i;
        SoundStatus[i].threadhandle = sceKernelCreateThread(str, (void*) &_soundChannelThread, 0x12, THREAD_STACK_SIZE, PSP_THREAD_ATTR_USER, NULL);
        if (SoundStatus[i].threadhandle < 0)
		{
            SoundStatus[i].threadhandle = -1;
            failed = 1;
            break;
        }
		// Activate the create sound thread.
		SoundStatus[i].handle = 1;
        ret = sceKernelStartThread(SoundStatus[i].threadhandle, sizeof(i), &i);
        if (ret != 0)
		{
            failed = 1;
            break;
        }
    }
    if (failed)
	{
        sound_terminate = 1;
        for (i = 0; i < sound_channels; i++)
		{
            if (SoundStatus[i].threadhandle != -1)
			{
                if (i) sceKernelSignalSema(SoundStatus[i-1].sema, 1);
				sceKernelWaitThreadEnd(SoundStatus[i].threadhandle, NULL);
                while (SoundStatus[i].threadactive)
                    sceKernelDelayThread(100000);
                sceKernelDeleteThread(SoundStatus[i].threadhandle);
            }
            SoundStatus[i].threadhandle = -1;
        }
		for (i = 0; i < sound_channels; i++)
		{
			if (SoundStatus[i].sema != -1)
				sceKernelDeleteSema(SoundStatus[i].sema);
			SoundStatus[i].sema = -1;
		}
		for (i = 0; i < PSP_MAx_SOUND_CHANNELS; i++)
		{
			if (SoundStatus[i].handle != -1)
			{
			    _soundRelease();
			    SoundStatus[i].handle = -1;
			}
		}
		sound_channels = 0;
        return -1;
    }
	sound_ready = 1;
    return 0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void pspSoundEndPre()
{
	sound_ready = 0;
	sound_terminate = 1;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void pspSoundEnd()
{
    int i;
	sound_ready = 0;
	sound_terminate = 1;

    for (i = 0; i < sound_channels; i++)
	{
        if (SoundStatus[i].threadhandle != -1)
		{
			if (i) sceKernelSignalSema(SoundStatus[i-1].sema, 1);
			sceKernelWaitThreadEnd(SoundStatus[i].threadhandle, NULL);
            while (SoundStatus[i].threadactive)
                sceKernelDelayThread(100000);
            sceKernelDeleteThread(SoundStatus[i].threadhandle);
        }
        SoundStatus[i].threadhandle = -1;
    }
	for (i = 0; i < sound_channels; i++)
	{
		if (SoundStatus[i].sema != -1)
			sceKernelDeleteSema(SoundStatus[i].sema);
		SoundStatus[i].sema = -1;
	}
	for (i = 0; i < PSP_MAx_SOUND_CHANNELS; i++)
	{
		if (SoundStatus[i].handle != -1)
		{
			_soundRelease();
			SoundStatus[i].handle = -1;
		}
    }
	//sceKernelDeleteSema(sound_sema);
	sound_channels = 0;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
void pspSoundSetVolume(unsigned int channel, int left, int right)
{
    if(channel >= sound_channels) return;
    SoundStatus[channel].volumeright = right;
    SoundStatus[channel].volumeleft  = left;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void pspSoundChannelThreadCallback(unsigned int channel, void *buf, unsigned int reqn)
{
    if(channel >= sound_channels)
        return;
    pspSoundCallback_t callback;
    callback = SoundStatus[channel].callback;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
void pspSoundSetChannelCallback(unsigned int channel, pspSoundCallback_t callback, void *pdata)
{
    if(channel >= sound_channels)
        return;
    //volatile psp_sound_channelinfo *pci = &SoundStatus[channel];
	psp_sound_channelinfo *pci = &SoundStatus[channel];
    //while (pci->threadactive)
    //    sceKernelDelayThread(100000);
    pci->callback=0;
    pci->pdata=pdata;
    pci->callback=callback;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspSoundOutBlocking(unsigned int channel, unsigned int vol1, unsigned int vol2, void *buf)
{
    if (!sound_ready) return -1;
    if (channel >= sound_channels) return -1;
    if (vol1 > PSP_VOLUME_MAX) vol1 = PSP_VOLUME_MAX;
    if (vol2 > PSP_VOLUME_MAX) vol2 = PSP_VOLUME_MAX;
	int ret = 0;
	sceKernelWaitSema(SoundStatus[sound_channels-1].sema, 1, 0);
    ret = _soundOutputBlocking(vol1, buf);
	sceKernelSignalSema(SoundStatus[sound_channels-1].sema, 1);
	return ret;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
int pspSoundSetFrequency(unsigned short freq)
{
	if (!sound_ready)
		return -1;
	switch(freq)
	{
		case 8000:
		case 12000:
		case 16000:
		case 24000:
		case 32000:
		case 48000:
		case 11025:
		case 22050:
		case 44100:
			break;
		default:
			return -1;
	}
	int ret = 0;
	sceKernelWaitSema(SoundStatus[sound_channels-1].sema, 1, 0);
	_soundRelease();
	if(_soundSetFrequency(PSP_NUM_SOUND_SAMPLES, freq, 2) < 0) ret = -1;
	sceKernelSignalSema(SoundStatus[sound_channels-1].sema, 1);
	return ret;
}
// -----------------------------------------------------------------------------------------------------
