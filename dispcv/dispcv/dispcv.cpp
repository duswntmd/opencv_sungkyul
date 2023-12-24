#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
   for(i=0; i<size_y; i++)
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
   for (i=0; i<size_y; i++)
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



int main(int argc, char* argv[])
{
   int i, j, Row, Col;
   //반드시 추가
   uchar** img;
   uchar** Result;
   double** dimg;
   int block_size = 3;

   if (argc != 4)
   {
      printf("Exe imgData x_size y_size \n");
      exit(0);
   }

   Col = atoi(argv[2]);
   Row = atoi(argv[3]);
   
   //반드시 추가
   

   img = uc_alloc(Col, Row);
   dimg = d_alloc(Col, Row);
   
   //read_ucmatrix(Col, Row, img, argv[1]);
   read_dmatrix(Col, Row, dimg, argv[1]);
   
   // 변수 초기화
   int Block = 8;
   //double avg = average(img, Row, Col);
   //double gamma = 2.2;
   //int position = 5;
   double diameter = 100; //결과 0하면 완전검정 100하면 검정에 중간 하얀원 200하면 완전하양

   // 반드시 추가
   //negative(img, negativeimg, Row, Col, Block);

   //write_ucmatrix(Col, Row, Result, "s");

   //Mat cvImg(Row, Col, CV_8UC(1));
   Mat cvImg(Row, Col, CV_64F);
   
   /*for (i = 0; i < Row; i++)
      for (j = 0; j < Col; j++)
      { 
         cvImg.at<uchar>(i, j) = img[i][j];
      }*/

   for (i = 0; i < Row; i++)
       for (j = 0; j < Col; j++)
       {
           cvImg.at<double>(i, j) = dimg[i][j];
       }

   namedWindow(argv[1], WINDOW_AUTOSIZE);
   imshow(argv[1], cvImg);

   waitKey(0);
   uc_free(Col, Row, img);
   uc_free(Col, Row, Result);
   return 0;
}