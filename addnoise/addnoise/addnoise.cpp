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

double uniform()
{
    return((double)(rand() & RAND_MAX) / RAND_MAX - 0.5);
}

double gaussian()
{
    static int ready = 0;
    static double gstore;
    double v1, v2, r, fac, gaus;
    double uniform();

    if (ready == 0) {
        do {
            v1 = 2. * uniform();
            v2 = 2. * uniform();
            r = v1 * v2 + v2 * v2;
        } while (r > 1.0);
        fac = sqrt(-2 * log(r) / r);
        gstore = v1 * fac;
        gaus = v2 * fac;
        ready = 1;
    }
    else {
        ready = 0;
        gaus = gstore;
    }
    return(gaus);
}

void psnruc(uchar** IM1, uchar** IM2, int Col, int Row, double* MSE, double* PSNR)
{
    int i, j;
    double temp, diff_sum = 0;

    for (i = 0; i < Row; i++)
        for (j = 0; j < Col; j++)
        {
            uchar noise = (uchar)(50 + gaussian());//여기부분

            int noisy_pixel = IM1[i][j] + noise;

            if (noisy_pixel < 0)
                noisy_pixel = 0;
            else if (noisy_pixel > 255)
                noisy_pixel = 255;



            // 결과 이미지를 0에서 255 사이로 클리핑
            noisy_pixel = std::max(0, std::min(255, noisy_pixel));

            IM2[i][j] = noisy_pixel;


            // 계산을 위한 MSE 및 PSNR 업데이트
            temp = (double)(IM1[i][j] - IM2[i][j]);
            diff_sum += temp * temp;
        }

    *MSE = diff_sum / ((double)Row * (double)Col);
    if (*MSE)
        *PSNR = (10. * log10((255. * 255.) / *MSE));

    printf("psnr = %10.4lf\n", *PSNR);
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
    uchar** IM1;
    uchar** IM2;
    uchar** Result;
    uchar** psnrucimg;

    if (argc != 4)
    {
        printf("Exe imgData x_size y_size \n");
        exit(0);
    }

    Col = atoi(argv[2]);
    Row = atoi(argv[3]);


    //반드시 추가
    img = uc_alloc(Col, Row);
    IM1 = uc_alloc(Col, Row);
    IM2 = uc_alloc(Col, Row);
    psnrucimg = uc_alloc(Col, Row);

    read_ucmatrix(Col, Row, img, argv[1]);

    // 변수 초기화
    double MSE = 0.0;
    double PSNR = 0.0;


    // 반드시 추가
    psnruc(img, psnrucimg, Row, Col, &MSE, &PSNR);
    //write_ucmatrix(Col, Row, psnrucimg, "lena512.addnoise");
    

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

//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <stdio.h>
//
//typedef unsigned char uchar;
//using namespace std;
//using namespace cv;
//
//uchar** uc_alloc(int size_x, int size_y)
//{
//    uchar** m;
//    int i;
//    if ((m = (uchar**)calloc(size_y, sizeof(uchar*))) == NULL)
//    {
//        printf("d_alloc error 1\7\n");
//        exit(0);
//    }
//
//    for (i = 0; i < size_y; i++)
//        if ((m[i] = (uchar*)calloc(size_x, sizeof(uchar))) == NULL)
//        {
//            printf("d_alloc error2\7\n");
//            exit(0);
//        }
//    return m;
//}
//
//void read_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
//{
//    int i;
//    FILE* f;
//
//    if ((f = fopen(filename, "rb")) == NULL)
//    {
//        printf(" % s File open Error!\n", filename);
//        exit(0);
//    }
//    for (i = 0; i < size_y; i++)
//        if (fread(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
//        {
//            printf("Data Read Error!\n");
//            exit(0);
//        }
//    fclose(f);
//}
//
//void write_ucmatrix(int size_x, int size_y, uchar** ucmatrix, const char* filename)
//{
//    int i;
//    FILE* f;
//
//    if ((f = fopen(filename, "wb")) == NULL)
//    {
//        printf(" %s File open Error!\n", filename);
//        exit(0);
//    }
//
//    for (i = 0; i < size_y; i++)
//    {
//        if (fwrite(ucmatrix[i], sizeof(uchar), size_x, f) != size_x)
//        {
//            printf("Data Write Error!\n");
//            exit(0);
//        }
//    }
//    fclose(f);
//}
//
//double uniform()
//{
//    return ((double)(rand() & RAND_MAX) / RAND_MAX - 0.5);
//}
//
//double gaussian()
//{
//    static int ready = 0;       //flag to indicated value
//    static double gstore;       //place to store other value
//    double v1, v2, r, fac, gaus;
//    double uniform();
//
//    if (ready == 0) {
//        do {
//            v1 = 2. * uniform();
//            v2 = 2. * uniform();
//            r = v1 * v1 + v2 * v2;
//        } while (r > 1.0);              //make radius less than 1
//        // remap v1 and v2 to two gaussian numbers
//        fac = sqrt(-2. * log(r) / r);
//        gstore = v1 * fac;      //store one
//        gaus = v2 * fac;        //return 1
//        ready = 1;              //set ready flag
//    }
//    else {
//        ready = 0;                 //reset ready flag for next pair
//        gaus = gstore;             //return the stored one
//    }
//    return(gaus);
//}
//
//double compute_psnr(unsigned char* original, unsigned char* noisy, int width, int height) {
//    double mse = 0.0;
//
//    for (int i = 0; i < width * height; i++) {
//        mse += pow((original[i] - noisy[i]), 2);
//    }
//
//    mse /= (double)(width * height);
//
//    if (mse == 0.0) {
//        return 100.0; // 이미지가 동일한 경우, PSNR은 무한대로 가깝습니다.
//    }
//
//    double max_pixel = 255.0; // 이미지 픽셀 최대 값
//
//    double psnr = 20 * log10(max_pixel / sqrt(mse));
//
//    return psnr;
//}
//
//int main(int argc, char* argv[])
//{
//    int i, j, Row, Col;
//    uchar** img;
//    uchar** img;
//
//    if (argc != 4)
//    {
//        printf("Usage: %s <image_file> <width> <height>\n", argv[0]);
//        exit(0);
//    }
//
//    Col = atoi(argv[2]);
//    Row = atoi(argv[3]);
//    img = uc_alloc(Col, Row);
//    read_ucmatrix(Col, Row, img, argv[1]);
//    
//    write_ucmatrix(Col, Row, img, "lena512.addnoise");
//    // 이미지를 OpenCV 형식(Mat)으로 변환
//    Mat cvImg(Row, Col, CV_8UC1);
//    for (i = 0; i < Row; i++)
//        for (j = 0; j < Col; j++)
//        {
//            cvImg.at<uchar>(i, j) = img[i][j];
//        }
//    namedWindow(argv[1], WINDOW_AUTOSIZE);
//    imshow(argv[1], cvImg);
//
//    // 가우시안 노이즈를 이미지에 추가
//    Mat noisyImage;
//    cvImg.copyTo(noisyImage);
//    for (i = 0; i < Row; i++)
//    {
//        for (j = 0; j < Col; j++)
//        {
//            double noise = gaussian(); // 가우시안 노이즈 사용
//            uchar pixelValue = noisyImage.at<uchar>(i, j);
//            noisyImage.at<uchar>(i, j) = saturate_cast<uchar>(pixelValue + noise * 32); // 노이즈 크기 조절
//        }
//    }
//
//    // 노이즈가 추가된 이미지 표시
//    namedWindow("Noisy Image", WINDOW_AUTOSIZE);
//    imshow("Noisy Image", noisyImage);
//
//    // 이미지의 PSNR 계산
//    double psnr_value = compute_psnr(cvImg.data, noisyImage.data, Col, Row);
//    cout << "PSNR: " << psnr_value << endl;
//
//    waitKey(0);
//
//    return 0;
//}