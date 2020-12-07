#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "output.h"

int main()
{
	double MAX_ITER = 255;
	double RE_START = -2;
	double RE_END = 1;
	double IM_START = -1;
	double IM_END = 1;

	/* size of the final image in pixels */
	unsigned int WIDTH = 3840 * 2;
	unsigned int HEIGHT = 1600 * 2;
	unsigned short int bytes_per_pixel = 3;
	unsigned int zero = 0;

	/* calculate file size and allocate memory */
	unsigned int row_length_in_bytes = WIDTH * bytes_per_pixel;
	unsigned int row_length_in_bytes_padded = (unsigned int)(ceil(((double)row_length_in_bytes) / 4.0) * 4.0);
	unsigned int payload_length = row_length_in_bytes_padded * HEIGHT;
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
	memcpy(ptr + 18, &WIDTH, 4);			  /* biWidth */
	memcpy(ptr + 22, &HEIGHT, 4);			  /* biHeight */
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
		unsigned int y = HEIGHT;
		unsigned char *ptr_row = ptr + file_length - row_length_in_bytes_padded;
		unsigned char *header_end = ptr + header_length;
		srand((unsigned int)time(NULL));
		do
		{
			unsigned int bytes_done = 0;
			unsigned int x = 0;
			while (bytes_done < row_length_in_bytes)
			{
				double real = RE_START + (x / (double)WIDTH) * (RE_END - RE_START);
				double imaginary = IM_START + (y / (double)HEIGHT) * (IM_END - IM_START);

				double u = 0.0;
				double v = 0.0;
				double u2 = u * u;
				double v2 = v * v;

				int k;
				for (k = 1; k < MAX_ITER && (u2 + v2 < 4.0); k++)
				{
					v = 2 * u * v + imaginary;
					u = u2 - v2 + real;
					u2 = u * u;
					v2 = v * v;
				}

				{
					unsigned char red = (unsigned char)k;
					unsigned char green = (unsigned char)(fabs(u) / 2.0) * 255;
					unsigned char blue = (unsigned char)(fabs(u2) / 2.0) * 255;

					memcpy(ptr_row + bytes_done++, &blue, 1);
					memcpy(ptr_row + bytes_done++, &green, 1);
					memcpy(ptr_row + bytes_done++, &red, 1);
				}
				x++;
			}
			while (bytes_done < row_length_in_bytes_padded)
				memcpy(ptr_row + bytes_done++, &zero, 1);
			ptr_row -= row_length_in_bytes_padded;
			y--;
		} while (ptr_row >= header_end);

		/* write values for all rows */
		write_file("./test.bmp", ptr, file_length);
	}

	free(ptr);
}
