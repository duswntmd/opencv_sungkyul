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

int rearrange(double* X, int N)
{
	int i, j, * power_of_2, * pos, stage, num_of_stages = 0;
	double temp;

	for (i = N; i > 1; i >>= 1, num_of_stages++);
	if ((power_of_2 = (int*)malloc(sizeof(int) * N)) == NULL)
		return -1;
	if ((pos = (int*)malloc(sizeof(int) * N)) == NULL)
		return -1;

	power_of_2[0] = 1;
	for (stage = 1; stage < num_of_stages; stage++)
		power_of_2[stage] = power_of_2[stage - 1] << 1;

	for (i = 1; i < N - 1; i++)
		pos[i] = 0;
	for (i = 1; i < N - 1; i++)
	{
		if (!pos[i])
		{
			for (j = 0; j < num_of_stages; j++)
			{
				if (i & power_of_2[j])
					pos[i] += power_of_2[num_of_stages - 1 - j];
			}
			temp = X[i];
			X[i] = X[pos[i]];
			X[pos[i]] = temp;
			pos[pos[i]] = 1;
		}
	}
	free(power_of_2);
	free(pos);
	return 0;
}

void fft(double* X_re, double* X_im, int N)
{
	double X_temp_re, X_temp_im;
	double phase;
	int num_of_stages = 0, num_of_elements, num_of_sections, size_of_butterfly;
	int i, j, stage, m1, m2;
	for (i = N; i > 1; i >>= 1, num_of_stages++);
	num_of_elements = N;
	num_of_sections = 1;
	size_of_butterfly = N >> 1;
	for (stage = 0; stage < num_of_stages; stage++) {
		m1 = 0;
		m2 = size_of_butterfly;
		for (i = 0; i < num_of_sections; i++) {
			for (j = 0; j < size_of_butterfly; j++, m1++, m2++)
			{
				X_temp_re = X_re[m1] - X_re[m2];
				X_temp_im = X_im[m1] - X_im[m2];
				X_re[m1] = X_re[m1] + X_re[m2];
				X_im[m1] = X_im[m1] + X_im[m2];
				phase = -2.0 * M_PI * j / num_of_elements;
				X_re[m2] = X_temp_re * cos(phase) - X_temp_im * sin(phase);
				X_im[m2] = X_temp_re * sin(phase) + X_temp_im * cos(phase);

			}
			m1 += size_of_butterfly;
			m2 += size_of_butterfly;
		}
		num_of_elements >>= 1;
		num_of_sections <<= 1;
		size_of_butterfly >>= 1;
	}
	rearrange(X_re, N);
	rearrange(X_im, N);
}

int fft_2d(double** X_re, double** X_im, int N, int Mode)
{
	int i, j;
	double* temp_re, * temp_im;

	if ((temp_re = (double*)malloc(sizeof(double) * N)) == NULL)
		return -1;
	if ((temp_im = (double*)malloc(sizeof(double) * N)) == NULL)
		return -1;

	if (Mode == 0)
	{
		for (i = 0; i < N; i++) {
			fft(X_re[i], X_im[i], N);
		}

		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j];
				temp_im[i] = X_im[i][j];
			}
			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;
			}
		}
	}
	else if (Mode == 1)
	{
		for (i = 0; i < N; i++) {
			fft(X_re[i], X_im[i], N);
		}

		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j];
				temp_im[i] = X_im[i][j];
			}
			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;
			}
		}
	}
	else if (Mode == 2)
	{
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				temp_re[j] = X_re[i][j] * pow(-1, j);
				temp_im[j] = X_im[i][j] * pow(-1, j);
			}
			fft(temp_re, temp_im, N);

			for (j = 0; j < N; j++) {
				X_re[i][j] = temp_re[j];
				X_im[i][j] = temp_im[j];
			}

		}
		for (j = 0; j < N; j++) {
			for (i = 0; i < N; i++) {
				temp_re[i] = X_re[i][j] * pow(-1, i);
				temp_im[i] = X_im[i][j] * pow(-1, i);
			}
			fft(temp_re, temp_im, N);
			for (i = 0; i < N; i++) {
				X_re[i][j] = temp_re[i] / N;
				X_im[i][j] = temp_im[i] / N;
			}
		}
	}
	free(temp_re);
	free(temp_im);
	return 0;
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

	char name[50] = "Fileter";
	char dname[50] = "dFileter";


	if (argc != 5)
	{
		printf("\n Usage : %s inImg COL ROW outImage flag(0:gaussian 1:average \n", argv[0]);
		exit(1);
	}
	ROW = atoi(argv[3]);
	COL = atoi(argv[2]);
	//flag = atoi(argv[4]);
	Mode = atoi(argv[4]);
	//checkimg = atoi(argv[5]);


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

	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
		{
			fourier_img[i][j] = (double)input_img[i][j];
			imaginary_img[i][j] = 0.;
		}
	if (Mode == 0) {
		fft_2d(fourier_img, imaginary_img, ROW, Mode);
		strcat(name, "cmanbutterfft0");
	}
	else if (Mode == 1) {
		fft_2d(fourier_img, imaginary_img, ROW, Mode);
		strcat(name, "cmanbutterfft1");
	}
	else if (Mode == 2) {
		fft_2d(fourier_img, imaginary_img, ROW, Mode);
		//strcat(name, "lenabutterfft2");
		//strcat(dname, "lenabutterfft2");
		strcat(name, "lenahighbutterfft1");
		strcat(dname, "lenahighbutterfft1");

		int n = 2;
		int D0 = 10;
		double diameter;

		for (i = 0; i < ROW; i++)
			for (j = 0; j < COL; j++)
			{
				diameter = sqrt((ROW / 2. - i) * (ROW / 2. - i) + (COL / 2. - j) * (COL / 2. - j));
				//Butterworth[i][j] = 1 / (1 + pow(diameter / D0, 2 * n)); //low
				Butterworth[i][j] = 1 - 1 / (1 + pow(diameter / D0, 2 * n)); //high

				fourier_img[i][j] *= Butterworth[i][j];
				imaginary_img[i][j] *= -Butterworth[i][j];

			}


		fft_2d(fourier_img, imaginary_img, ROW, Mode - 1);
		//strcat(name, "cmanfft2");

		for (i = 0; i < ROW; i++)
			for (j = 0; j < COL; j++)
			{
				fourier_img[i][j] = fourier_img[i][j] * pow(-1, i + j);
			}
	}
	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
		{
			if (max < fourier_img[i][j]) max = fourier_img[i][j];
			if (min > fourier_img[i][j]) min = fourier_img[i][j];
			
		}
	
	printf("Fourier Max Min value = %If %If\n", max, min);

	for (i = 0; i < ROW; i++)
		for (j = 0; j < COL; j++)
		{
			fourier_img[i][j] -= min;
		}

	if (Mode == 2)
	{
		for (i = 0; i < ROW; i++)
			for (j = 0; j < COL; j++)
			{
				if (fourier_img[i][j] > 255) fourier_img[i][j] = 255;
				else if (fourier_img[i][j] < 0) fourier_img[i][j] = 0;

				output_img[i][j] = (double)fourier_img[i][j];
			}
	}
	
	

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