#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "histogramspecification.h"

typedef unsigned char uchar;

using namespace std;
using namespace cv;

int histogram[256], cdfOfHisto[256], histogramEqual[256], histogramSpec[256], histogramMatch[256];

uchar** uc_alloc(int size_x, int size_y)
{
    uchar** m;
    int i;

    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL)
    {
        printf("uc_alloc error 1\n");
        exit(0);
    }
    for (i = 0; i < size_y; i++)
        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL)
        {
            printf("uc_alloc error 2\n");
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

void get_hist1(uchar** img, int X_size, int Y_size)
{
    int i, j, tmp;
    double tmp1;
    int t, tp, range, hrange;
    CvSize histoSize, cdfSize;
    IplImage* imgHisto, * cdflmgHisto;

    histoSize.width = 256;
    histoSize.height = 256;

    cdfSize.width = 256;
    cdfSize.height = 256;

    imgHisto = cvCreateImage(histoSize, 8, 1);
    cdflmgHisto = cvCreateImage(cdfSize, 8, 1);

    for (i = 0; i < histoSize.height; i++)
        for (j = 0; j < histoSize.width; j++)
        {
            ((uchar*)(imgHisto->imageData + imgHisto->widthStep * i))[j] = 0;
        }
    for (i = 0; i < cdfSize.height; i++)
        for (j = 0; j < cdfSize.width; j++)
        {
            ((uchar*)(cdflmgHisto->imageData + cdflmgHisto->widthStep * i))[j] = 0;
        }

    tp = X_size * Y_size;

    for (i = 0; i < 256; i++)
        histogram[i] = 0;

    for (i = 0; i < Y_size; i++)
        for (j = 0; j < X_size; j++)
            histogram[img[i][j]]++;

    tmp1 = 0;
    for (i = 0; i < 256; ++i)
    {
        tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
    }

    for (i = 0; i < 256; ++i)
    {
        tmp = (int)255 * (histogram[i] / tmp1);

        cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), cvScalar(255, 255, 255), 1, 8, 0);
    }

    cvShowImage("Histo Equal", imgHisto);
    //cvShowImage("Histo match", histogramSpec);

    cdfOfHisto[0] = histogram[0];
    for (i = 0; i < 256; i++)
    {
        cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
    }

    tmp1 = (double)cdfOfHisto[255];
    for (i = 0; i < 256; ++i)
    {
        tmp = (int)255 * (cdfOfHisto[i] / tmp1);
        cvLine(cdflmgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), cvScalar(255, 255, 255), 1, 8, 0);
    }
    cvShowImage("CDF of Histogram", cdflmgHisto);
    //cvShowImage("match CDF of Histogram", histogramSpec);

    range = cdfOfHisto[255] - cdfOfHisto[0];
    histogramEqual[0] = 0;
    for (i = 1; i < 256; ++i) {
        t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
        histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
    }

    cvReleaseImage(&imgHisto);
    cvReleaseImage(&cdflmgHisto);
    for (i = 0; i < Y_size; ++i)
        for (j = 0; j < X_size; ++j)
        {
            img[i][j] = histogramEqual[img[i][j]];
        }
}

void get_hist(uchar** img, int X_size, int Y_size)
{
    int i, j, tmp;
    double tmp1;
    int t, tp, range, hrange;
    CvSize histoSize, cdfSize;
    IplImage* imgHisto, * cdflmgHisto;

    histoSize.width = 256;
    histoSize.height = 256;

    cdfSize.width = 256;
    cdfSize.height = 256;

    imgHisto = cvCreateImage(histoSize, 8, 1);
    cdflmgHisto = cvCreateImage(cdfSize, 8, 1);

    for (i = 0; i < histoSize.height; i++)
        for (j = 0; j < histoSize.width; j++)
        {
            ((uchar*)(imgHisto->imageData + imgHisto->widthStep * i))[j] = 0;
        }
    for (i = 0; i < cdfSize.height; i++)
        for (j = 0; j < cdfSize.width; j++)
        {
            ((uchar*)(cdflmgHisto->imageData + cdflmgHisto->widthStep * i))[j] = 0;
        }

    tp = X_size * Y_size;

    for (i = 0; i < 256; i++)
        histogram[i] = 0;

    for (i = 0; i < Y_size; i++)
        for (j = 0; j < X_size; j++)
            histogram[img[i][j]]++;

    tmp1 = 0;
    for (i = 0; i < 256; ++i)
    {
        tmp1 = tmp1 > histogram[i] ? tmp1 : histogram[i];
    }

    for (i = 0; i < 256; ++i)
    {
        tmp = (int)255 * (histogram[i] / tmp1);

        cvLine(imgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), cvScalar(255, 255, 255), 1, 8, 0);
    }

    cvShowImage("Histo Line", imgHisto);
    //cvShowImage("Histo match", histogram);
    

    cdfOfHisto[0] = histogram[0];
    for (i = 0; i < 256; i++)
    {
        cdfOfHisto[i] = cdfOfHisto[i - 1] + histogram[i];
    }
    

    tmp1 = (double)cdfOfHisto[255];
    for (i = 0; i < 256; ++i)
    {
        tmp = (int)255 * (cdfOfHisto[i] / tmp1);
        cvLine(cdflmgHisto, cvPoint(i, 255), cvPoint(i, 255 - tmp), cvScalar(255, 255, 255), 1, 8, 0);
    }
    cvShowImage("original CDF of Histogram", cdflmgHisto);
    //cvShowImage("match CDF of Histogram", histogram);

    range = cdfOfHisto[255] - cdfOfHisto[0];
    histogramEqual[0] = 0;
    for (i = 1; i < 256; ++i) {
        t = (int)ceil(((cdfOfHisto[i] - cdfOfHisto[0]) * 255.0) / range);
        histogramEqual[i] = (t < 0) ? 0 : (t > 255) ? 255 : t;
    }
    

    cvReleaseImage(&imgHisto);
    for (i = 0; i < Y_size; ++i)
        for (j = 0; j < X_size; ++j)
        {
            img[i][j] = histogramEqual[img[i][j]];
        }
}

void get_Match(uchar** img, int X_Size, int Y_Size, int histogram[256], int histogramSpec[256], int histogramMatch[256])
{
    void get_hist1(uchar * *img, int X_size, int Y_size);
    int i, j, tmp, matchz = 0;
    float diff;
    printf("Start HistoGram Specification \n");
    for (i = 0; i < 256; i++)
    {
        histogramMatch[i] = 0;
        for (j = 0; j < 256; j++)
        {
            if ((i - histogramSpec[j]) > 0)
            {
                histogramMatch[i] = j;
            }

        }
    }
    //Inverse Processing of expected Histogram
    for (i = 0; i < 256; i++)
    {
        diff = abs(i - histogramSpec[0]);
        matchz = 0;
        for (j = 0; j < 256; j++)
        {
            if (abs(i - histogramSpec[j]) < diff);
            {
                diff = abs(i - histogramSpec[j]);
                matchz = j;
            }
        }
        histogramMatch[i] = (uchar)matchz;
    }

    
    for (i = 0; i < Y_Size; ++i)
        for (j = 0; j < X_Size; ++j)
            img[i][j] = histogramMatch[img[i][j]];
    //cvShowImage("d Line", histogramMatch);
    
    
}

void uc_free(int size_x, int size_y, uchar** ucmatrix)
{
    int i;
    for (i = 0; i < size_y; i++)
        free(ucmatrix[i]);
    free(ucmatrix);
}

int main(int argc, char* argv[])
{
    int i, j;
    IplImage* cvImg;
    CvSize imgSize;
    uchar** img;
    int histogram[100], cdfOfHisto[256], histogramEqual[256], histogramSpec[256], histogramMatch[256];
    int histogramimg[256] = { 0 }; // 히스토그램 배열 초기화
    int histogramSpecimg[256] = { 0 }; // 히스토그램 배열 초기화
    int histogramMatchimg[256] = { 0 }; // 히스토그램 배열 초기화

   

    if (argc != 4)
    {
        printf("Usage: %s imgData x_size y_size\n", argv[0]);
        exit(0);
    }

    imgSize.width = atoi(argv[2]);
    imgSize.height = atoi(argv[3]);

    img = uc_alloc(imgSize.width, imgSize.height);
    
    

    read_ucmatrix(imgSize.width, imgSize.height, img, argv[1]);
    //read_ucmatrix(imgSize.width, imgSize.height, img, argv[4]);
    //read_ucmatrix(imgSize.width, imgSize.height, img, argv[5]);

    

    cvImg = cvCreateImage(imgSize, 8, 1);
  

    for (i = 0; i < imgSize.height; i++)
        for (j = 0; j < imgSize.width; j++)
        {
            ((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
        }

    cvNamedWindow(argv[1], 1);
    cvShowImage(argv[1], cvImg);
    get_hist(img, imgSize.width, imgSize.height);

    for (i = 0; i < imgSize.height; i++)
        for (j = 0; j < imgSize.width; j++)
        {
            ((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
        }

    cvShowImage("Histogram Equalized", cvImg);
    get_hist1(img, imgSize.width, imgSize.height);


    for (i = 0; i < imgSize.width; i++)
        for (j = 0; j < imgSize.height; j++)
        {
            ((uchar*)(cvImg->imageData + cvImg->widthStep * i))[j] = img[i][j];
        }

    cvShowImage("get_Match", cvImg);
    get_Match(img, imgSize.width, imgSize.height, histogram, histogramSpec, histogramMatch);

    

   //get_Match(img, imgSize.width, imgSize.height, matImage, matImage, histogramMatch);
   //cvShowImage("Histogram Match", cvImg);
   //get_Match(img, imgSize.width, imgSize.height, histogram, histogramSpec, histogramMatch);


    cvWaitKey(0);
    cvDestroyWindow(argv[1]);
    cvReleaseImage(&cvImg);
    uc_free(imgSize.width, imgSize.height, img);

    return 0;
}
