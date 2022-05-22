/***
 * 
 * This is a script. It's meant to copy windres file's bytes over to a cpp file.
 * It's only usable for windows users. It's useless for yall, linux folk.
 * 
***/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int main() {
	// check if the windres file exists
	FILE* windres_file = fopen("./build/icon.res", "rb");
	if(windres_file == NULL) {
		// There is no windres file to be found!
		printf("Couldn't find a windres binary!\n");
		return 0;
	}

	// We try to load the file 2 times
	FILE* cpp_file;
	for(uint8_t i = 0; i < 2; i++) {
		cpp_file = fopen("./src/common/windres/_icon.cpp", "wb");
		if(cpp_file == NULL) {
			system("mkdir src\\common\\windres");
		}
		else {
			break;
		}
	}
	if(cpp_file == NULL) {
		// There is no way to write to the file!
		printf("Couldn't create a source file!\n");
		return 0;
	}




	fclose(cpp_file);
	fclose(windres_file);

	return 0;
}