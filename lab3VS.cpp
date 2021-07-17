#include <iostream>
#include <cstdint>
#include <cstdio>
using namespace std;

/* This removes the padding that C++ would */
/* automatically add to your structs */
#pragma pack(push, 2)
struct BitmapFileHeader
{
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
};

struct BitmapInfoHeader
{
	// TODO Write the info header members
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitcount;
	uint32_t compression;
	uint32_t imagesize;
	int32_t x_pixels_per_meter;
	int32_t y_pixels_per_meter;
	uint32_t color_used;
	uint32_t color_important;
};
#pragma pack(pop)

struct Pixel
{
	// TODO make a struct that contains pixels
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

int main(int argc, char *argv[])
{
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;
	int i, j, padding;
	//Pixel* pixels;
	size_t bytes;
	FILE* f;


	// TODO This should open the file given by the user
	// Remember what you need to do before using arguments
	f = fopen("lab.bmp", "rb");
	if (f == nullptr)
	{
		// Let the user know something went wrong
		perror("lab.bmp");
		return -1;
	}

	/* Read the file header, make sure it read properly */
	bytes = fread(&bfh, 1, sizeof(bfh), f);
	if (bytes != sizeof(bfh))
	{
		// TODO Let the user know something went wrong
		printf("I was unable to read all bytes of file header.. \n");
		return -1;
	}

	printf("I was able to read %lu bytes of file header.. \n", bytes);

	// TODO Read the info header and make sure it worked
	bytes = fread(&bih, 1, sizeof(bih), f);
	if (bytes != sizeof(bih))
	{
		// TODO Let the user know something went wrong
		printf("I was unable to read all bytes of info header. \n");
		return -1;
	}

	printf("I was able to read %lu bytes of file header. \n", bytes);

	/* Calculate the number of padding bytes per row */
	//padding = (4 - (3 * sizeof(Pixel)) % 4) % 4;

	/* A much cooler (and faster) way to do it */
	/* This only works when padding to a power of 2 though */
	//padding = ((3 * sizeof(Pixel)) + 3) & -4;
	//cout << padding;
	/* The pixel data starts at the offset given in the file header */
	fseek(f, bfh.offset, SEEK_SET);
	padding = (4 - (bih.width * sizeof(Pixel)) % 4) % 4;
	// TODO Allocate memory for the pixels
	// Remember to delete it when you're done with it
	Pixel* parray = new Pixel[bih.height * bih.width] ;
	//cout << bih.width * bih.height;
	
	
	//padding = ((3 * sizeof(*pixels)) + 3) & -4;
	cout << bih.width * sizeof(Pixel) << endl;
	//cout << sizeof(Pixel) << endl;
	cout << padding << endl;
	//cout << sizeof(*parray) << endl;
	
	// TODO Read the pixels row by row (not one at a time)
	for (i = 0; i < bih.height; ++i)
	{

		cout << (bih.width * sizeof(*parray)) << endl;
		// TODO Where do we read to, and how much do we read
		bytes = fread(parray + (bih.width * i), 1, bih.width * sizeof(Pixel), f);
		cout << bytes << endl;
		// TODO Make sure that fread worked too
		if (bytes != bih.width * sizeof(Pixel))
		{
			// TODO Let the user know something went wrong
			printf("Only read %lu bytes.\n", bytes);
			printf("Should have read %d.\n", (bih.width * sizeof(*parray)));
		}
		
		// TODO Skip over the padding
		//fseek(f, padding, SEEK_CUR);
		cout << ftell(f) << endl;
		cout << i << endl;
	}

	// TODO We're done with the input file, so we can close it
	fclose(f);
	
	// TODO Invert the color of every pixel

	// TODO Flip the image along the Y axis (left becomes right)
	/*fseek(f, bfh.offset, SEEK_SET);
	for (i = 0; i < bih.height; ++i)
	{
		for (j = 0; j < (bih.width / 2); ++j)
		{
			swap(parray[i * bih.width + j], parray[(i * bih.width) + (bih.width - 1) - j]);
		}
	}*/

	// TODO Write the new bmp image
	// The header information won't change, so you can use the one you have
	f = fopen("output.bmp", "wb");
	if (f == nullptr)
	{
		// Let the user know something went wrong
		perror("output.bmp");
		return -1;
	}

	bytes = fwrite(&bfh, 1, sizeof(bfh), f);
	if (bytes != sizeof(bfh))
	{
		// TODO Let the user know something went wrong
		printf("I was unable to write all bytes of file header. \n");
		return -1;
	}
	cout << bytes << endl;
	bytes = fwrite(&bih, 1, sizeof(bih), f);
	if (bytes != sizeof(bih))
	{
		// TODO Let the user know something went wrong
		printf("I was unable to write all bytes of info header. \n");
		return -1;
	}
	cout << bytes << endl;
	cout << bfh.offset << endl;
	fseek(f, bfh.offset, SEEK_SET);
	cout << ftell(f) << endl;

	for (i = 0; i < bih.height; ++i)
	{

		bytes = fwrite(parray + (bih.width * i), 1, bih.width * sizeof(Pixel), f);
		cout << bytes << endl;
		if (bytes != bih.width * sizeof(Pixel))
		{
			// TODO Let the user know something went wrong
			printf("Only read %lu bytes.\n", bytes);
			printf("Should have read %d.\n", (bih.width * sizeof(*parray)));
		}

		cout << ftell(f) << endl;
		cout << i << endl;
	}

	fclose(f);
	
	return(0);
}
