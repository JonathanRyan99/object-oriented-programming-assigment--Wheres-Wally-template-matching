#pragma once
#include"Matrix.h"
class Large : public Matrix {
public:
	using Matrix::Matrix; //brings in matrix constuctors (for the 2D)
	
	double* BlockSelect(int x, int y);
	void OutLine(int x, int y, int colour);
};