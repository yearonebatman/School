/*			Adam Martin
 *			CS130
 *			07/10/2021
 *			Lab3
 *			Bitmap Lab to test our ability to open a bitmap file and read information from it then swap any information as needed and output back to a file again while keeping bitmap intact
 *			Helped by: TA - C Muncey
 */
#include <iostream>
#include <cstdint>
#include <cstdio>
#include <string>
#include <cstring>
using namespace std;

/* Remove the padding that C++ would add to the struct and Create BitmapFileHeader struct */
#pragma pack(push, 2)
struct BitmapFileHeader
{
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offset;
};

/* Create BitmapInfoHeader struct */
struct BitmapInfoHeader
{
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

/* Create Pixel struct with 3 unsigned ints of 8 bits/1 byte each*/
struct Pixel
{
	uint8_t blue;
	uint8_t green;
	uint8_t red;
};

int main(int argc, char *argv[])
{
	string nameOfFileIn, nameOfFileOut;
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;
	int i, j, padding, test = 1;
	size_t bytes;
	FILE* f;

	/* Have user input the name of the file that needs to be read and grab information from*/
	cout << "Name of file to read (make sure to include .bmp): ";
	cin >> nameOfFileIn;

	/* Try to open file given by user and give error if couldnt open1*/
	f = fopen(nameOfFileIn.c_str(), "rb");
	if (f == nullptr)
	{
		perror(nameOfFileIn.c_str());
		return -1;
	}



	/* Read the file header, make sure it read properly */
	bytes = fread(&bfh, 1, sizeof(bfh), f);
	if (bytes != sizeof(bfh))
	{
		printf("I was unable to read all bytes of file header.. \n");
		return -1;
	}

	/* Read the info header, make sure it worked read properly */
	bytes = fread(&bih, 1, sizeof(bih), f);
	if (bytes != sizeof(bih))
	{
		printf("I was unable to read all bytes of info header. \n");
		return -1;
	}



	/* seek to offset bit just in case it's different and calculate padding*/
	fseek(f, bfh.offset, SEEK_SET);
	padding = (4 - (bih.width * sizeof(Pixel)) % 4) % 4;

	/* Allocate memory for the pixels of size height * width */
	Pixel* parray = new Pixel[bih.height * bih.width] ;
	
	/* Start for loop to being reading information from open file to newly created parray*/
	for (i = 0; i < bih.height; ++i)
	{

		/* Start reading and use bih.width * i to change reading location each time*/
		bytes = fread(parray + (bih.width * i), 1, bih.width * sizeof(Pixel), f);
		/* Make sure it worked and print error if not */
		if (bytes != bih.width * sizeof(Pixel))
		{
			// TODO Let the user know something went wrong
			printf("Only read %lu bytes.\n", bytes);
			printf("Should have read %lu.\n", (bih.width * sizeof(*parray)));
		}
		
		/* Skip over extra padding so not pulling it into parray*/
		fseek(f, padding, SEEK_CUR);
	}

	/* Close opened file because done reading to parray*/
	fclose(f);

	/* Go through every pixel in parray and invert the color */
	for (i = 0; i < (bih.height * bih.width); ++i)
	{
		parray[i].blue = 255 - parray[i].blue;
		parray[i].green = 255 - parray[i].green;
		parray[i].red = 255 - parray[i].red;
	}
	
	/* Use double for loop and swap function to begin going row by row and flipping picture, make sure to do width/2 because flipping two pixels at a time */
	for (i = 0; i < bih.height; ++i)
	{
		for (j = 0; j < (bih.width / 2); ++j)
		{
			swap(parray[i * bih.width + j], parray[(i * bih.width) + (bih.width - 1) - j]);
		}
	}

	/* Again ask user where they want file output to */
	cout << "Name of file to Output (make sure to include .bmp): ";
	cin >> nameOfFileOut;

	/* Open stream for writing and let users know if something went wrong */
	f = fopen(nameOfFileOut.c_str(), "wb");
	if (f == nullptr)
	{
		// Let the user know something went wrong
		perror(nameOfFileOut.c_str());
		return -1;
	}

	/* Attemp to write first the same BitmapFileHeader and give error if something went wrong */
	bytes = fwrite(&bfh, 1, sizeof(bfh), f);
	if (bytes != sizeof(bfh))
	{
		// TODO Let the user know something went wrong
		printf("I was unable to write all bytes of file header. \n");
		return -1;
	}

	/* Attempt to write the same BitmapInfoHeader and giver error if something went wrong*/
	bytes = fwrite(&bih, 1, sizeof(bih), f);
	if (bytes != sizeof(bih))
	{
		printf("I was unable to write all bytes of info header. \n");
		return -1;
	}

	/* Seek to offset bit once again to make sure starting in correct place */
	fseek(f, bfh.offset, SEEK_SET);

	/* Same as reading into parray before but instead writing new changed information from parray into the output file*/
	for (i = 0; i < bih.height; ++i)
	{

		bytes = fwrite(parray + (bih.width * i), 1, bih.width * sizeof(Pixel), f);
		if (bytes != bih.width * sizeof(Pixel))
		{
			printf("Only wrote %lu bytes.\n", bytes);
			printf("Should have wrote %lu.\n", (bih.width * sizeof(*parray)));
		}
		
		/* Make sure to make own padding or else information wont make sense*/
		bytes = fwrite("0", 1, padding, f);

	}

	/* Close file and delete parray */
	fclose(f);
	delete[] parray;
	
	return(0);
}
