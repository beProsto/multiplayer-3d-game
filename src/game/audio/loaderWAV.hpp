#pragma once

#include "audiodata.hpp"

#include <stdint.h>

namespace LoaderWAV {

struct WAVHeader {
	uint32_t riff;
	uint32_t size, chunkSize;
	int16_t formatType, channels;
	uint32_t sampleRate, avgBytesPerSec;
	int16_t bytesPerSample, bitsPerSample;
	uint32_t dataSize;
};

inline AudioData Load(const std::string& _filename) {
	FILE* audioFile = fopen("assets/audio.wav", "rb");
	AudioData finalData;
	WAVHeader fileHeader;

	if(audioFile == nullptr) {
		fprintf(stderr, "The file couldn't be loaded!\n");
		return finalData;
	}
	
	fread(&fileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(fileHeader.riff != 1179011410 /* Non null terminated "RIFF" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'RIFF' found)\n");
		return finalData;
	}

	fread(&fileHeader.size, sizeof(uint32_t), 1, audioFile);

	fread(&fileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(fileHeader.riff != 1163280727 /* Non null terminated "WAVE" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'WAVE' found)\n");
		return finalData;
	}

	fread(&fileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(fileHeader.riff != 544501094 /* Non null terminated "fmt " string */) {
		fprintf(stderr, "The file isn't WAV! (No 'fmt ' found)\n");
		return finalData;
	}

	fread(&fileHeader.chunkSize, sizeof(uint32_t), 1, audioFile);
	fread(&fileHeader.formatType, sizeof(int16_t), 1, audioFile);
	fread(&fileHeader.channels, sizeof(int16_t), 1, audioFile);
	fread(&fileHeader.sampleRate, sizeof(uint32_t), 1, audioFile);
	fread(&fileHeader.avgBytesPerSec, sizeof(uint32_t), 1, audioFile);
	fread(&fileHeader.bytesPerSample, sizeof(int16_t), 1, audioFile);
	fread(&fileHeader.bitsPerSample, sizeof(int16_t), 1, audioFile);

	fread(&fileHeader.riff, sizeof(uint32_t), 1, audioFile);
	if(fileHeader.riff == 1414744396 /* Non null terminated "LIST" string */) {
		fprintf(stderr, "Parsing through LIST...\n");

		uint32_t sizeToSkip = 0;
		fread(&sizeToSkip, sizeof(uint32_t), 1, audioFile);
		fprintf(stderr, "Skipping %u bytes...\n", sizeToSkip);
		fseek(audioFile, sizeToSkip, SEEK_CUR);

		fread(&fileHeader.riff, sizeof(uint32_t), 1, audioFile);
	}
	if(fileHeader.riff != 1635017060 /* Non null terminated "data" string */) {
		fprintf(stderr, "The file isn't WAV! (No 'data' found)\n");
		return finalData;
	}

	fread(&fileHeader.dataSize, sizeof(uint32_t), 1, audioFile);
	fprintf(stdout, "Reading %u bytes of WAVE data...\n", fileHeader.dataSize);

	finalData.data.resize(fileHeader.dataSize);
	fread(finalData.data.data(), sizeof(unsigned char), fileHeader.dataSize, audioFile);

	finalData.frequency = fileHeader.sampleRate;
	// you may ask why
	// for i am lazy
	// thus i tasked myself with finding an easier way
	// and came up with something that took even longer
	finalData.format = 0x1100;
	finalData.format += ((fileHeader.channels - 1 ) * 2) + ((fileHeader.bitsPerSample / 8) - 1);

	// if(format == AL_FORMAT_MONO8) {
	// 	std::cout << "MONO8\n";
	// }
	// else if(format == AL_FORMAT_MONO16) {
	// 	std::cout << "MONO16\n";
	// }
	// else if(format == AL_FORMAT_STEREO8) {
	// 	std::cout << "STEREO8\n";
	// }
	// else if(format == AL_FORMAT_STEREO16) {
	// 	std::cout << "STEREO16\n";
	// }
	// else {
	// 	std::cout << "Unknown Format!\n";
	// }
	fclose(audioFile);

	return finalData;
}

}