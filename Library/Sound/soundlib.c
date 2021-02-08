// Psp Sound library ( FileName: soundlib.c )
// ------------------------------------------------------------------------
// Version: 1.00
// Copyright (c) 2012 M4MSOFT
// ------------------------------------------------------------------------

// Include psp Header files.
#include <pspkernel.h>

// Include C header files.
#include <malloc.h>
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <math.h>
//#include <stdbool.h>

// Include the PSP soundlib Header files.
#include "soundlib.h"

// -----------------------------------------------------------------------------------------------------
// Defines.
#define SOUNDLIB_OCTAVE_COUNT (6)
#define SOUNDLIB_SAMPEL_COUNT (1024)

#define NEWRAWBUFFER(obj, size) if((size)>0){((sound_raw)(obj))->data=(soundBuf)malloc((size)*sizeof(soundBuf_));if(((sound_raw)(obj))->data){((sound_raw)(obj))->dataLength=(size);memset(((sound_raw)(obj))->data,0,(size)*sizeof(soundBuf_));}}
#define NEWPOLBUFFER(obj, size) if((size)>0){((sound_pol)(obj))->data=(sound_note)malloc((size)*sizeof(sound_note_));if(((sound_pol)(obj))->data){((sound_pol)(obj))->dataLength=(size);memset(((sound_pol)(obj))->data,0,(size)*sizeof(sound_note_));}}
#define NEWWAVBUFFER(obj, size) if((size)>0){((sound_wav)(obj))->data=(char*)malloc((size)*sizeof(char));if(((sound_wav)(obj))->data){((sound_wav)(obj))->dataLength=(size);memset(((sound_wav)(obj))->data,0,(size)*sizeof(char));}}

#define pol_frerequency (sound_samples[(int)(pol->index)])
#define pol_data        ((pol->data+pol->dataPos))
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Private variables.
static bool init = false;

static unsigned int channel_count = 0;
static sound_userdata_ channel_userdata[PSP_NUM_SOUND_CHANNELS];

static const float sound_attack = 0.004;
static const float sound_decay = 0.02;
static const float sound_sustain = 0.5;
static const float sound_release = 0.08;
static const float sound_octaves[SOUNDLIB_OCTAVE_COUNT][12] =
{
	{40.00000000f, 42.37852377f, 44.89848193f, 47.56828460f, 50.39684200f, 53.39359417f, 56.56854250f, 59.93228308f, 63.49604208f, 67.27171322f, 71.27189745f, 75.50994501f},
	{80.00000000f, 84.75704755f, 89.79696387f, 95.13656920f, 100.7936840f, 106.7871883f, 113.1370850f, 119.8645662f, 126.9920842f, 134.5434264f, 142.5437949f, 151.0198900f},
	{160.0000000f, 169.5140951f, 179.5939277f, 190.2731384f, 201.5873680f, 213.5743767f, 226.2741700f, 239.7291323f, 253.9841683f, 269.0868529f, 285.0875898f, 302.0397801f},
	{320.0000000f, 339.0281902f, 359.1878555f, 380.5462768f, 403.1747360f, 427.1487533f, 452.5483400f, 479.4582646f, 507.9683366f, 538.1737058f, 570.1751796f, 604.0795601f},
	{640.0000000f, 678.0563804f, 718.3757109f, 761.0925536f, 806.3494720f, 854.2975067f, 905.0966800f, 958.9165292f, 1015.936673f, 1076.347412f, 1140.350359f, 1208.159120f},
	{1280.000000f, 1356.112761f, 1436.751422f, 1522.185107f, 1612.698944f, 1708.595013f, 1810.193360f, 1917.833058f, 2031.873347f, 2152.694823f, 2280.700719f, 2416.318241f}
};
static const float sound_samples[SOUNDLIB_SAMPEL_COUNT] =
{
     1.000000000f,  0.998046875f,  0.996093750f,  0.994140625f,  0.992187500f,  0.990234375f,  0.988281250f,  0.986328125f,  0.984375000f,  0.982421875f,  0.980468750f,  0.978515625f,  0.976562500f,  0.974609375f,  0.972656250f,  0.970703125f,
     0.968750000f,  0.966796875f,  0.964843750f,  0.962890625f,  0.960937500f,  0.958984375f,  0.957031250f,  0.955078125f,  0.953125000f,  0.951171875f,  0.949218750f,  0.947265625f,  0.945312500f,  0.943359375f,  0.941406250f,  0.939453125f,
     0.937500000f,  0.935546875f,  0.933593750f,  0.931640625f,  0.929687500f,  0.927734375f,  0.925781250f,  0.923828125f,  0.921875000f,  0.919921875f,  0.917968750f,  0.916015625f,  0.914062500f,  0.912109375f,  0.910156250f,  0.908203125f,
     0.906250000f,  0.904296875f,  0.902343750f,  0.900390625f,  0.898437500f,  0.896484375f,  0.894531250f,  0.892578125f,  0.890625000f,  0.888671875f,  0.886718750f,  0.884765625f,  0.882812500f,  0.880859375f,  0.878906250f,  0.876953125f,
     0.875000000f,  0.873046875f,  0.871093750f,  0.869140625f,  0.867187500f,  0.865234375f,  0.863281250f,  0.861328125f,  0.859375000f,  0.857421875f,  0.855468750f,  0.853515625f,  0.851562500f,  0.849609375f,  0.847656250f,  0.845703125f,
     0.843750000f,  0.841796875f,  0.839843750f,  0.837890625f,  0.835937500f,  0.833984375f,  0.832031250f,  0.830078125f,  0.828125000f,  0.826171875f,  0.824218750f,  0.822265625f,  0.820312500f,  0.818359375f,  0.816406250f,  0.814453125f,
     0.812500000f,  0.810546875f,  0.808593750f,  0.806640625f,  0.804687500f,  0.802734375f,  0.800781250f,  0.798828125f,  0.796875000f,  0.794921875f,  0.792968750f,  0.791015625f,  0.789062500f,  0.787109375f,  0.785156250f,  0.783203125f,
     0.781250000f,  0.779296875f,  0.777343750f,  0.775390625f,  0.773437500f,  0.771484375f,  0.769531250f,  0.767578125f,  0.765625000f,  0.763671875f,  0.761718750f,  0.759765625f,  0.757812500f,  0.755859375f,  0.753906250f,  0.751953125f,
     0.750000000f,  0.748046875f,  0.746093750f,  0.744140625f,  0.742187500f,  0.740234375f,  0.738281250f,  0.736328125f,  0.734375000f,  0.732421875f,  0.730468750f,  0.728515625f,  0.726562500f,  0.724609375f,  0.722656250f,  0.720703125f,
     0.718750000f,  0.716796875f,  0.714843750f,  0.712890625f,  0.710937500f,  0.708984375f,  0.707031250f,  0.705078125f,  0.703125000f,  0.701171875f,  0.699218750f,  0.697265625f,  0.695312500f,  0.693359375f,  0.691406250f,  0.689453125f,
     0.687500000f,  0.685546875f,  0.683593750f,  0.681640625f,  0.679687500f,  0.677734375f,  0.675781250f,  0.673828125f,  0.671875000f,  0.669921875f,  0.667968750f,  0.666015625f,  0.664062500f,  0.662109375f,  0.660156250f,  0.658203125f,
     0.656250000f,  0.654296875f,  0.652343750f,  0.650390625f,  0.648437500f,  0.646484375f,  0.644531250f,  0.642578125f,  0.640625000f,  0.638671875f,  0.636718750f,  0.634765625f,  0.632812500f,  0.630859375f,  0.628906250f,  0.626953125f,
     0.625000000f,  0.623046875f,  0.621093750f,  0.619140625f,  0.617187500f,  0.615234375f,  0.613281250f,  0.611328125f,  0.609375000f,  0.607421875f,  0.605468750f,  0.603515625f,  0.601562500f,  0.599609375f,  0.597656250f,  0.595703125f,
     0.593750000f,  0.591796875f,  0.589843750f,  0.587890625f,  0.585937500f,  0.583984375f,  0.582031250f,  0.580078125f,  0.578125000f,  0.576171875f,  0.574218750f,  0.572265625f,  0.570312500f,  0.568359375f,  0.566406250f,  0.564453125f,
     0.562500000f,  0.560546875f,  0.558593750f,  0.556640625f,  0.554687500f,  0.552734375f,  0.550781250f,  0.548828125f,  0.546875000f,  0.544921875f,  0.542968750f,  0.541015625f,  0.539062500f,  0.537109375f,  0.535156250f,  0.533203125f,
     0.531250000f,  0.529296875f,  0.527343750f,  0.525390625f,  0.523437500f,  0.521484375f,  0.519531250f,  0.517578125f,  0.515625000f,  0.513671875f,  0.511718750f,  0.509765625f,  0.507812500f,  0.505859375f,  0.503906250f,  0.501953125f,
     0.500000000f,  0.498046875f,  0.496093750f,  0.494140625f,  0.492187500f,  0.490234375f,  0.488281250f,  0.486328125f,  0.484375000f,  0.482421875f,  0.480468750f,  0.478515625f,  0.476562500f,  0.474609375f,  0.472656250f,  0.470703125f,
     0.468750000f,  0.466796875f,  0.464843750f,  0.462890625f,  0.460937500f,  0.458984375f,  0.457031250f,  0.455078125f,  0.453125000f,  0.451171875f,  0.449218750f,  0.447265625f,  0.445312500f,  0.443359375f,  0.441406250f,  0.439453125f,
     0.437500000f,  0.435546875f,  0.433593750f,  0.431640625f,  0.429687500f,  0.427734375f,  0.425781250f,  0.423828125f,  0.421875000f,  0.419921875f,  0.417968750f,  0.416015625f,  0.414062500f,  0.412109375f,  0.410156250f,  0.408203125f,
     0.406250000f,  0.404296875f,  0.402343750f,  0.400390625f,  0.398437500f,  0.396484375f,  0.394531250f,  0.392578125f,  0.390625000f,  0.388671875f,  0.386718750f,  0.384765625f,  0.382812500f,  0.380859375f,  0.378906250f,  0.376953125f,
     0.375000000f,  0.373046875f,  0.371093750f,  0.369140625f,  0.367187500f,  0.365234375f,  0.363281250f,  0.361328125f,  0.359375000f,  0.357421875f,  0.355468750f,  0.353515625f,  0.351562500f,  0.349609375f,  0.347656250f,  0.345703125f,
     0.343750000f,  0.341796875f,  0.339843750f,  0.337890625f,  0.335937500f,  0.333984375f,  0.332031250f,  0.330078125f,  0.328125000f,  0.326171875f,  0.324218750f,  0.322265625f,  0.320312500f,  0.318359375f,  0.316406250f,  0.314453125f,
     0.312500000f,  0.310546875f,  0.308593750f,  0.306640625f,  0.304687500f,  0.302734375f,  0.300781250f,  0.298828125f,  0.296875000f,  0.294921875f,  0.292968750f,  0.291015625f,  0.289062500f,  0.287109375f,  0.285156250f,  0.283203125f,
     0.281250000f,  0.279296875f,  0.277343750f,  0.275390625f,  0.273437500f,  0.271484375f,  0.269531250f,  0.267578125f,  0.265625000f,  0.263671875f,  0.261718750f,  0.259765625f,  0.257812500f,  0.255859375f,  0.253906250f,  0.251953125f,
     0.250000000f,  0.248046875f,  0.246093750f,  0.244140625f,  0.242187500f,  0.240234375f,  0.238281250f,  0.236328125f,  0.234375000f,  0.232421875f,  0.230468750f,  0.228515625f,  0.226562500f,  0.224609375f,  0.222656250f,  0.220703125f,
     0.218750000f,  0.216796875f,  0.214843750f,  0.212890625f,  0.210937500f,  0.208984375f,  0.207031250f,  0.205078125f,  0.203125000f,  0.201171875f,  0.199218750f,  0.197265625f,  0.195312500f,  0.193359375f,  0.191406250f,  0.189453125f,
     0.187500000f,  0.185546875f,  0.183593750f,  0.181640625f,  0.179687500f,  0.177734375f,  0.175781250f,  0.173828125f,  0.171875000f,  0.169921875f,  0.167968750f,  0.166015625f,  0.164062500f,  0.162109375f,  0.160156250f,  0.158203125f,
     0.156250000f,  0.154296875f,  0.152343750f,  0.150390625f,  0.148437500f,  0.146484375f,  0.144531250f,  0.142578125f,  0.140625000f,  0.138671875f,  0.136718750f,  0.134765625f,  0.132812500f,  0.130859375f,  0.128906250f,  0.126953125f,
     0.125000000f,  0.123046875f,  0.121093750f,  0.119140625f,  0.117187500f,  0.115234375f,  0.113281250f,  0.111328125f,  0.109375000f,  0.107421875f,  0.105468750f,  0.103515625f,  0.101562500f,  0.099609375f,  0.097656250f,  0.095703125f,
     0.093750000f,  0.091796875f,  0.089843750f,  0.087890625f,  0.085937500f,  0.083984375f,  0.082031250f,  0.080078125f,  0.078125000f,  0.076171875f,  0.074218750f,  0.072265625f,  0.070312500f,  0.068359375f,  0.066406250f,  0.064453125f,
     0.062500000f,  0.060546875f,  0.058593750f,  0.056640625f,  0.054687500f,  0.052734375f,  0.050781250f,  0.048828125f,  0.046875000f,  0.044921875f,  0.042968750f,  0.041015625f,  0.039062500f,  0.037109375f,  0.035156250f,  0.033203125f,
     0.031250000f,  0.029296875f,  0.027343750f,  0.025390625f,  0.023437500f,  0.021484375f,  0.019531250f,  0.017578125f,  0.015625000f,  0.013671875f,  0.011718750f,  0.009765625f,  0.007812500f,  0.005859375f,  0.003906250f,  0.001953125f,
     0.000000000f, -0.001953125f, -0.003906250f, -0.005859375f, -0.007812500f, -0.009765625f, -0.011718750f, -0.013671875f, -0.015625000f, -0.017578125f, -0.019531250f, -0.021484375f, -0.023437500f, -0.025390625f, -0.027343750f, -0.029296875f,
    -0.031250000f, -0.033203125f, -0.035156250f, -0.037109375f, -0.039062500f, -0.041015625f, -0.042968750f, -0.044921875f, -0.046875000f, -0.048828125f, -0.050781250f, -0.052734375f, -0.054687500f, -0.056640625f, -0.058593750f, -0.060546875f,
    -0.062500000f, -0.064453125f, -0.066406250f, -0.068359375f, -0.070312500f, -0.072265625f, -0.074218750f, -0.076171875f, -0.078125000f, -0.080078125f, -0.082031250f, -0.083984375f, -0.085937500f, -0.087890625f, -0.089843750f, -0.091796875f,
    -0.093750000f, -0.095703125f, -0.097656250f, -0.099609375f, -0.101562500f, -0.103515625f, -0.105468750f, -0.107421875f, -0.109375000f, -0.111328125f, -0.113281250f, -0.115234375f, -0.117187500f, -0.119140625f, -0.121093750f, -0.123046875f,
    -0.125000000f, -0.126953125f, -0.128906250f, -0.130859375f, -0.132812500f, -0.134765625f, -0.136718750f, -0.138671875f, -0.140625000f, -0.142578125f, -0.144531250f, -0.146484375f, -0.148437500f, -0.150390625f, -0.152343750f, -0.154296875f,
    -0.156250000f, -0.158203125f, -0.160156250f, -0.162109375f, -0.164062500f, -0.166015625f, -0.167968750f, -0.169921875f, -0.171875000f, -0.173828125f, -0.175781250f, -0.177734375f, -0.179687500f, -0.181640625f, -0.183593750f, -0.185546875f,
    -0.187500000f, -0.189453125f, -0.191406250f, -0.193359375f, -0.195312500f, -0.197265625f, -0.199218750f, -0.201171875f, -0.203125000f, -0.205078125f, -0.207031250f, -0.208984375f, -0.210937500f, -0.212890625f, -0.214843750f, -0.216796875f,
    -0.218750000f, -0.220703125f, -0.222656250f, -0.224609375f, -0.226562500f, -0.228515625f, -0.230468750f, -0.232421875f, -0.234375000f, -0.236328125f, -0.238281250f, -0.240234375f, -0.242187500f, -0.244140625f, -0.246093750f, -0.248046875f,
    -0.250000000f, -0.251953125f, -0.253906250f, -0.255859375f, -0.257812500f, -0.259765625f, -0.261718750f, -0.263671875f, -0.265625000f, -0.267578125f, -0.269531250f, -0.271484375f, -0.273437500f, -0.275390625f, -0.277343750f, -0.279296875f,
    -0.281250000f, -0.283203125f, -0.285156250f, -0.287109375f, -0.289062500f, -0.291015625f, -0.292968750f, -0.294921875f, -0.296875000f, -0.298828125f, -0.300781250f, -0.302734375f, -0.304687500f, -0.306640625f, -0.308593750f, -0.310546875f,
    -0.312500000f, -0.314453125f, -0.316406250f, -0.318359375f, -0.320312500f, -0.322265625f, -0.324218750f, -0.326171875f, -0.328125000f, -0.330078125f, -0.332031250f, -0.333984375f, -0.335937500f, -0.337890625f, -0.339843750f, -0.341796875f,
    -0.343750000f, -0.345703125f, -0.347656250f, -0.349609375f, -0.351562500f, -0.353515625f, -0.355468750f, -0.357421875f, -0.359375000f, -0.361328125f, -0.363281250f, -0.365234375f, -0.367187500f, -0.369140625f, -0.371093750f, -0.373046875f,
    -0.375000000f, -0.376953125f, -0.378906250f, -0.380859375f, -0.382812500f, -0.384765625f, -0.386718750f, -0.388671875f, -0.390625000f, -0.392578125f, -0.394531250f, -0.396484375f, -0.398437500f, -0.400390625f, -0.402343750f, -0.404296875f,
    -0.406250000f, -0.408203125f, -0.410156250f, -0.412109375f, -0.414062500f, -0.416015625f, -0.417968750f, -0.419921875f, -0.421875000f, -0.423828125f, -0.425781250f, -0.427734375f, -0.429687500f, -0.431640625f, -0.433593750f, -0.435546875f,
    -0.437500000f, -0.439453125f, -0.441406250f, -0.443359375f, -0.445312500f, -0.447265625f, -0.449218750f, -0.451171875f, -0.453125000f, -0.455078125f, -0.457031250f, -0.458984375f, -0.460937500f, -0.462890625f, -0.464843750f, -0.466796875f,
    -0.468750000f, -0.470703125f, -0.472656250f, -0.474609375f, -0.476562500f, -0.478515625f, -0.480468750f, -0.482421875f, -0.484375000f, -0.486328125f, -0.488281250f, -0.490234375f, -0.492187500f, -0.494140625f, -0.496093750f, -0.498046875f,
    -0.500000000f, -0.501953125f, -0.503906250f, -0.505859375f, -0.507812500f, -0.509765625f, -0.511718750f, -0.513671875f, -0.515625000f, -0.517578125f, -0.519531250f, -0.521484375f, -0.523437500f, -0.525390625f, -0.527343750f, -0.529296875f,
    -0.531250000f, -0.533203125f, -0.535156250f, -0.537109375f, -0.539062500f, -0.541015625f, -0.542968750f, -0.544921875f, -0.546875000f, -0.548828125f, -0.550781250f, -0.552734375f, -0.554687500f, -0.556640625f, -0.558593750f, -0.560546875f,
    -0.562500000f, -0.564453125f, -0.566406250f, -0.568359375f, -0.570312500f, -0.572265625f, -0.574218750f, -0.576171875f, -0.578125000f, -0.580078125f, -0.582031250f, -0.583984375f, -0.585937500f, -0.587890625f, -0.589843750f, -0.591796875f,
    -0.593750000f, -0.595703125f, -0.597656250f, -0.599609375f, -0.601562500f, -0.603515625f, -0.605468750f, -0.607421875f, -0.609375000f, -0.611328125f, -0.613281250f, -0.615234375f, -0.617187500f, -0.619140625f, -0.621093750f, -0.623046875f,
    -0.625000000f, -0.626953125f, -0.628906250f, -0.630859375f, -0.632812500f, -0.634765625f, -0.636718750f, -0.638671875f, -0.640625000f, -0.642578125f, -0.644531250f, -0.646484375f, -0.648437500f, -0.650390625f, -0.652343750f, -0.654296875f,
    -0.656250000f, -0.658203125f, -0.660156250f, -0.662109375f, -0.664062500f, -0.666015625f, -0.667968750f, -0.669921875f, -0.671875000f, -0.673828125f, -0.675781250f, -0.677734375f, -0.679687500f, -0.681640625f, -0.683593750f, -0.685546875f,
    -0.687500000f, -0.689453125f, -0.691406250f, -0.693359375f, -0.695312500f, -0.697265625f, -0.699218750f, -0.701171875f, -0.703125000f, -0.705078125f, -0.707031250f, -0.708984375f, -0.710937500f, -0.712890625f, -0.714843750f, -0.716796875f,
    -0.718750000f, -0.720703125f, -0.722656250f, -0.724609375f, -0.726562500f, -0.728515625f, -0.730468750f, -0.732421875f, -0.734375000f, -0.736328125f, -0.738281250f, -0.740234375f, -0.742187500f, -0.744140625f, -0.746093750f, -0.748046875f,
    -0.750000000f, -0.751953125f, -0.753906250f, -0.755859375f, -0.757812500f, -0.759765625f, -0.761718750f, -0.763671875f, -0.765625000f, -0.767578125f, -0.769531250f, -0.771484375f, -0.773437500f, -0.775390625f, -0.777343750f, -0.779296875f,
    -0.781250000f, -0.783203125f, -0.785156250f, -0.787109375f, -0.789062500f, -0.791015625f, -0.792968750f, -0.794921875f, -0.796875000f, -0.798828125f, -0.800781250f, -0.802734375f, -0.804687500f, -0.806640625f, -0.808593750f, -0.810546875f,
    -0.812500000f, -0.814453125f, -0.816406250f, -0.818359375f, -0.820312500f, -0.822265625f, -0.824218750f, -0.826171875f, -0.828125000f, -0.830078125f, -0.832031250f, -0.833984375f, -0.835937500f, -0.837890625f, -0.839843750f, -0.841796875f,
    -0.843750000f, -0.845703125f, -0.847656250f, -0.849609375f, -0.851562500f, -0.853515625f, -0.855468750f, -0.857421875f, -0.859375000f, -0.861328125f, -0.863281250f, -0.865234375f, -0.867187500f, -0.869140625f, -0.871093750f, -0.873046875f,
    -0.875000000f, -0.876953125f, -0.878906250f, -0.880859375f, -0.882812500f, -0.884765625f, -0.886718750f, -0.888671875f, -0.890625000f, -0.892578125f, -0.894531250f, -0.896484375f, -0.898437500f, -0.900390625f, -0.902343750f, -0.904296875f,
    -0.906250000f, -0.908203125f, -0.910156250f, -0.912109375f, -0.914062500f, -0.916015625f, -0.917968750f, -0.919921875f, -0.921875000f, -0.923828125f, -0.925781250f, -0.927734375f, -0.929687500f, -0.931640625f, -0.933593750f, -0.935546875f,
    -0.937500000f, -0.939453125f, -0.941406250f, -0.943359375f, -0.945312500f, -0.947265625f, -0.949218750f, -0.951171875f, -0.953125000f, -0.955078125f, -0.957031250f, -0.958984375f, -0.960937500f, -0.962890625f, -0.964843750f, -0.966796875f,
    -0.968750000f, -0.970703125f, -0.972656250f, -0.974609375f, -0.976562500f, -0.978515625f, -0.980468750f, -0.982421875f, -0.984375000f, -0.986328125f, -0.988281250f, -0.990234375f, -0.992187500f, -0.994140625f, -0.996093750f, -0.998046875f,
};
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Define the function prototypes.
static void _soundCallback(soundBuf buf, unsigned int reqn, sound_userdata pdata);

static sound_object _loadWav(FILE *fp);
static void _seekWav(sound_wav wav, int time);
static void _ampWav(sound_wav wav, float amp);
static bool _bufferWav(soundBuf buf, unsigned int reqn, sound_wav wav, bool loop);
static void _freeWav(sound_wav wav);

static sound_object _loadRaw(FILE *fp);
static void _saveRaw(const char *path, sound_raw raw);
static void _seekRaw(sound_raw raw, int time);
static void _ampRaw(sound_raw raw, float amp);
static soundBuf _getBufferRaw(sound_raw raw, unsigned int pos);
static bool _bufferRaw(soundBuf buf, unsigned int reqn, sound_raw raw, bool loop);
static void _freeRaw(sound_raw raw);

static sound_object _loadPol(FILE *fp);
static void _savePol (const char *path, sound_pol pol);
static void _seekPol(sound_pol pol, int time);
static void _ampPol(sound_pol pol, float amp);
static sound_note _getBufferPol(sound_pol pol, unsigned int pos);
static float _adsrPol(float time, float duration);
static bool _bufferPol(soundBuf buf, unsigned int reqn, sound_pol pol, bool loop);
static void _freePol(sound_pol pol);
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// This function gets called by pspsoundlib every time the audio buffer needs to be filled.
static void _soundCallback(soundBuf buf, unsigned int reqn, sound_userdata pdata)
{
	if (!pdata->paused)
	{
		// Fill the sound buffer.
		switch (pdata->object->type)
		{
			case SOUNDLIB_TYPE_RAW: // raw sound stream.
				pdata->playing = _bufferRaw(buf, reqn, pdata->object->pdata, pdata->loop);
				break;
			case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
				pdata->playing = _bufferPol(buf, reqn, pdata->object->pdata, pdata->loop);
				break;
			case SOUNDLIB_TYPE_WAV: // wav sound stream.
				pdata->playing = _bufferWav(buf, reqn, pdata->object->pdata, pdata->loop);
				break;
			default:
				memset(buf, 0, sizeof(soundBuf_)*reqn);
				break;
		}
	}

	// Set the soundCallback to NULL.
	if (!pdata->playing || pdata->stop)
	{
		unsigned int channel = pdata->channel;
		SoundSeekObject(pdata->object, 0);
		SoundFreeObject(&(pdata->object));
		pspSoundSetChannelCallback(channel, NULL, NULL);
		pdata->channel = 0;
		pdata->playing = false;
		pdata->paused = false;
		pdata->loop = false;
		pdata->stop = false;
		channel_userdata[channel].object = NULL;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
//  Initializes the sound library.
void SoundInit(unsigned int channels)
{
	if (init)
		return;

	// Check the numbeer of channels.
	if (channels < 1 || channels > PSP_NUM_SOUND_CHANNELS)
		return;

	// Initializes the psp sound library.
	if (pspSoundInit(channels))
		return;

	// clear the channel_userdata array.
	memset(&channel_userdata, 0, sizeof(sound_userdata_)*PSP_NUM_SOUND_CHANNELS);

	channel_count = channels;
	init = true;
	return;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Shutdowns the sound library.
void SoundTerm()
{
	if (!init)
		return;

	pspSoundEnd();

	// Unload all sound objects.
	unsigned int i;
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object)
		{
			channel_userdata[i].channel = 0;
			channel_userdata[i].playing = false;
			channel_userdata[i].paused = false;
			channel_userdata[i].loop = false;
			channel_userdata[i].stop = false;
			SoundSeekObject(channel_userdata[i].object, 0);
			SoundFreeObject(&(channel_userdata[i].object));
		}

	channel_count = 0;
	init = false;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// create a new sound object.
sound_object SoundCreateObject(int type, int len)
{
	if (type < SOUNDLIB_TYPE_RAW || type > SOUNDLIB_TYPE_WAV)
		return NULL;

	sound_object sobj = malloc(sizeof(sound_object_));
	if (sobj == NULL)
		return NULL;

	sobj->type = type;
	sobj->pdata = NULL;
	sobj->memCount = 0;

	switch (type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			sobj->pdata = malloc(sizeof(sound_raw_));
			if (sobj->pdata)
			{
				memset(sobj->pdata, 0, sizeof(sound_raw_));
				_ampRaw(sobj->pdata, 1.0f);
				NEWRAWBUFFER(sobj->pdata, len)
			}
			break;
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			sobj->pdata = malloc(sizeof(sound_pol_));
			if (sobj->pdata)
			{
				memset(sobj->pdata, 0, sizeof(sound_pol_));
				_ampPol(sobj->pdata, 1.0f);
				NEWPOLBUFFER(sobj->pdata, len)
			}
			break;
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			sobj->pdata = malloc(sizeof(sound_wav_));
			if (sobj->pdata)
			{
				memset(sobj->pdata, 0, sizeof(sound_wav_));
				_ampWav(sobj->pdata, 1.0f);
				NEWWAVBUFFER(sobj->pdata, len)
			}
			break;
	}

	if (sobj->pdata == NULL)
	{
		free(sobj);
		return NULL;
	}

	return sobj;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Frees a sond object & set its pointer to NULL.
void SoundFreeObject(sound_object *psobj)
{
    if (psobj == NULL)
        return;
    if (*psobj == NULL)
        return;

	// Remove a link.
	if (((*psobj)->memCount)-- > 0)
		return;

	// Free the sound stream object.
	if ((*psobj)->pdata)
	{
		switch ((*psobj)->type)
		{
			case SOUNDLIB_TYPE_RAW: // raw sound stream.
				_freeRaw((*psobj)->pdata);
				break;
			case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
				_freePol((*psobj)->pdata);
				break;
			case SOUNDLIB_TYPE_WAV: // wav sound stream.
				_freeWav((*psobj)->pdata);
				break;
		}
		free((*psobj)->pdata);
		(*psobj)->pdata = NULL;
	}

	// Free the object and set the pointer to NULL.
	free((*psobj));
	(*psobj) = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Save a sound object to a file.
void SoundSaveObject(sound_object sobj, const char *path)
{
	if (sobj == NULL)
		return;

	switch (sobj->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			_saveRaw(path, sobj->pdata);
			return;
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			_savePol(path, sobj->pdata);
			return;
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			return;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a sound object from a file.
sound_object SoundLoadObject(const char *path)
{
	if (path == NULL)
        return NULL;
	char* suffix = strrchr(path, '.');
	if (stricmp(suffix, ".raw") == 0)
		return SoundLoadRaw(path);
	if (stricmp(suffix, ".pol") == 0)
		return SoundLoadPol(path);
	if (stricmp(suffix, ".wav") == 0)
        return SoundLoadWav(path);
	else
		return NULL;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Get the buffer from a sound object.
void* SoundGetBufferObject(sound_object sobj, unsigned int pos)
{
	if (sobj == NULL)
		return NULL;

	switch (sobj->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			return _getBufferRaw(sobj->pdata, pos);
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			return _getBufferPol(sobj->pdata, pos);
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			return NULL;
		default:
			return NULL;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the buffer for a sound object.
void SoundSetBufferObject(sound_object sobj, unsigned int pos, void *buf)
{
	void * data = NULL;

	if (buf == NULL)
		return;
	if ((data = SoundGetBufferObject(sobj, pos)) == NULL)
		return;

	switch (sobj->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			*((soundBuf)data) = *((soundBuf)buf);
			return;
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			*((sound_note)data) = *((sound_note)buf);
			return;
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			return;
	}
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Start playing a sound object.
void SoundPlayObject(sound_object sobj, bool rewind, bool loop)
{
	if (sobj == NULL)
		return;

	if (rewind)
		SoundSeekObject(sobj, 0);

	unsigned int i;
	// Check if the object isn't playing.
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object == sobj)
		{
			channel_userdata[i].paused = false;
			channel_userdata[i].loop = loop;
			return;
		}

	// Play the object if there is a free channel.
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object == NULL)
		{
			sobj->memCount++;
			channel_userdata[i].channel = i;
			channel_userdata[i].playing = true;
			channel_userdata[i].paused = false;
			channel_userdata[i].loop = loop;
			channel_userdata[i].stop = false;
			channel_userdata[i].object = sobj;
			pspSoundSetChannelCallback(i, (void*)_soundCallback, &(channel_userdata[i]));
			return;
		}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the loop value of a Playing a sound object.
void SoundLoopObject(sound_object sobj, bool loop)
{
	if (sobj == NULL)
		return;

	// Check if the object is playing.
	unsigned int i;
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object == sobj)
		{
			channel_userdata[i].loop = loop;
			return;
		}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop Playing a sound object.
void SoundStopObject(sound_object sobj)
{
	if (sobj == NULL)
		return;

	// Check if the object is playing.
	unsigned int i;
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object == sobj)
		{
			// Stop playing the sound object.
			channel_userdata[i].stop = true;
			return;
		}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Pause a playing sound object.
void SoundPauseObject(sound_object sobj)
{
	if (sobj == NULL)
		return;

	// Check if the object is playing.
	unsigned int i;
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object == sobj)
		{
			// Pause the playing sound object.
			channel_userdata[i].paused = !channel_userdata[i].paused;
			return;
		}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// the the time of a sound object.
void SoundSeekObject(sound_object sobj, int time)
{
	if (sobj == NULL)
		return;

	switch (sobj->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			_seekRaw(sobj->pdata, time);
			return;
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			_seekPol(sobj->pdata, time);
			return;
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			_seekWav(sobj->pdata, time);
			return;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Rewind a sound object.
void SoundRewindObject(sound_object sobj)
{
	SoundSeekObject(sobj, 0);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Set the amplification value of a sound object.
void SoundAmplification(sound_object sobj, float amp)
{
	if (sobj == NULL)
		return;

	switch (sobj->type)
	{
		case SOUNDLIB_TYPE_RAW: // raw sound stream.
			_ampRaw(sobj->pdata, amp);
			return;
		case SOUNDLIB_TYPE_POL: // polyphonic sound stream.
			_ampPol(sobj->pdata, amp);
			return;
		case SOUNDLIB_TYPE_WAV: // wav sound stream.
			_ampWav(sobj->pdata, amp);
			return;
	}
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Stop Playing all sound objects.
void SoundStopAllObject()
{
	// Stop playing all sound objects.
	unsigned int i;
	for (i = 0; i < channel_count; i++)
		if (channel_userdata[i].object)
			channel_userdata[i].stop = true;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Wav management.
// Base code from: 'pspaalib' - Created by Arshia001
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static sound_object _loadWav(FILE *fp)
{
	sound_object sobj = SoundCreateObject(SOUNDLIB_TYPE_WAV, 0);

	if (sobj == NULL)
		return NULL;

	sound_wav sobj_wav = sobj->pdata;

	long int size = 0;
	int chunks = 0;
	short compressionCode = 0;
	char temp[5];
	temp[4]='\0';

	// Check the file header.
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"RIFF"))
	{
		SoundFreeObject(&sobj);
		return NULL;
	}
	fread(&size, sizeof(int), 1, fp);
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"WAVE"))
	{
		SoundFreeObject(&sobj);
		return NULL;
	}

	// load the wav file.
	while (chunks<2)
	{
		fread(temp, sizeof(char), 4, fp);
		if (!strcmp(temp,"fmt "))
		{
			fread(&size, sizeof(int), 1, fp);
			fread(&compressionCode, sizeof(short), 1, fp);
			if ((compressionCode != 0) && ( compressionCode != 1))
			{
				SoundFreeObject(&sobj);
				return NULL;
			}
			fread(&(sobj_wav->numChannels), sizeof(short), 1, fp);
			fread(&(sobj_wav->sampleRate), sizeof(int), 1, fp);
			fread(&(sobj_wav->bytesPerSecond), sizeof(int), 1, fp);
			fseek(fp, 2, SEEK_CUR);
			fread(&(sobj_wav->sigBytes), sizeof(short), 1, fp);
			(sobj_wav->sigBytes) >>= 3;
			fseek(fp, size-16, SEEK_CUR);
			chunks++;
			continue;
		}
		if (!strcmp(temp,"data"))
		{
			if (chunks<1)
			{
				SoundFreeObject(&sobj);
				return NULL;
			}
			fread(&(sobj_wav->dataLength), sizeof(int), 1, fp);
			sobj_wav->data = (char*) malloc(sobj_wav->dataLength);
			if(sobj_wav->data == NULL)
			{
				SoundFreeObject(&sobj);
				return NULL;
			}
			fread(sobj_wav->data, sizeof(char), sobj_wav->dataLength, fp);
			sobj_wav->dataPos = 0;
			chunks++;
			continue;
		}
		fread(&size, sizeof(int), 1, fp);
		fseek(fp, size, SEEK_CUR);
	}

	return sobj;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _seekWav(sound_wav wav, int time)
{
	if (wav == NULL)
		return;
	wav->dataPos = (time < 0 ? 0 : (time > wav->dataLength ? wav->dataLength : time));
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _ampWav(sound_wav wav, float amp)
{
	if (wav == NULL)
		return;
	wav->amp = amp;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static bool _bufferWav(soundBuf buf, unsigned int reqn, sound_wav wav, bool loop)
{
	unsigned int i, index;
	unsigned int realLength = reqn*wav->sigBytes*wav->numChannels*wav->sampleRate/PSP_SAMPLE_RATE;

	// end of stream.
	if (wav->dataPos + realLength >= wav->dataLength)
	{
		wav->dataPos = 0;
		if (!loop)
		{
			memset(buf, 0, sizeof(soundBuf)*reqn);
			return false;
		}
	}

	// Check wave data.
	if (wav->sigBytes != 1 && wav->sigBytes != 2)
	{
		memset(buf, 0, sizeof(soundBuf)*reqn);
		return false;
	}

	// fill sound buffer.
	for (i = 0; i < reqn; i++)
	{
		if (wav->sigBytes == 1)
		{
			index = wav->numChannels*(int)(i*wav->sampleRate/PSP_SAMPLE_RATE)+wav->dataPos;
			buf->l = (short) *((wav->data) + index) * 0xf00 * wav->amp;
			index += (wav->numChannels > 1 ? 1 : 0);
			buf->r = (short) *((wav->data) + index) * 0xf00 * wav->amp;
		}
		else
		{
			index = wav->numChannels*(int)(i*wav->sampleRate/PSP_SAMPLE_RATE)+(wav->dataPos/2);
			buf->l = ((short*)(wav->data))[index] * wav->amp;
			index += (wav->numChannels > 1 ? 1 : 0);
			buf->r = ((short*)(wav->data))[index] * wav->amp;
		}
		buf++;
	}
	wav->dataPos += realLength;
	return true;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _freeWav(sound_wav wav)
{
	if (wav == NULL)
		return;
	if (wav->data == NULL)
		return;

	free(wav->data);
	wav->data = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a wav from a file.
sound_object SoundLoadWav(const char *path)
{
	sound_object sobj = NULL;
    FILE *fp = NULL;

    if (path == NULL)
        return NULL;
    if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	sobj = _loadWav(fp);

    fclose(fp);
    fp = NULL;

    return sobj;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// Raw management.
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static sound_object _loadRaw(FILE *fp)
{
	sound_object sobj = SoundCreateObject(SOUNDLIB_TYPE_RAW, 0);

	if (sobj == NULL)
		return NULL;

	sound_raw sobj_raw = sobj->pdata;

	long int size = 0;
	char temp[5] = {'\0','\0','\0','\0','\0'};

	// Get the file size.
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &size);
	rewind(fp);

	// Check the file header.
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"RAW%"))
	{
		SoundFreeObject(&sobj);
		return NULL;
	}

	// Read the data lengtht.
	fread(&(sobj_raw->dataLength), sizeof(int), 1, fp);

	// Check the file size.
	if (sobj_raw->dataLength*sizeof(soundBuf_)+8 != size)
	{
		SoundFreeObject(&sobj);
		return NULL;
	}

	// Read the file data.
	sobj_raw->data = (soundBuf) malloc(sobj_raw->dataLength*sizeof(soundBuf_));
	if(sobj_raw->data == NULL)
	{
		SoundFreeObject(&sobj);
		return NULL;
	}
	fread(sobj_raw->data, sizeof(soundBuf_), sobj_raw->dataLength, fp);
	sobj_raw->dataPos = 0;

	return sobj;
}
// -----------------------------------------------------------------------------------------------------
// Save a Raw sound object to a file.
static void _saveRaw (const char *path, sound_raw raw)
{
	if (path == NULL)
        return;

	if (raw == NULL)
        return;

	FILE * fp = fopen(path, "wb");
	if (fp == NULL)
		return;
	char temp[5] = {'R','A','W','%','\0'};

	fwrite(temp, sizeof(char), 4, fp);
	fwrite(&(raw->dataLength), sizeof(int), 1, fp);
	fwrite(raw->data, sizeof(soundBuf_), raw->dataLength, fp);
	fclose(fp);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _seekRaw(sound_raw raw, int time)
{
	if (raw == NULL)
		return;
	raw->dataPos = (time < 0 ? 0 : (time > raw->dataLength ? raw->dataLength : time));
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _ampRaw(sound_raw raw, float amp)
{
	if (raw == NULL)
		return;
	raw->amp = amp;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static soundBuf _getBufferRaw(sound_raw raw, unsigned int pos)
{
	if (pos > raw->dataLength)
		return NULL;
	else
		return ((raw->data) + pos);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static bool _bufferRaw(soundBuf buf, unsigned int reqn, sound_raw raw, bool loop)
{
	unsigned int i, index;

	// fill sound buffer.
	for (i = 0, index = raw->dataPos; i < reqn; i++, index++)
	{
		// end of stream.
		if (index >= raw->dataLength)
		{
			index = 0;
			raw->dataPos = 0;
			if (!loop)
			{
				memset(buf, 0, sizeof(soundBuf)*(reqn-i));
				return false;
			}
		}
		buf->r = ((raw->data) + index)->r * raw->amp;
		buf->l = ((raw->data) + index)->l * raw->amp;
		buf++;
	}
	raw->dataPos += reqn;
	return true;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _freeRaw(sound_raw raw)
{
	if (raw == NULL)
		return;
	if (raw->data == NULL)
		return;

	free(raw->data);
	raw->data = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a raw from a file.
sound_object SoundLoadRaw(const char *path)
{
	sound_object sobj = NULL;
    FILE *fp = NULL;

    if (path == NULL)
        return NULL;
    if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	sobj = _loadRaw(fp);

    fclose(fp);
    fp = NULL;

    return sobj;
}
// -----------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------
// polyphonic management.
// Code From: Basic audio sample - Copyright (c) 2005 Frank Buss <fb@frank-buss.de> (aka Shine)
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static sound_object _loadPol(FILE *fp)
{
	sound_object sobj = SoundCreateObject(SOUNDLIB_TYPE_POL, 0);

	if (sobj == NULL)
		return NULL;

	sound_pol sobj_pol = sobj->pdata;

	long int size = 0;
	char temp[5] = {'\0','\0','\0','\0','\0'};

	// Get the file size.
	fseek(fp, 0, SEEK_END);
	fgetpos(fp, &size);
	rewind(fp);

	// Check the file header.
	fread(temp, sizeof(char), 4, fp);
	if (strcmp(temp,"POL%"))
	{
		SoundFreeObject(&sobj);
		return NULL;
	}

	// Read the data lengtht.
	fread(&(sobj_pol->dataLength), sizeof(int), 1, fp);

	// Check the file size.
	if (sobj_pol->dataLength*sizeof(sound_note_)+8 != size)
	{
		SoundFreeObject(&sobj);
		return NULL;
	}

	// Read the file data.
	sobj_pol->data = (sound_note) malloc(sobj_pol->dataLength*sizeof(sound_note_));
	if(sobj_pol->data == NULL)
	{
		SoundFreeObject(&sobj);
		return NULL;
	}
	fread(sobj_pol->data, sizeof(sound_note_), sobj_pol->dataLength, fp);
	_seekPol(sobj_pol, 0);

	return sobj;
}
// -----------------------------------------------------------------------------------------------------
// Save a polyphon sound object to a file.
static void _savePol (const char *path, sound_pol pol)
{
	if (path == NULL)
        return;

	if (pol == NULL)
        return;

	FILE * fp = fopen(path, "wb");
	if (fp == NULL)
		return;
	char temp[5] = {'P','O','L','%','\0'};

	fwrite(temp, sizeof(char), 4, fp);
	fwrite(&(pol->dataLength), sizeof(int), 1, fp);
	fwrite(pol->data, sizeof(sound_note_), pol->dataLength, fp);
	fclose(fp);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _seekPol(sound_pol pol, int time)
{
	if (pol == NULL)
		return;
	pol->dataPos = (time < 0 ? 0 : (time > pol->dataLength ? pol->dataLength : time));
	pol->time = 0;
	pol->increment = 0.0f;
	pol->index = 0.0f;
	if (pol_data->note != SOUNDLIB_NOTE_PAUSE)
		pol->increment = sound_octaves[pol_data->octave][pol_data->note-1] * ((float)PSP_NUM_SOUND_SAMPLES) / ((float)PSP_SAMPLE_RATE);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _ampPol(sound_pol pol, float amp)
{
	if (pol == NULL)
		return;
	pol->amp = amp;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static sound_note _getBufferPol(sound_pol pol, unsigned int pos)
{
	if (pos > pol->dataLength)
		return NULL;
	else
		return ((pol->data) + pos);
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// calculate current value of attack/delay/sustain/release envelope
static float _adsrPol(float time, float duration)
{
	if (time < 0.0) return 0.0;
	duration -= sound_attack + sound_decay + sound_release;
	if (time < sound_attack) return time / sound_attack;
	time -= sound_attack;
	if (time < sound_decay) return (sound_decay - time) / sound_decay * (1.0 - sound_sustain) + sound_sustain;
	time -= sound_decay;
	if (time < duration) return sound_sustain;
	time -= duration;
	if (time < sound_release) return (sound_release - time) / sound_release * sound_sustain;
	return 0.0;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static bool _bufferPol(soundBuf buf, unsigned int reqn, sound_pol pol, bool loop)
{
	unsigned int i;
	float time = 0;
	float scale = pol_data->volume * pol->amp;
	short value = 0;

	// fill sound buffer.
	for (i = 0; i < reqn; i++)
	{
		// calculate the real time.
		time = ((float)pol->time) / ((float)PSP_SAMPLE_RATE);

		// Go to the next note.
		if ((pol->time)++ >= pol_data->duration)
		{
			pol->time = 0;
			pol->increment = 0.0f;
			pol->index = 0.0f;
			// end of stream.
			if (++(pol->dataPos) >= pol->dataLength)
			{
				pol->dataPos = 0;
				if (!loop)
				{
					memset(buf, 0, sizeof(soundBuf)*(reqn-i));
					return false;
				}
			}
			// Select to the next note.
			if (pol_data->note != SOUNDLIB_NOTE_PAUSE)
				pol->increment = sound_octaves[pol_data->octave][pol_data->note-1] * ((float)SOUNDLIB_SAMPEL_COUNT) / ((float)PSP_SAMPLE_RATE);
			scale = pol_data->volume * pol->amp;
		}

		// Add the note to the sound buffer.
		if (pol->increment)
		{
			value = pol_frerequency * _adsrPol(time, ((float)pol_data->duration) / ((float)PSP_SAMPLE_RATE)) * scale;
			pol->index += pol->increment;
			if (pol->index >= SOUNDLIB_SAMPEL_COUNT) pol->index -= (float)SOUNDLIB_SAMPEL_COUNT;
		}
		else
			value = 0;

		buf->r = value;
		buf->l = value;
		buf++;
	}
	return true;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
static void _freePol(sound_pol pol)
{
	if (pol == NULL)
		return;
	if (pol->data == NULL)
		return;

	free(pol->data);
	pol->data = NULL;
}
// -----------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------
// Load a polyphon from a file.
sound_object SoundLoadPol(const char *path)
{
	sound_object sobj = NULL;
    FILE *fp = NULL;

    if (path == NULL)
        return NULL;
    if ((fp = fopen(path, "rb")) == NULL)
        return NULL;

	sobj = _loadPol(fp);

    fclose(fp);
    fp = NULL;

    return sobj;
}
// -----------------------------------------------------------------------------------------------------
