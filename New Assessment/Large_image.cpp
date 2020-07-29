#include "Large_image.h"
//minipulation of largeImage because arrays are private

//selects the sectino of the larger image that is in equal size to RefImage of wally
//returns it as a 1D array



double* Large::BlockSelect(int x, int y) {

	int current = 0;
	double * sample = new double[1764];//1764 = RefHeight(49) * RefWidth(36)

	for (int Row = y; Row < y + 49; Row++)//loops from start row y, loops to the max height of the RefImage(which is size of selection)
	{
		for (int Col = x; Col < x + 36; Col++)
		{
			sample[current] = this->array2d[Row][Col];//using coordinates to from  large image select and put in order as 1D sample
			current++;
		}

	}
	return sample;
}

void Large::OutLine(int x, int y, int colour) {
	//colour = 0-255 (0 =black, 255 = white)

	for (int a = y; a < y + 49; a++)
	{
		for (int b = x; b < x + 36; b++)
		{
			if (((a >= y) && (a < y + 3)) || ((b >= x) && (b < x + 3)) || ((a < y + 49) && (a >= y + 46)) || ((b < x + 36) && (b >= x + 33)))
				this->array2d[a][b] = colour; // Change pixel value to black in order to show the location of N images
		}
	}
}

