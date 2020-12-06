#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "output.h"

int main()
{
	/* size of the final image in pixels */
	unsigned int hight_in_pixel = 1080;
	unsigned int width_in_pixel = 1920;
	unsigned short int bytes_per_pixel = 3;
	unsigned int zero = 0;

	/* calculate file size and allocate memory */
	unsigned int row_length_in_bytes = width_in_pixel * bytes_per_pixel;
	unsigned int row_length_in_bytes_padded = (unsigned int)(ceil(((double)row_length_in_bytes) / 4.0) * 4.0);
	unsigned int payload_length = row_length_in_bytes_padded * hight_in_pixel;
	unsigned int file_header_length = 14;
	unsigned int info_header_length = 40;
	unsigned int header_length = file_header_length + info_header_length;
	unsigned int file_length = header_length + payload_length;
	unsigned short int biBitCount = bytes_per_pixel * 8;
	unsigned char *ptr = malloc(file_length);

	/* BITMAPFILEHEADER */
	memcpy(ptr + 0, &"BM", 2);			/* bfType */
	memcpy(ptr + 2, &file_length, 4);	/* bfSize */
	memcpy(ptr + 6, &zero, 4);			/* bfReserved */
	memcpy(ptr + 9, &header_length, 4); /* bfOffBits */
	/* BITMAPINFOHEADER */
	memcpy(ptr + 14, &info_header_length, 4); /* biSize */
	memcpy(ptr + 18, &width_in_pixel, 4);	  /* biWidth */
	memcpy(ptr + 22, &hight_in_pixel, 4);	  /* biHeight */
	memcpy(ptr + 26, &zero, 2);				  /* biPlanes */
	memcpy(ptr + 28, &biBitCount, 2);		  /* biBitCount */
	memcpy(ptr + 30, &zero, 4);				  /* biCompression */
	memcpy(ptr + 34, &payload_length, 4);	  /* biSizeImage */
	memcpy(ptr + 38, &zero, 4);				  /* biXPelsPerMeter */
	memcpy(ptr + 42, &zero, 4);				  /* biYPelsPerMeter */
	memcpy(ptr + 46, &zero, 4);				  /* biClrUsed */
	memcpy(ptr + 50, &zero, 4);				  /* biClrImportant */
	memcpy(ptr + 50, &zero, 4);				  /* biClrImportant */

	{
		unsigned int hight_pixel_written = hight_in_pixel;
		unsigned char *ptr_row = ptr + file_length - row_length_in_bytes_padded;
		unsigned char *header_end = ptr + header_length;
		srand((unsigned int)time(NULL));
		do
		{
			unsigned int bytes_done = 0;
			unsigned int width_pixel_written = 0;
			while (bytes_done < row_length_in_bytes)
			{
				/* render pixel here */
				unsigned char red = (rand() % (255 - 0)) + 0;
				unsigned char green = (rand() % (255 - 0)) + 0;
				unsigned char blue = (rand() % (255 - 0)) + 0;
				/* render pixel here */

				memcpy(ptr_row + bytes_done++, &blue, 1);
				memcpy(ptr_row + bytes_done++, &green, 1);
				memcpy(ptr_row + bytes_done++, &red, 1);
				width_pixel_written++;
			}
			while (bytes_done < row_length_in_bytes_padded)
				memcpy(ptr_row + bytes_done++, &zero, 1);
			ptr_row -= row_length_in_bytes_padded;
			hight_pixel_written--;
		} while (ptr_row >= header_end);

		/* write values for all rows */
		write_file("./test.bmp", ptr, file_length);
	}

	free(ptr);
}
