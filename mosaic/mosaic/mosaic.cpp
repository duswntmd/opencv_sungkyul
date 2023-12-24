#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>

typedef unsigned char uchar;   

using namespace std;
using namespace cv;

uchar** uc_alloc(int size_x, int size_y)  
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

void mosaic(uchar** img, uchar** out, int Row, int Col, int Block)
{
    int i, j, x, y, tmp, count;
    for (i = 0; i < Row; i += Block)
    {
        for (j = 0; j < Col; j += Block)
        {
            tmp = 0;
            count = 0;
            for (y = 0; y < Block; y++)
            {
                for (x = 0; x < Block; x++)
                {
                    tmp += img[i + y][j + x];
                    count++;
                }
            } // get average

            tmp /= count;
            for (y = 0; y < Block; y++)
            {
                for (x = 0; x < Block; x++)
                {
                    out[i + y][j + x] = tmp;
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    int i, j, Row, Col;
    //�ݵ�� �߰�
    uchar** img;
    uchar** Result;
    uchar** mosaicImg; 
    


    if (argc != 4)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);

    //�ݵ�� �߰�
    img = uc_alloc(Col, Row);
    mosaicImg = uc_alloc(Col, Row); // ������ũ ��� �̹����� ���� �޸� �Ҵ�
   
    read_ucmatrix(Col, Row, img, argv[1]);
    


    // ���� �ʱ�ȭ
    double mean = 0.0;
    double variance = 0.0;
    int Block = 32;

    mosaic(img, mosaicImg, Row, Col, Block);
    //write_ucmatrix(Col, Row, mosaicImg, "face256.mosaic");
    Mat cvImg(Row, Col, CV_8UC(1));

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            mean += img[i][j]; // �ȼ��� �����Ͽ� ��� ����� ���� ���
           variance += img[i][j] * img[i][j]; // �ȼ��� ���� �����Ͽ� �л� ����� ���� ���
        }

    mean /= (Row * Col); // ��� ���
    variance = (variance / (Row * Col)) - (mean * mean); // �л� ���

     // ��հ� �л��� ����Ѵ�.
    printf("�ȼ��� ���: %lf\n", mean);
    printf("�ȼ��� �л�: %lf\n", variance);

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            cvImg.at<uchar>(i, j) = mosaicImg[i][j];
        }

    namedWindow(argv[1], WINDOW_AUTOSIZE);
    imshow(argv[1], cvImg);

    waitKey(0);
    uc_free(Col, Row, img);
    uc_free(Col, Row, Result);
    return 0;
}