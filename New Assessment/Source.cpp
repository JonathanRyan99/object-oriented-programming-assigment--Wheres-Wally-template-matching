//programming solution to wheres wally
//author Jonathan Ryan
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include "Matrix.h"
#include "Large_image.h"
#include "Reference_image.h"

using namespace std;

void write_pgm(string filename, double *data, int sizeR, int sizeC)
{
	int i;
	int Q = 255;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[sizeR*sizeC];

	for (i = 0; i < sizeR*sizeC; i++)
		image[i] = (unsigned char)data[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << sizeC << " " << sizeR << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (sizeR*sizeC) * sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;

}
//reads in from text files and outputs 1D array
double* read_text(string fileName, int sizeR, int sizeC)
{
	double* data = new double[sizeR*sizeC];
	int i = 0;
	ifstream myfile(fileName);
	if (myfile.is_open())
	{

		while (myfile.good())
		{
			if (i > sizeR*sizeC - 1) break;
			myfile >> *(data + i);

			i++;
		}
		myfile.close();
	}

	else cout << "Unable to open file" << endl;

	return data;
}



double SSDCal(double** sample, double** Ref)//calculates the SSD value of the passed in section
{
	int Y = 49;
	int X = 36;
	double variance = 0.0;

	for (int i = 0; i < Y; i++)//nested loops to access each "pixel"
	{
		for (int j = 0; j < X; j++)
		{
			if (Ref[i][j] < 255)//wally has a lot of white space in the template, only using values under 255 so that those points are not compaired
			{
				variance += ((sample[i][j] - Ref[i][j]) * (sample[i][j] - Ref[i][j])) / 1764;

			}

		}
	}
	
	return sqrt(variance);

}



//input validation from user
int validate() {
	int UserNum;
	while (!(cin >> UserNum))//while cin isnt an int
	{
		cout << "error please enter a valid interger" << endl;
		cin.clear();
		cin.ignore(132, '\n');
	}
	for (int i = 0; i < 10; i++)//clears some space on console
		cout << endl;
	
	return UserNum;
}



//holds information from about a selection
struct comparedresults {
	double simalarity;
	int x;
	int y;
};

//operator overload function to allow sort function to reorder the MatchData vector that holds the comparedresults structs
bool operator<(const comparedresults& input1, const comparedresults& input2)
{
	return input1.simalarity < input2.simalarity;
}


int main() {

	//first create the large and the ref image in 2D
	int RefWidth = 36;
	int RefHeight = 49;
	string RefName = "Wally_grey.txt";


	int LargeWidth = 1024;
	int LargeHeight = 768;
	string LargeName = "Cluttered_scene.txt";

	

	double* TempData = 0; //contains temp data is refreshed/deleted often
	TempData = read_text(RefName, RefWidth, RefHeight);	 // Read in the file as an array of doubles
	Reference *RefImage = new Reference(RefWidth, RefHeight, TempData);	// Matrix constructor overload creates 2D array of input
	cout << "Wally template loaded" << endl;

	TempData = 0;
	TempData = read_text(LargeName, LargeWidth, LargeHeight);
	Large *LargeImage = new Large(LargeWidth, LargeHeight, TempData);
	cout << "Large image loaded" << endl;
	delete[] TempData;

	
	
//---------------------------------------------above reads in / intiailzes the ref image and the background image--------------------------------------------------

	cout << "starting scan this can take a moment..." << endl;
	//this is the main loop for search
	int selectX = 0;
	int selectY = 0;
	double SSDval= 0.0;
	
		
	//creates a vector of the struct comparedresults, to store all results from search
	vector<comparedresults> MatchData;
	
	for (int y = 0; (y + 49) <= 768; y += 24)//moves template down half its height
	{

		
		for (int x = 0; (x + 36) <= 1024; x += 18)//moves template across half its width
		{

			double* TempData = LargeImage->BlockSelect(x, y);//select sample of largeimage to be compared

			Matrix *SampleImage = new Matrix(RefWidth, RefHeight, TempData);//creates new instance of Matrix that will hold 2D matrix of the BlockSelect sample

			SSDval = SSDCal(SampleImage->getMatrix2D(), RefImage->getMatrix2D());//sends selction from large image, and RefImage copy of Wally

			SampleImage->~Matrix();//frees memory
			//stores all results from search
			MatchData.push_back({ SSDval ,x ,y });//pushs current variables into vector
		}
	}
	
	
	cout << "scan complete" << endl;
//-------------------------------------end of searching -----------------------------------------------------------------------
	cout << "Enter interger of matches to view" << endl;
	int UserNum =validate();

//image miniuplation and printing------------------------------------------------------------------------------------------------------------
	sort(MatchData.begin(), MatchData.end()); // Sort imageDetails vector by sim score
	int end = MatchData.size() - 1;
	
	MatchData.resize(UserNum);//resizes MatchData to the amount the user wants to see

	for (int i = 0; i < MatchData.size(); i++) {
		
		comparedresults CurrentMatch = { MatchData[i] };//creates instance of selected MatchData holds in CurrentMatch
		cout << "rank: " << i << " X: " << CurrentMatch.x << "  y: " << CurrentMatch.y << endl;
		
		if( i==0 )
			LargeImage->OutLine(CurrentMatch.x, CurrentMatch.y, 0); //adds the outline at these positions
		else {
			LargeImage->OutLine(CurrentMatch.x, CurrentMatch.y, 50);
		}
			
		
	}
	
	//prints out the LargeImage which has had the matches outlined on it
	TempData = LargeImage->convertTo1D();
	string output = "Output_image.pgm";
	write_pgm(output, TempData, 768, 1024);
	
	


	cout << "best match will have a solid black boarder" << endl;
	cout << "results outputed to file: "<< output << endl;
	cout << "program ended" << endl;
	cin.get();
	cin.get();

}