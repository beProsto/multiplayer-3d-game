#pragma once

#include <AL/al.h>
#include <AL/alc.h>

class AudioBuffer {
public:
	AudioBuffer();
	~AudioBuffer();
};

class AudioSource {
public:
	AudioSource();
	~AudioSource();
};

/// VORBIS

// #include <al.h>
// #include <alc.h>
// #include <vorbis/vorbisfile.h>
// #include <stdlib.h>
// #include <cstdio>
// #include <iostream>
// #include <vector>
 
// //#include <windows.h>
// #define BUFFER_SIZE     32768       // 32 KB buffers
 
// using namespace std;
 
// // This function loads a .ogg file into a memory buffer and returns
// // the format and frequency.
// void LoadOGG(char *fileName, vector<char> &buffer, ALenum &format, ALsizei &freq)
//     {
//     int endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
//     int bitStream;
//     long bytes;
//     char array[BUFFER_SIZE];                // Local fixed size array
//     FILE *f;
 
//     // Open for binary reading
//     f = fopen(fileName, "rb");
 
//     if (f == NULL)
//         {
//         cerr << "Cannot open " << fileName << " for reading..." << endl;
//         exit(-1);
//         }
//     // end if
 
//     vorbis_info *pInfo;
//     OggVorbis_File oggFile;
 
//     // Try opening the given file
//     if (ov_open(f, &oggFile, NULL, 0) != 0)
//         {
//         cerr << "Error opening " << fileName << " for decoding..." << endl;
//         exit(-1);
//         }
//     // end if
 
//     // Get some information about the OGG file
//     pInfo = ov_info(&oggFile, -1);
 
//     // Check the number of channels... always use 16-bit samples
//     if (pInfo->channels == 1)
//         format = AL_FORMAT_MONO16;
//     else
//         format = AL_FORMAT_STEREO16;
//     // end if
 
//     // The frequency of the sampling rate
//     freq = pInfo->rate;
 
//     // Keep reading until all is read
//     do
//         {
//         // Read up to a buffer's worth of decoded sound data
//         bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
 
//         if (bytes < 0)
//             {
//             ov_clear(&oggFile);
//             cerr << "Error decoding " << fileName << "..." << endl;
//             exit(-1);
//             }
//         // end if
 
//         // Append to end of buffer
//         buffer.insert(buffer.end(), array, array + bytes);
//         }
//     while (bytes > 0);
 
//     // Clean up!
//     ov_clear(&oggFile);
//     }
// // end of LoadOGG
 
// int main(int argc, char *argv[])
//     {
//     ALint state;                            // The state of the sound source
//     ALuint bufferID;                        // The OpenAL sound buffer ID
//     ALuint sourceID;                        // The OpenAL sound source
//     ALenum format;                          // The sound data format
//     ALsizei freq;                           // The frequency of the sound data
//     vector<char> bufferData;                // The sound buffer data from file
 
//     // Make sure there is a file name
//     if (argc < 2)
//         {
//         cerr << "Syntax: " << argv&#91;0&#93; << " OGGFile" << endl;
//         return -1;
//         }
 
//     // Initialize the OpenAL library
//     //alutInit(&argc, argv);
 
//     //ALDEVICEINFO  ALDeviceInfo;
//     char *devices;
//     int index;
//     const char *defaultDeviceName;
//     const char *actualDeviceName;
 
//     if (alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT")) {
//         devices = (char *)alcGetString(NULL, ALC_DEVICE_SPECIFIER);
//         defaultDeviceName = (char *)alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
 
//         //OutputDebugString(defaultDeviceName);
//     }
 
//     ALCcontext *pContext = NULL;
//     ALCdevice *pDevice = NULL;
 
//     pDevice = alcOpenDevice(defaultDeviceName);
//     if (pDevice)
//     {
//         pContext = alcCreateContext(pDevice, NULL);
//         if (pContext)
//         {
//             //OutputDebugString("\nOpened Device\n");
//             //OutputDebugString(alcGetString(pDevice, ALC_DEVICE_SPECIFIER));
//             alcMakeContextCurrent(pContext);
 
//         }
//         else
//         {
//             alcCloseDevice(pDevice);
//         }
//     }
 
//     // Create sound buffer and source
//     alGenBuffers(1, &bufferID);
//     alGenSources(1, &sourceID);
 
//     // Set the source and listener to the same location
//     alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
//     alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
 
//     // Load the OGG file into memory
//     LoadOGG(argv&#91;1&#93;, bufferData, format, freq);
 
//     // Upload sound data to buffer
//     alBufferData(bufferID, format, &bufferData&#91;0&#93;, static_cast<ALsizei>(bufferData.size()), freq);
 
//     // Attach sound buffer to source
//     alSourcei(sourceID, AL_BUFFER, bufferID);
 
//     // Finally, play the sound!!!
//     alSourcePlay(sourceID);
 
//     // This is a busy wait loop but should be good enough for example purpose
//     do
//         {
//         // Query the state of the souce
//         alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
//         }
//     while (state != AL_STOPPED);
 
//     // Clean up sound buffer and source
//     alDeleteBuffers(1, &bufferID);
//     alDeleteSources(1, &sourceID);
 
//     // Clean up the OpenAL library
//     alcMakeContextCurrent(NULL);
//     alcDestroyContext(pContext);
//     alcCloseDevice(pDevice);
 
//     return 0;
//     }
// // end of main