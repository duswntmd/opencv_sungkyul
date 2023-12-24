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

double average(uchar** img, int Row, int Col)
{
    double sum = 0, avg;
    int i, j;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            sum += img[i][j];
        }
    }
    avg = sum / ((double)Row * Col);
    printf("Average of Image %If \n", avg);
    return avg;
}

void uc_free(int size_x, int size_y, uchar** ucmatrix)
{
    int i;
    for (i = 0; i < size_y; i++)
        free(ucmatrix[i]);
    free(ucmatrix);
}

void powimg(uchar** img, uchar** Result, int Row, int Col, double gamma)
{
    int i, j;
    double tmp;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            tmp = pow(img[i][j] / 255., 1 / gamma);

            if (tmp * 255 > 255) tmp = 1;
            else if (tmp * 255 < 0) tmp = 0;

            Result[i][j] = tmp * 255;
        }
    }
}

void powimg(uchar** img, uchar** Result, int Row, int Col, double gamma);

int main(int argc, char* argv[])
{
    int i, j, Row, Col;
    //반드시 추가
    uchar** img;
    uchar** Result;
    uchar** powimgimg;
    
    if (argc != 4)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);


    //반드시 추가
    img = uc_alloc(Col, Row);
    powimgimg = uc_alloc(Col, Row);

    read_ucmatrix(Col, Row, img, argv[1]);

    // 변수 초기화
    int Block = 8;
    double gamma = 0.2;

    // 반드시 추가
    powimg(img, powimgimg, Row, Col, gamma);

    //write_ucmatrix(Col, Row, powimgimg, "lena512.gammab");

    Mat cvImg(Row, Col, CV_8UC(1));

    //for (gamma = 0; gamma <= 3.0; gamma += 0.01) {
    //    powimg(img, powimgimg, Row, Col, gamma); // PowImg 함수 호출

    //    // 계산된 감마 보정 결과의 평균을 구함
    //    double avg = average(powimgimg, Row, Col);

    //    // 평균이 127에서 129 사이인 경우 결과를 출력하고 종료
    //    if (avg >= 127 && avg <= 129) {
    //        printf("Found a result with avg = %lf at gamma = %lf\n", avg, gamma);
    //        break;
    //    }
    //}

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            cvImg.at<uchar>(i, j) = powimgimg[i][j];
        }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], cvImg);

    waitKey(0);
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);
    return 0;
}