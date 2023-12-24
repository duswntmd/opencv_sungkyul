#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef unsigned char uchar;   // #define uchar unsigned char 라고 해도 같은 뜻임.

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

int main(int argc, char* argv[])
{
    int ROW, COL, flag, i, j;
    uchar** inImg, ** outImg;
    char name[50] = "Fileter";



    if (argc != 5)
    {
        printf("\n Usage : %s inImg COL ROW outImage flag(0:gaussian 1:average \n", argv[0]);
        exit(1);
    }
    ROW = atoi(argv[3]);
    COL = atoi(argv[2]);
    flag = atoi(argv[4]);

    gaussMask = d_alloc(block_size, block_size);
    aveMask = d_alloc(block_size, block_size);

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

    inImg = uc_alloc(COL, ROW);
    outImg = uc_alloc(COL, ROW);

    read_ucmatrix(COL, ROW, inImg, argv[1]);

    printf("Start Simple Filtering \n");

    if (flag == 0) {
        convolution(gaussMask, block_size, COL, ROW, inImg, outImg);
        strcat(name, "gaussian");
    }
    else if (flag == 1) {
        convolution(aveMask, block_size, COL, ROW, inImg, outImg);
        strcat(name, "average");
    }
    else if (flag == 2) {
        printf("Start Simple Filtering \n");
        median(inImg, outImg, COL, ROW, mode, filtersize);
        printf("Start Simple Filtering \n");
        strcat(name, "median");
        printf("Start Simple Filtering \n");
    }
    else
    {
        printf("flag must be 0 or 1 \n");
        exit(1);
    }

    char new_filename[100];
    sprintf(new_filename, "%s_%s", argv[1], name);

    write_ucmatrix(COL, ROW, outImg, new_filename);

    uc_free(COL, ROW, inImg);
    uc_free(COL, ROW, outImg);

    return 0;

}

