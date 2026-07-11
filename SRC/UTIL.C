#include <Windows.h>
#include <stdlib.h>


int serializeInt(int i) {
	return 0;	
}

int serializeStr(LPSTR str) {
	return 0;
}

int serializeShort(short s) {
	return 0;
}

int murmurHash(void *data, size_t len) {
	size_t i;
	unsigned int hash = 5381;

	for(i = 0; i < len; i++) {
		hash += (hash << 5) + ((unsigned char *) data)[i];
	}

	return hash;
}
