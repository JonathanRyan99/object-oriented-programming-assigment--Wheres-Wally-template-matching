
#pragma once
class Matrix
{
public:
	Matrix();    // Matrix constructor
	Matrix(int column, int row, double* input_Data);    // Matrix overloaded constructor //this creates a matrix instance that is 2D array
	~Matrix();    // Matrix destructor
	
	double* convertTo1D(); // Takes the object 2D array and returns it in the form of a 1D array
	double** getMatrix2D();

protected:
	double** array2d; //Two double pointers to 2D array
	double* array1d;  //Double pointer at 1D array
	int storedRow;
	int storedCol;
};
