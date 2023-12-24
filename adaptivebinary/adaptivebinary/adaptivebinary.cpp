#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>

typedef unsigned char uchar;   // #define uchar unsigned char 라고 해도 같은 뜻임.

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

void adaptivebinary2(uchar** img, uchar** out, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) out[i][j] = img[i][j];
            else out[i][j] = 0;
        }
    }
}

void adaptivebinary1(uchar** img, uchar** out, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) out[i][j] = 200;
            else out[i][j] = img[i][j];
        }
    }
}

void adaptivebinary0(uchar** img, uchar** out, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if (img[i][j] > 50 && img[i][j] < 120) out[i][j] = 200;
            else out[i][j] = 0;
        }
    }
}

void adaptivebinary0(uchar** img, uchar** out, int Row, int Col);
void adaptivebinary1(uchar** img, uchar** out, int Row, int Col);
void adaptivebinary2(uchar** img, uchar** out, int Row, int Col);

int main(int argc, char* argv[])
{
    int i, j, Row, Col;
    //반드시 추가
    uchar** img;
    uchar** Result;
    uchar** adaptivebinary0img;
    uchar** adaptivebinary1img;
    uchar** adaptivebinary2img;

    if (argc != 4)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);

    //반드시 추가
    img = uc_alloc(Col, Row);
    adaptivebinary0img = uc_alloc(Col, Row);
    adaptivebinary1img = uc_alloc(Col, Row);
    adaptivebinary2img = uc_alloc(Col, Row);

    read_ucmatrix(Col, Row, img, argv[1]);

    // 변수 초기화
    int Block = 8;

    // 반드시 추가
    adaptivebinary0(img, adaptivebinary0img, Row, Col);
    adaptivebinary1(img, adaptivebinary1img, Row, Col);
    adaptivebinary2(img, adaptivebinary2img, Row, Col);
    
    //write_ucmatrix(Col, Row, adaptivebinary0img, "cman256.adaptivebinary0img");

    Mat cvImg(Row, Col, CV_8UC(1));

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            cvImg.at<uchar>(i, j) = adaptivebinary2img[i][j];
        }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], cvImg);

    waitKey(0);
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);
    return 0;
}