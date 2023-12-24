#include <iostream>
#include <bitset>
#include <stdio.h>

int main() {
    int decimalNumber;
    while (true)
    {
        std::cout << "10진수를 입력하세요: ";
        std::cin >> decimalNumber;

        if (decimalNumber < 0) {
            std::cout << "음수는 처리하지 않습니다." << std::endl;
        }
        else {
            std::bitset<32> binary(decimalNumber); // 32비트 2진수로 변환
            std::cout << "10진수: " << decimalNumber << std::endl;
            std::cout << "2진수: " << binary << std::endl;
        }
    }
    return 0;
}

//int main()
//{
//	int input = 0;
//	int mask;
//
//	while (true)
//	{
//		printf("Input Number(Decimal: if you input zero then exit): ");
//		scanf_s("%d", &input);
//
//		if (input == 0)
//		{
//			break;
//		}
//
//		for (int i = 31; i >= 0; i--)
//		{
//			mask = 1 << i;
//			printf("%d", input & mask ? 1 : 0);
//
//
//			if (i % 8 == 0)
//				printf(" ");
//		}
//		printf("\n");
//	}
//
//	return 0;
//}

//#include <stdio.h>
//
//void convert(int n) {
//    if (n > 1)
//        convert(n / 2);
//    printf("%d", n % 2);
//}
//
//int main(/*int argc, char* argv[]*/)
//{
//    /*
//    if (argc != 2)
//    {
//        printf("Please input : Exe, Number\n");
//        exit(0);
//    }
//
//    int dec = argv[1];
//    */
//
//    int dec = 1;
//    printf("10진수를 입력하면 2진수로 변환합니다.\n");
//    printf("0을 입력하면 종료됩니다.\n\n");
//
//    while (dec != 0) {
//        printf("====================\n");
//        printf("입력한 10진수 : ");
//        scanf_s("%d", &dec);
//        printf("변환된 2진수 : ");
//        convert(dec);
//        printf("\n");
//    }
//
//    printf("====================\n");
//    return 0;
//}
