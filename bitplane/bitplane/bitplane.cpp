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

void bitslicing(uchar** img, uchar** Result, int Row, int Col, int position)
{
    int i, j;
    uchar mask = 0x01;
    mask <<= position;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            if ((mask & img[i][j]) == pow(2, position))
            {
                Result[i][j] = 255;
            }
            else
            {
                Result[i][j] = 0;
            }
        }
    }
}



void imgsum(uchar** in1Img, uchar** in2Img, uchar** outimg2, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            outimg2[i][j] = in1Img[i][j] + in2Img[i][j];
        }
}

void imgsum1(uchar** in1Img, uchar** in2Img, uchar** in3Img, uchar** outimg2, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            outimg2[i][j] = in1Img[i][j] & in2Img[i][j] & in3Img[i][j];
        }
}


void bitslicing(uchar** img, uchar** Result, int Row, int Col, int position);

int main(int argc, char* argv[])
{
    int i, j, Row, Col;
    //반드시 추가
    uchar** img;
    uchar** outimg2;
    uchar** outimg3;
    uchar** Result;
    uchar** bitslicingimg;
    uchar** imgsumimg;
    uchar** img4;
    uchar** img5;
    uchar** img6;

    if (argc != 4)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);

    //반드시 추가
    img = uc_alloc(Col, Row);
    outimg2 = uc_alloc(Col, Row);
    outimg3 = uc_alloc(Col, Row);
    bitslicingimg = uc_alloc(Col, Row);
    img4 = uc_alloc(Col, Row);
    img5 = uc_alloc(Col, Row);
    img6 = uc_alloc(Col, Row);

    read_ucmatrix(Col, Row, img, argv[1]);
    //read_ucmatrix(Col, Row, img4, argv[4]);
    //read_ucmatrix(Col, Row, img5, argv[5]);
    //read_ucmatrix(Col, Row, img6, argv[6]);
    
    // 변수 초기화
    int Block = 8;
    int position = 7;

    // 반드시 추가   
    bitslicing(img, bitslicingimg, Row, Col, position);
    imgsum(img4, img5, outimg2, Row, Col); //outimg2로 결과 값을 본다 img4, img5는 read_ucmatrix를 사용하여 원본2개를 바로 합칠수있다.
    imgsum1(img4, img5, img6, outimg3, Row, Col); //하지만 bitslicingimg 와 img처럼 하면 함수의 처리된 값을 적용 후 이미지가 합쳐진다.
                                          //imgsum함수에 어떤 부호를 넣냐에 따라 그림 합쳐짐이 달라진다. ex) +, &, | 요런거?
                                          //원본이미지 2개를 바로 합치고 싶다면 read_ucmatrix를 활성화해라 그리고 argc != 6 변경해라
    //write_ucmatrix(Col, Row, bitslicingimg, "cman256.bitplane8");

    Mat cvImg(Row, Col, CV_8UC(1));

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            cvImg.at<uchar>(i, j) = img[i][j];
        }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], cvImg);

    waitKey(0);
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);
    return 0;
}