#include "stdafx.h"
#include <iostream>
#include <iomanip>
using namespace std;

class SimpleMatrix {

public:
	int rows;
	int columns;
	int **element;

	SimpleMatrix(int row, int fill) {
		this->init(row, fill);
	}
	//void Copy(SimpleMatrix &mat)
	//{
	//	for (int i = 0; i < 4; i++)
	//		for (int j = 0; j < 4; j++)
	//			this->element[i][j] = mat.get(i, j);
	//}
	//SimpleMatrix& operator=(SimpleMatrix& mat) {
	//	if (&mat != this)
	//	{
	//		//this->~SimpleMatrix();
	//		this->init(4, 0);
	//		for(int i=0;i<4;i++)
	//			for (int j=0;j<4;j++)
	//				this->element[i][j] = mat.get(i, j);
	//	}
	//	return *this;
	//}
	SimpleMatrix(SimpleMatrix &mat)
	{
		this->init(4, 0);
		for (int i=0; i<4; i++)
			for (int j=0; j<4; j++)
				this->element[i][j] = mat.get(i, j);
	}
	~SimpleMatrix() {
		if (this->element != NULL)
		{
			for (int i = 0; i < 4; i++)
				delete[] this->element[i];
			delete [] this->element;
		}

	}

	SimpleMatrix(int row, int values[]) {
		this->init(row);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				this->element[i][j] = values[i * columns + j];
	}
	//设置元素
	void set(int i, int j, int x)
	{
		if (i >= 0 && i < rows && j >= 0 && j < columns)
			this->element[i][j] = x;
		else throw out_of_range("i and j must be range from 0 to rows/columns");
	}
	//获取元素
	int get(int i, int j)
	{
		if (i >= 0 && i < rows && j >= 0 && j < columns)
			return this->element[i][j];
		throw out_of_range("can not find the specify i and j!");
	}
	// 逆时针旋转矩阵90度  
	void rotate1()
	{
		SimpleMatrix temp(4, 0); 
		for(int i=0;i<this->rows;i++)  
			for(int j=0;j<this->columns;j++)
				temp.element[this->rows-1-j][i] = this->element[i][j];
		
		 for (int x = 0; x < this->rows; x++)
            for (int y = 0; y < this->columns; y++)
                this->element[x][y] = temp.get(x, y);
	}
	// 顺时针旋转90度
	void rotate2()
	{  
		SimpleMatrix temp(4, 0);
		//顺时针旋转矩阵90度  
		for(int i=0;i<this->rows;i++)  
			for(int j=0;j<this->columns;j++)
				temp.element[i][this->rows - 1 - j] = this->element[j][i];
		
		 for (int x = 0; x < this->rows; x++)
            for (int y = 0; y < this->columns; y++)
                this->element[x][y] = temp.get(x, y);
	}
	// 顺时针90度
	void rotate3() {
		this->rotate2();
		this->rotate2();
	}
	int getElementCount() {
		int count = 0;
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				if (this->element[i][j] != -1)
					count++;
		return count;
	}

	int getElementCount(int element) {
		int count = 0;
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				if (this->element[i][j] == element)
					count++;
		return count;
	}
	// 获取最大元素
	int getMaxValue() {
		int temp = 0;
		for (int i = 0; i<4; i++) {
			for (int j = 0; j<4; j++) {
				if (this->element[i][j] > temp) {
					temp = this->element[i][j];
				}
			}
		}
		return temp;
	}

	//重载<<符号
	friend ostream& operator<<(ostream& out, SimpleMatrix &mat)
	{
		//printf();
		out << "SimpleMatrix (" << mat.rows << "X" << mat.columns << ") Points(" << mat.getElementCount() << ")" << endl;
		for (int i = 0; i < mat.rows; i++)
		{
			for (int j = 0; j < mat.columns; j++)
			{
				// if (mat.element[i][j] == 0) out << setw(2) << "\033[37m□  \033[0m";
				//printf(" The ......\n");
				// if (mat.element[i][j] == 1) out << setw(2) << "\033[32m■  \033[0m";
				out << setw(4) << mat.element[i][j] << " ";
			}

			out << endl;
		}
		return out;
	}

private:
	void init(int row = 8, int fill = 0) {//8*8
		this->rows = this->columns = row;
		this->element = new int*[row];
		for (int i = 0; i<row; i++) this->element[i] = new int[row];

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				this->element[i][j] = fill;
	}
};
