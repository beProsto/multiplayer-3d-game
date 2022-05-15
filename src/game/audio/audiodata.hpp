#pragma once

#include <stdint.h>
#include <vector>

struct AudioData {
	uint32_t frequency;
	int32_t format;
	std::vector<uint8_t> data;
};