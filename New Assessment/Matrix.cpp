#include <iostream>
#include "Matrix.h"
using namespace std;

Matrix::Matrix() {} // Matrix constructor

//this creates the instance as 2d matrix 
Matrix::Matrix(int column, int row, double* data) // Matrix overloaded constructor
{
	storedRow = row;
	storedCol = column;
	array1d = data;
	
	
	array2d = new double*[row];
	for (int i = 0; i < row; i++)
	{
		array2d[i] = new double[column];
	}
	
	for (int a = 0; a < storedRow; a++) // Loop rows
	{
		for (int b = 0; b < storedCol; b++) // Loop columns
		{
			array2d[a][b] = data[a*storedCol + b]; // Move values from input into 2D array, a*storedCol finds the row, +b finds the position within that row
		}
	}
}

//function of matrix
double* Matrix::convertTo1D() // Takes the 2D array and returns it in the form of a 1D array
{
	double* arr1d = new double[storedCol*storedRow];
	int count = 0;
	for (int a = 0; a < storedRow; a++)
	{
		for (int b = 0; b < storedCol; b++)
		{
			arr1d[count] = this->array2d[a][b];//2D array is stored in instance of class
			count++;
		}
	}
	return arr1d;//array is returned
}

double** Matrix::getMatrix2D() {//returns 2D matrix
	return array2d;
}



Matrix::~Matrix() //Matrix destructor used to free the memory from holding image data
{
	for (int a = 0; a < storedRow; ++a)   //deletes columns of 2D array,
	{
		delete[] array2d[a];
	}
	delete[] array2d;  // Delete the 2D array
}
