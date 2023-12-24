#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>

typedef unsigned char uchar;   // #define uchar unsigned char ��� �ص� ���� ����.

using namespace std;
using namespace cv;

uchar** uc_alloc(int size_x, int size_y)  //uchar�� double�� �ٲٴ� ���� �´�.
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
        printf("%s File open Error! ������ ã�ƺ�����\n", filename);
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

void circle(uchar** Result, int Row, int Col, double diameter)
{
    int i, j;
    double tmp, xsqure, ysqure;

    for (i = 0; i < Row; i++) {
        for (j = 0; j < Col; j++) {
            ysqure = (abs(Row / 2.5 - i)) * (abs(Row / 2.5 - i));
            xsqure = (abs(Col / 2.5 - j)) * (abs(Col / 2.5 - j));

            tmp = sqrt(ysqure + xsqure);

            if (tmp < diameter) Result[i][j] = 255;
            else Result[i][j] = 0;
        }
    }
}

void maskand(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            outImg[i][j] = in1Img[i][j] & in2Img[i][j];
        }
}

void maskor(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col)
{
    int i, j;

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            outImg[i][j] = in1Img[i][j] | in2Img[i][j];
        }
}

void circle(uchar** Result, int Row, int Col, double diameter);
void maskand(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col);
void maskor(uchar** in1Img, uchar** in2Img, uchar** outImg, int Row, int Col);

int main(int argc, char* argv[])
{
    int i, j, Row, Col;
    //�ݵ�� �߰�
    uchar** img;
    uchar** img2;
    uchar** Result;
    uchar** circleimg;
    uchar** maskandimg;
    uchar** maskorimg;

    if (argc != 5)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);

    //�ݵ�� �߰�
    img = uc_alloc(Col, Row);
    img2 = uc_alloc(Col, Row);
    circleimg = uc_alloc(Col, Row);
    maskandimg = uc_alloc(Col, Row);
    maskorimg = uc_alloc(Col, Row);

    read_ucmatrix(Col, Row, img, argv[1]);
    read_ucmatrix(Col, Row, img2, argv[4]);

    // ���� �ʱ�ȭ
    int Block = 8;
    double diameter = 20; //��� 0�ϸ� �������� 100�ϸ� ������ �߰� �Ͼ�� 200�ϸ� �����Ͼ�

    // �ݵ�� �߰�
    circle(circleimg, Row, Col, diameter);
    maskand(img, circleimg, maskandimg, Row, Col);
    maskor(img, img2, maskorimg, Row, Col);

    //write_ucmatrix(Col, Row, maskorimg, "cman256.maskor");

    Mat cvImg(Row, Col, CV_8UC(1));

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            cvImg.at<uchar>(i, j) = maskorimg[i][j];
        }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], cvImg);

    waitKey(0);
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);
    return 0;
}