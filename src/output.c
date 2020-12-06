#include <stdio.h>
#include "output.h"

void write_file(char *name, void *data, unsigned int length)
{
	FILE *file_pointer = fopen(name, "wb");
	fwrite(data, 1, length, file_pointer);
	fclose(file_pointer);
}
