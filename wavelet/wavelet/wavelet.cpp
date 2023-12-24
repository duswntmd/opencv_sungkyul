#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


typedef unsigned char uchar;   // #define uchar unsigned char 라고 해도 같은 뜻임.

double** sobelYMask, ** sobelXMask, ** PrewitMask, ** RobertsMask, ** Laplace1Mask, ** Laplace2Mask, ** embossing;
int mask_size = 3;
double** embosMask;
int embos_size = 3;
int F_length = 3;
double** gaussMask, ** aveMask;
int block_size = 3;
int mode = 0;
int filtersize = 3;
int ROW, COL;

using namespace std;
using namespace cv;

uchar** uc_alloc(int size_x, int size_y)  //uchar을 double로 바꾸는 날이 온다.
{
	uchar** m;
	int i;

	if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL)
	{
		printf("d_alloc error 1\7\n");
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL)
		{
			printf("d_alloc error 2\7\n");
			exit(0);
		}
	return m;
}

double** d_alloc(int size_x, int size_y)  //uchar을 double로 바꾸는 날이 온다.
{
	double** m;
	int i;

	if ((m = (double**)calloc(size_y, sizeof(double*))) == NULL)
	{
		printf("d_alloc error 1\7\n");
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if ((m[i] = (double*)calloc(size_x, sizeof(double))) == NULL)
		{
			printf("d_alloc error 2\7\n");
			exit(0);
		}
	return m;
}

void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i;
	FILE* f;

	if ((f = fopen(filename, "rb")) == NULL)
	{
		printf("%s File open Error!\n", filename);
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Read Error!\n");
			exit(0);
		}
	fclose(f);
}

void read_dmatrix(int size_x, int size_y, double** d_alloc, const char* filename)
{
	int i;
	FILE* f;

	if ((f = fopen(filename, "rb")) == NULL)
	{
		printf("%s File open Error!\n", filename);
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if (fread(d_alloc[i], sizeof(double), size_x, f) != size_x)
		{
			printf("Data Read Error!\n");
			exit(0);
		}
	fclose(f);
}

void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
{
	int i;
	FILE* f;

	if ((f = fopen(filename, "wb")) == NULL)
	{
		printf("%s File open Error! 파일을 찾아보세요\n", filename);
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
		{
			printf("Data Write Error!\n");
			exit(0);
		}
	fclose(f);
}

void write_dmatrix(int size_x, int size_y, double** dmatrix, const char* filename)
{
	int i;
	FILE* f;

	if ((f = fopen(filename, "wb")) == NULL)
	{
		printf("%s File open Error! 파일을 찾아보세요\n", filename);
		exit(0);
	}
	for (i = 0; i < size_y; i++)
		if (fwrite(dmatrix[i], sizeof(double), size_x, f) != size_x)
		{
			printf("Data Write Error!\n");
			exit(0);
		}
	fclose(f);
}

void uc_free(int size_x, int size_y, uchar** ucmatrix)
{
	int i;
	for (i = 0; i < size_y; i++)
		free(ucmatrix[i]);
	free(ucmatrix);
}

void d_free(int size_x, int size_y, double** dmatrix)
{
	int i;
	for (i = 0; i < size_y; i++)
		free(dmatrix[i]);
	free(dmatrix);
}

// Anotonini
double A97L1[9] = {
	0.026749,
	-0.016864,
	-0.078223,
	0.266864,
	0.602949,
	0.266864,
	-0.078223,
	-0.016864,
	0.026749};
int A97L_1 = 9;
double A97H1[11] = {
	0.0,
	0.0,
	-0.045636,
	0.028772,
	0.295636,
	-0.557543,
	0.295636,
	0.028772,
	-0.045636,
	0.0,
	0.0 };
int A97H1_1 = 11;
double A97L2[11] = {
	0.0,
	0.0,
	-0.045636,
	-0.028772,
	0.295636,
	0.557543,
	0.295636,
	-0.028772,
	-0.045636,
	0.0,
	0.0};
int A97L22_1 = 11;
//Antonini Filter
double A97H2[9] = {
	-0.026749,
	-0.016864,
	0.078223,
	0.266864,
	-0.608949,
	0.266864,
	0.078223,
	-0.016864,
	-0.026749};
int A97H2_1 = 9;

void row_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2)
{
	int i, j, k, margin, index, size_x2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x / 2;
	temp = d_alloc(size_x, size_y);

	for (i = 0, coeff = 0.; i < F_length; i++) coeff += h[i];
	printf("If\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_y; i++) {
		for (j = 0; j < size_x; j++) {
			for (k = 0, sum = 0.; k < F_length; k++) {
				index = j - margin + k;
				if (index < 0)
					index = -index;
				else if (index >= size_x)
					index = (2 * size_x - index - 2);
				sum += h[k] * image1[i][index];
			}
			if (coeff > 1.) sum /= coeff;
			if (sum < 0) sum = 0.;
			else if (sum > 255) sum = 255.;
			temp[i][j] = sum;
		}
	}

	for (i = 0; i < size_y; i++) 
		for (j = 0, k = 0; j < size_x; j += 2, k++) 
			image2[i][k] = temp[i][j];

			d_free(size_x, size_y, temp);
}

void column_analysis(double* h, int F_length, int size_x, int size_y, double** image1, double** image2)
{
	int i, j, k, margin, index, size_x2, size_y2;
	double sum, coeff;
	double** temp;

	size_x2 = size_x / 2;
	size_y2 = size_y / 2;
	temp = d_alloc(size_x2, size_y2);

	for (i = 0, coeff = 0.; i < F_length; i++) coeff += h[i];
	printf("%If\n", coeff);

	margin = (int)(F_length / 2);
	for (i = 0; i < size_x2; i++) {
		for (j = 0; j < size_y2; j++) {
			for (k = 0, sum = 0.; k < F_length; k++) {
				index = j - margin + k;
				if (index < 0)
					index = -index;
				else if (index >= size_y)
					index = (2 * size_y - index - 2);
				sum += h[k] * image1[i][index];
			}
			if (coeff > 1.) sum /= coeff;
			if (sum < 0) sum = 0.;
			else if (sum > 255) sum = 255.;
			temp[i][j] = sum;
			
		}
	}
	for (i = 0; i < size_x2; i++)
		for (j = 0, k = 0; j < size_y; j += 2, k++)
			image2[k][i] = temp[j][i];

	d_free(size_x2, size_y, temp);
}

void conv_uc_to_d(int size_x, int size_y, uchar** ucmatrix, double** dmatrix) {
	for (int i = 0; i < size_y; ++i) {
		for (int j = 0; j < size_x; ++j) {
			dmatrix[i][j] = static_cast<double>(ucmatrix[i][j]);
		}
	}
}

void conv_d_to_ucabs(int size_x, int size_y, double** dmatrix, uchar** ucmatrix) {
	for (int i = 0; i < size_y; ++i) {
		for (int j = 0; j < size_x; ++j) {
			double value = fabs(dmatrix[i][j]);
			ucmatrix[i][j] = static_cast<uchar>(value > 255.0 ? 255 : value);
		}
	}
}

void analysis(int argc, char* argv[], char* filename, int size_x, int size_y, int level)
{
	int size_x2, size_y2;
	int i, j;
	uchar** image1;
	double** Image1, ** Image2, ** Image3;
	FILE* f1;
	char* make_fname;
	size_x2 = size_x / 2;
	size_y2 = size_y / 2;
	Image1 = d_alloc(size_x, size_y);
	Image2 = d_alloc(size_x2, size_y);
	Image3 = d_alloc(size_x2, size_y2);
	image1 = uc_alloc(size_x2, size_y2);

	read_dmatrix(size_x, size_y, Image1, (argv[1], 'c', level - 1, 0)); //make_fname
	row_analysis(A97L1, A97L_1, size_x, size_y, Image1, Image2);
	column_analysis(A97L1, A97L_1, size_x, size_y, Image2, Image3);
	write_dmatrix(size_x2, size_y2, Image3, (argv[1], 'c', level, 0));
	conv_d_to_ucabs(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, (argv[1], 'a', level, 0));
	column_analysis(A97H1, A97H1_1, size_x, size_y, Image2, Image3);
	/*write_dmatrix(size_x2, size_y2, Image3, (argv[1], 'd', level, 1));
	conv_d_to_ucabs(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, (argv[1], 'w', level, 1));
	row_analysis(A97H1, A97H1_1, size_x, size_y, Image1, Image2);
	column_analysis(A97L1, A97L_1, size_x, size_y, Image2, Image3);
	write_dmatrix(size_x2, size_y2, Image3, (argv[1], 'd', level, 2));
	conv_d_to_ucabs(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, (argv[1], 'w', level, 2));
	column_analysis(A97H1, A97H1_1, size_x, size_y, Image2, Image3);
	write_dmatrix(size_x2, size_y2, Image3, (argv[1], 'd', level, 3));
	conv_d_to_ucabs(size_x2, size_y2, Image3, image1);
	write_ucmatrix(size_x2, size_y2, image1, (argv[1], 'w', level, 3));*/

	uc_free(size_x2, size_y2, image1);
	d_free(size_x, size_y, Image1);
	d_free(size_x2, size_y, Image2);
	d_free(size_x2, size_y2, Image3);
}

void Analysis(int argc, char* argv[], char* filename, int size_x, int size_y, int level)
{
	int i, size_x2, size_y2;
	uchar** image1;
	double** Image1;

	size_x2 = size_x;
	size_y2 = size_y;

	image1 = uc_alloc(size_x, size_y);
	Image1 = d_alloc(size_x, size_y);

	read_ucmatrix(size_x, size_y, image1, argv[1]);
	conv_uc_to_d(size_x, size_y, image1, Image1);

	write_dmatrix(size_x, size_y, Image1,(argv[1],'c',0,0)); //make_fname
	uc_free(size_x, size_y, image1);
	d_free(size_x, size_y, Image1);

	for (i = 1; i <= level; i++) {
		analysis(argc, argv, filename, size_x2, size_y2, i);
		size_x2 /= 2;
		size_y2 /= 2;
	}
	printf("\nAnalysis is end.\nPress enter key to continue.\n");
	getchar();

}

int main(int argc, char* argv[])
{
	int ROW, COL, flag, i, j, Mode;
	double max = -10E30, min = 10E30;
	unsigned char** input_img, ** output_img, ** output2_img;
	FILE* fp1;
	double** fourier_img, ** imaginary_img;
	uchar** inImg, ** outImg;
	double** Butterworth;
	double** doutimg;
	double** maskimg;
	double** dinImg;
	double** doutput_img;
	int size_x, size_y, level;
	char* filename;

	char name[50] = "Fileter";
	char dname[50] = "dFileter";


	if (argc != 5)
	{
		printf("\n Usage : %s inImg COL ROW outImage flag(0:gaussian 1:average \n", argv[0]);
		exit(1);
	}
	//ROW = atoi(argv[3]);
	//COL = atoi(argv[2]);
	//flag = atoi(argv[4]);
	//Mode = atoi(argv[4]);
	//checkimg = atoi(argv[5]);
	size_x = atoi(argv[2]);
	size_y = atoi(argv[3]);
	level = atoi(argv[4]);



	maskimg = d_alloc(block_size, block_size);
	//sobelYMask = d_alloc(block_size, block_size);

	input_img = (uchar**)uc_alloc(COL, ROW);
	output_img = (uchar**)uc_alloc(COL, ROW);  //요거
	output2_img = (uchar**)uc_alloc(COL, ROW);
	doutput_img = (double**)d_alloc(COL, ROW);
	fourier_img = (double**)d_alloc(COL, ROW);
	imaginary_img = (double**)d_alloc(COL, ROW);
	Butterworth = (double**)d_alloc(COL, ROW);

	inImg = uc_alloc(COL, ROW);
	outImg = uc_alloc(COL, ROW);

	dinImg = d_alloc(COL, ROW);
	doutimg = d_alloc(COL, ROW);
	//maskimg = d_alloc(COL, ROW);

	read_ucmatrix(COL, ROW, input_img, argv[1]);
	//read_dmatrix(COL, ROW, dinImg, argv[1]);

	printf("Start Simple Filtering \n");

	Analysis(argc, argv, filename, size_x, size_y, level);



	char new_filename[100];
	char new_dfilename[100];

	sprintf(new_filename, "%s_%s", argv[1], name);
	write_ucmatrix(COL, ROW, output_img, new_filename);
	sprintf(new_dfilename, "%s_%s", argv[1], dname);
	write_dmatrix(COL, ROW, fourier_img, new_dfilename);

	//write_dmatrix(COL, ROW, doutimg, new_filename);

	uc_free(COL, ROW, inImg);
	uc_free(COL, ROW, outImg);

	return 0;

}