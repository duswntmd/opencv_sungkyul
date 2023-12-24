#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>


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

void read_dmatrix(int size_x, int size_y, double** ucmatrix, const char* filename)
{
    int i;
    FILE* f;

    if ((f = fopen(filename, "rb")) == NULL)
    {
        printf("%s File open Error!\n", filename);
        exit(0);
    }
    for (i = 0; i < size_y; i++)
        if (fread(ucmatrix[i], sizeof(double), size_x, f) != size_x)
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

void write_dmatrix(int size_x, int size_y, double** d_alloc, const char* filename)
{
    int i;
    FILE* f;

    if ((f = fopen(filename, "wb")) == NULL)
    {
        printf("%s File open Error! 파일을 찾아보세요\n", filename);
        exit(0);
    }
    for (i = 0; i < size_y; i++)
        if (fwrite(d_alloc[i], sizeof(double), size_x, f) != size_x)
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

void convolution(double** h, int F_length, int size_x, int size_y, uchar** image1, uchar** image2)
{
    int i, j, x, y;
    int margin, indexX, indexY;
    double sum, coeff;

    margin = (int)(F_length / 2);

    for (i = 0; i < size_y; i++) {
        for (j = 0; j < size_x; j++) {
            sum = 0;
            for (y = 0; y < F_length; y++) {
                indexY = i - margin + y;
                if (indexY < 0) {
                    indexY = -indexY;
                }
                else if (indexY >= size_y) {
                    indexY = (2 * size_y - indexY - 1);
                }

                for (x = 0; x < F_length; x++) {
                    indexX = j - margin + x;
                    if (indexX < 0) indexX = -indexX;
                    else if (indexX >= size_x) indexX = (2 * size_x - indexX - 1);

                    sum += h[y][x] * (double)image1[indexY][indexX];
                }
            }

            // sum += 128;
            if (sum < 0) sum = 0.;
            else if (sum > 255) sum = 255.;
            image2[i][j] = (uchar)sum;
        }
    }
}

void Bubble_sort(uchar* Sort, uchar* median_value, int Mode, int filterSize)
{
    int i, x;
    uchar temp, swap;

    for (x = 0; x < filterSize * filterSize; x++) {
        temp = Sort[x];
        for (i = x; i < filterSize * filterSize - 1; i++) {
            if (temp >= Sort[i + 1]) {
                swap = temp;
                temp = Sort[i + 1];
                Sort[i + 1] = swap;
            }
        }

        Sort[x] = temp;
    }
    if (Mode == -1) {
        *median_value = (uchar)Sort[0];
    }
    else if (Mode == 0) {
        *median_value = (uchar)Sort[filterSize * filterSize / 2];
    }
    else if (Mode == 1) {
        *median_value = (uchar)Sort[filterSize * filterSize - 1];
    }
}

void median(uchar** img, uchar** result, int Col, int Row, int Mode, int filterSize)
{
    int i, j, x, y, z, count = 0;
    uchar median_value;
    uchar* Sort;

    Sort = (uchar*)malloc(filterSize * filterSize * sizeof(uchar));
    if (Sort == NULL) {
        printf("Sort array allocation error\n");
        exit(0);
    }

    for (i = 0; i < Col; i++) {
        for (j = 0; j < Col; j++) {
            result[i][j] = img[i][j];
        }
    }

    for (i = 0; i < Row - filterSize; i++) {
        for (j = 0; j < Col - filterSize; j++) {

            for (x = 0; x < filterSize; x++) {
                for (y = 0; y < filterSize; y++) {
                    Sort[filterSize * x + y] = img[i + x][j + y];
                }
            }

            Bubble_sort(Sort, &median_value, Mode, filterSize);
            result[i + 1][j + 1] = median_value;
        }
    }
}



void make_Mask(int mask_size, double** Mask, int checkMask)
{
    int i, j;
    double sobelYMask[3][3] = { 1,0,-1,2,0,-2,1,0,-1 };
    double sobelXMask[3][3] = { -1,-2,-1,0,0,0,1,2,1 };
    double PrewittYMask[3][3] = { 1,0,-1,1,0,-1,1,0,-1 };
    double PrewittXMask[3][3] = { -1,-1,-1,0,0,0,1,1,1 };
    double RobertsYMask[3][3] = { 0,0,-1,0,1,0,0,0,0 };
    double RobertsXMask[3][3] = { -1,0,0,0,1,0,0,0,0 };
    double Laplace1Mask[3][3] = { 0,-1,0,-1,4,-1,0,-1,0 };
    double Laplace2Mask[3][3] = { -1,-1,-1,-1,8,-1,-1,-1,-1 };
    double Laplace3Mask[3][3] = { 0,-1,0,-1,5,-1,0,-1,-0 };
    double Laplace4Mask[3][3] = { -1,-1,-1,-1,9,-1,-1,-1,-1 };
    double embossing[3][3] = { 0,0,-1,0,0,0,1,0,0 };
    switch(checkMask)
    {
    case 0 :
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = sobelYMask[i][j];
        break;
    case 1:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = sobelXMask[i][j];
        break;
    case 2:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = sobelYMask[i][j] + sobelXMask[i][j];
        break;
    case 3:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = PrewittYMask[i][j];
        break;
    case 4:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = PrewittXMask[i][j];
        break;
    case 6:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = RobertsYMask[i][j];
        break;
    case 7:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = RobertsXMask[i][j];
        break;
    case 8:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = RobertsYMask[i][j] + RobertsXMask[i][j];
        break;
    case 9:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = Laplace1Mask[i][j];
        break;
    case 10:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = Laplace2Mask[i][j];
        break;
    case 11:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = Laplace3Mask[i][j];
        break;
    case 12:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = Laplace4Mask[i][j];
        break;
    case 13:
        for (i = 0; i < mask_size; i++)
            for (j = 0; j < mask_size; j++)
                Mask[i][j] = embossing[i][j];
        break;

    default :
        printf("Mask Number is wrong \n");
        exit(1);
    }
}

void convolutionD(double** h, int F_length, int size_x, int size_y, uchar** image1, double** image2)
{
    int i, j, x, y;
    int margin, indexX, indexY;
    double sum, coeff;

    margin = (int)(F_length / 2);

    for (i = 0; i < size_y; i++) {
        for (j = 0; j < size_x; j++) {
            sum = 0;
            for (y = 0; y < F_length; y++) {
                indexY = i - margin + y;
                if (indexY < 0) 
                    indexY = -indexY;
                else if (indexY >= size_y) indexY = (2 * size_y - indexY - 1);

                for (x = 0; x < F_length; x++) {
                    indexX = j - margin + x;
                    if (indexX < 0) 
                    indexX = -indexX;
                    else if (indexX >= size_x) indexX = (2 * size_x - indexX - 1);

                    sum += h[y][x] * (double)image1[indexY][indexX];
                }
            }
            if (sum < 0)
            {
                sum == 0;
            }

            if (sum > 255)
            {
                sum == 255;
            }
            //sum += 10;
            sum -= 40;
            image2[i][j] = sum;
        }
    }
}

int main(int argc, char* argv[])
{
    int ROW, COL, flag, i, j;
    uchar** inImg, ** outImg;
    double** doutimg;
    double** maskimg;
    double** dinImg;
    int checkimg0 = 0;
    int checkimg1 = 1;
    int checkimg2 = 2;
    int checkimg6 = 6;
    int checkimg7 = 7;
    int checkimg8 = 8;
    int checkimg9 = 9;
    int checkimg10 = 10;
    int checkimg13 = 13;
    char name[50] = "Fileter";
    


    if (argc != 5)
    {
        printf("\n Usage : %s inImg COL ROW outImage flag(0:gaussian 1:average \n", argv[0]);
        exit(1);
    }
    ROW = atoi(argv[3]);
    COL = atoi(argv[2]);
    flag = atoi(argv[4]);
   //checkimg = atoi(argv[5]);

    gaussMask = d_alloc(block_size, block_size);
    aveMask = d_alloc(block_size, block_size);
    maskimg = d_alloc(block_size, block_size);
    //sobelYMask = d_alloc(block_size, block_size);

    gaussMask[0][0] = 1 / 16.;
    gaussMask[0][1] = 2 / 16.;
    gaussMask[0][2] = 1 / 16.;
    gaussMask[1][0] = 2 / 16.;
    gaussMask[1][1] = 4 / 16.;
    gaussMask[1][2] = 2 / 16.;
    gaussMask[2][0] = 1 / 16.;
    gaussMask[2][1] = 2 / 16.;
    gaussMask[2][2] = 1 / 16.;


    aveMask[0][0] = 1 / 9.;
    aveMask[0][1] = 1 / 9.;
    aveMask[0][2] = 1 / 9.;
    aveMask[1][0] = 1 / 9.;
    aveMask[1][1] = 1 / 9.;
    aveMask[1][2] = 1 / 9.;
    aveMask[2][0] = 1 / 9.;
    aveMask[2][1] = 1 / 9.;
    aveMask[2][2] = 1 / 9.;

    /*embosMask[0][0] = -1;
    embosMask[2][2] = 1;*/

    inImg = uc_alloc(COL, ROW);
    outImg = uc_alloc(COL, ROW);

    dinImg = d_alloc(COL, ROW);
    doutimg = d_alloc(COL, ROW);
    //maskimg = d_alloc(COL, ROW);

    read_ucmatrix(COL, ROW, inImg, argv[1]);
    //read_dmatrix(COL, ROW, dinImg, argv[1]);

    printf("Start Simple Filtering \n");

    if (flag == 0) {
        convolution(gaussMask, block_size, COL, ROW, inImg, outImg);
        strcat(name, "gaussian");
        printf("0 \n");
    }
    else if (flag == 1) {
        convolution(aveMask, block_size, COL, ROW, inImg, outImg);
        strcat(name, "average");
        printf("1 \n");
    }
    else if (flag == 2) {
        median(inImg, outImg, COL, ROW, mode, filtersize);
        strcat(name, "median");
        printf("2 \n");
    }
    else if (flag == 3) {
        make_Mask(mask_size, maskimg, checkimg0);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening1");
        printf("3 \n");
    }
    else if (flag == 4) {
        make_Mask(mask_size, maskimg, checkimg1);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening2");
        printf("4 \n");
    }
    else if (flag == 5) {
        make_Mask(mask_size, maskimg, checkimg2);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening3");
        printf("5 \n");
    }
    else if (flag == 6) {
        make_Mask(mask_size, maskimg, checkimg6);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening6");
        printf("6 \n");
    }
    else if (flag == 7) {
        make_Mask(mask_size, maskimg, checkimg7);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening7");
        printf("7 \n");
    }
    else if (flag == 8) {
        make_Mask(mask_size, maskimg, checkimg8);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening8");
        printf("8 \n");
    }
    else if (flag == 9) {
        make_Mask(mask_size, maskimg, checkimg9);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening9");
        printf("9 \n");
    }
    else if (flag == 10) {
        make_Mask(mask_size, maskimg, checkimg9);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening10");
        printf("10 \n");
    }
    else if (flag == 10) {
        make_Mask(mask_size, maskimg, checkimg13);
        printf("Before convolutionD\n");
        convolutionD(maskimg, mask_size, COL, ROW, inImg, doutimg);
        printf("After convolutionD\n");
        strcat(name, "sharpening13");
        printf("10 \n");
    }
    else
    {
        printf("flag must be 0 or 1 \n");
        exit(1);
    }

    
    //convolution(embosMask, embos_size, COL, ROW, inImg, outImg);

    char new_filename[100];
    sprintf(new_filename, "%s_%s", argv[1], name);

    //write_ucmatrix(COL, ROW, outImg, new_filename);
    write_dmatrix(COL, ROW, doutimg, new_filename);

    uc_free(COL, ROW, inImg);
    uc_free(COL, ROW, outImg);

    return 0;

}

