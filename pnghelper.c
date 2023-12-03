#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <png.h>
#include "pnghelper.h"

inline void setRGB(png_byte *ptr, uint32_t val)
{
	int r = ((val >> 16) & 0xFF);
	int g = ((val >>  8) & 0xFF);
	int b = ((val >>  0) & 0xFF);
	__unused
	int a = ((val >> 24) & 0xFF);

	ptr[0] = r;
	ptr[1] = g;
	ptr[2] = b;
}

int write_png_file(const char* filename, const char* title, int width, int height, uint32_t* pixels)
{
	FILE *fp = fopen(filename, "wb");
	if (!fp)
	{
		return 1;
	}

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)	
	{
		return 2;
	}


	png_infop info = png_create_info_struct(png);
	if (!info)
	{
		return 2;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		return 3;
	}

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		width, height,
		8,
		PNG_COLOR_TYPE_RGB,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	// Set title
	if (title != NULL) 
	{
		png_text title_text;
		title_text.compression = PNG_TEXT_COMPRESSION_NONE;
		title_text.key = "Title";
		title_text.text = (char*)title;
		png_set_text(png, info, &title_text, 1);
	}

	png_write_info(png, info);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	// png_set_filler(png, 0, PNG_FILLER_AFTER);

 	// Allocate memory for one row (3 bytes per pixel - RGB)
	png_bytep row = (png_bytep)malloc(3 * width * sizeof(png_byte));

	// Write image data
	int x, y;
	for (y = 0; y < height; y++) 
	{
		for (x = 0; x < width; x++) 
		{
			setRGB(&(row[x * 3]), pixels[y * width + x]);
		}

		png_write_row(png, row);
	}	

	// png_write_image(png, image.row_pointers);
	png_write_end(png, NULL);

	if (fp != NULL) fclose(fp);
	if (info != NULL) png_free_data(png, info, PNG_FREE_ALL, -1);
	if (png != NULL) png_destroy_write_struct(&png, &info);
	if (row != NULL) free(row);

	return 0;
}
