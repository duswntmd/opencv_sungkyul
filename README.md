# 서론
영상처리 및 컴퓨터 비전은 현대 기술 분야에서 광범위하게 활용되며, 다양한 응용 분야에서 사용된다. OpenCV와 Visual Studio를 결합하여 C\+\+ 언어를 활용하여 영상처리에 관한 다양한 기법을 적용하고, 결과를 분석해보고자 한다. 이 프로젝트에서는 다양한 영상처리 기법을 활용하여 여러 영상의 사진을 분석하고, 각각의 크기를 조사하여 결과를 살펴보고자 한다. 이러한 프로젝트를 통해, 영상처리 기술을 이해하고 다양한 응용 분야에서 활용할 수 있는 실력과 능력을 향상시키는 것이 목표이다. OpenCV와 C\+\+을 활용하여 영상처리에 대한 기본적인 지식을 습득하고, 다른 프로젝트에서도 활용할 수 있는 중요한 기술과 도구에 대한 이해를 증진시킬 것이다.

# 본론
이 프로젝트를 시작하기 전에, OpenCV 및 Visual Studio를 설치하고 프로젝트를 설정해야 한다. Visual Studio에서 C\+\+ 프로젝트를 생성하고, OpenCV 라이브러리를 이 프로젝트에 연동한다. OpenCV는 이미지 및 비디오 처리에 유용한 다양한 함수와 도구를 제공하여 영상처리 작업을 효율적으로 수행할 수 있게 도와준다. 다음으로, 여러 영상의 사진을 선택하고 OpenCV를 사용하여 이러한 영상을 불러온다. 이미지를 불러올 때는 이미지 파일의 경로를 지정하고, OpenCV의 함수를 사용하여 이미지 데이터를 읽어와야 한다. 이를 수행하기 위해서 간단한 준비가 필요하다. 준비는 계속 쓰이는 코드준비과정과 속성시트이다. 이제 영상처리 기법을 적용할 차례다. 여러 다양한 기법을 사용할 수 있으며, 본문에서는 10가지인 Nagative, Mosaic, Binary, Adaptive Binary, Gamma, BitPlane, AddNoise, Circle, Logical을 사용해 보겠다.



코드준비과정


uchar라는 사용자 지정 데이터 타입을 정의한다. 이 데이터 타입은 부호 없는 8bit 정수를 나타낸다. 2차원 부호 없는 8bit 정수 uchar 배열을 동적으로 할당하는 역할을 한다. 이 함수는 배열의 크기를 받아들이고 해당 크기로 메모리를 할당한 다음 이를 반환한다.


이 함수는 파일에서 데이터를 읽어 2차원 uchar 배열에 저장한다. 파일 이름과 배열 크기를 입력으로 받아 파일을 열고 데이터를 배열에 읽어온다.



이 함수는 2차원 uchar 배열의 데이터를 파일에 쓴다. 파일 이름과 배열 크기를 입력으로 받아 파일을 생성하고 배열의 데이터를 파일에 쓴다.



경로를 알맞게 사용했다면 프로젝트 원본 영상 사진과 Col, Row를 cmd에 작성하고 Enter를 치면 원본 영상 사진이 나오는 것을 볼 수 있다.





argc\[4]를 추가해서 write\_ucmatrix를 만들었다. cmd 마지막 줄에 파일 이름을 추가하면 위 영상 사진처럼 이름이 바뀌고 영상처리 기법을 저장하여 데이터 파일을 쓴다.


이 함수는 uc\_alloc 함수로 할당된 메모리를 해제하는 역할을 한다. 동적으로 할당된 배열을 해제하여 메모리 누수를 방지한다.

속성 시트
OpenCV를 사용하기 위해서 속성 설정이 필요하다. 속성 시트는 여러 프로젝트에서 번거롭게 OpenCV 속성을 다시 설정하지 않도록 편하게 만들어 준다.


환경변수를 클릭해 설정에 들어간다. 새로 만들기에서 C:\\opencv\\build\\x64\\vc15\\bin를 추가한다. 추가를 다 했으면 Visual Studio 와서 보기 다른 창 속성 관리자를 들어간다.
추가하는 예시를 보여주겠다.


프로젝트를 생성한다면 오른쪽에 솔루션 탐색기를 볼 수 있다. 속성 관리창에 들어갔다면 위 사진처럼 나올 것이다.


위 예시 같은 경우 mosaic를 오른쪽 버튼을 누르면 나온다. 기존에 사용한 dispcv에서 새 프로젝트 속성 시트 추가를 하였다면 이번에는 새로 만들었던 mosaic에 기존 속성 시트 추가를 클릭한다.


그러면 왼쪽 사진과 오른쪽 사진에 openCV가 들어간 것을 볼 수 있다.
이제 OpenCV 속성 설정되었다. 













Image


위 사진은 face, cman, baboon, barbara, lena, jet 원본 사진이다. 사진은 Row와 Col의 크기를 가지고 있고 그중 cman과 face는 Row 256 Col 256이고 baboon, barbara, lena, jet는 Row 512 Col 512 크기를 가지고 있다.

Nagative Image

위 왼쪽 사진 2장은 125만큼 Negative 값을 준 사진이다.
오른쪽 2장의 사진은 255만큼의 Negative 값을 준 사진이다.


원본 영상에서 흐릿하게 보였던 검은색과 하얀색 계통의 영상들이 변하면서 눈에 더운 잘 띄는 형상을 볼 수 있다. 인간의 시각체계가 정상적인 영상에서 자 인식하지 못하는 특정 부분들이 존재할 수 있으므로 반대되는 영상에 대해서도 관찰하여 보이지 않던 부분도 볼 경우가 있다. Nagative 영상처럼 X\-Ray 영상의 경우에는 Nagative 영상을 적용함으로써 더 정확하게 영상을 검사하는 것이 가능하다. 

Mosaic Image
 4 8 16 32

 4 8 16 32

위 결과 영상에서 볼 수 있듯 블록의 평균 크기를 점점 기운 것이다. 영상의 크기에 따라 블록이 달라지고 모자이크 효과를 극대화할 수 있지만 모자이크한 영상은 복원할 수 없는 손실이 발생하여 원 영상으로 복구할 수 없게 된다.


영상의 사진에 평균과 분산을 계산해 cmd에 출력하는 코드이다.


cmd창에서 나오는 결과값이다.







Binary Image

이진 영상을 만든 표준 영상 cman, lena, jet, face을 보여준다. 영상은 평균보다 큰 경우 하얀색으로, 작은 경우 검은색으로 설정하였다. 보여주는 색은 다양하게 조절할 수 있으나 일반적인 이진 영상의 경우 검은색과 하얀색으로 나타낸다. 


사진의 평균을 cmd창에서 볼 수 있게 만들었다.


img가 avg보다 크다면 결과가 255이고 아니라면 0으로 나오게 만든 코드이다.


cmd에서 평균 나온 결과값이다.






Adaptive Binary Image

A와 B의 특정 값을 정해서 만든 여러 개의 영상 사진이다.



이미지의 특정 값을 정해서 나오는 함수 코드이다.


밝기변환 함수이다. A와 B 사이의 값은 특별한 값으로 정해주고 나머지는 0에 가까운 값을 주어 값을 배제하는 것이다. 특정한 목적을 가지고 영상을 변환하고자 할 때 적용할 수 있는 영상처리 방법이다. A와 B라는 특정한 값이 목적에 맞는 어떤 값이어야 하며 이 값이 의미가 있으려면 그 결과를 목적으로 하는 특별한 작업이 필요할 때뿐이다. 따라서 일반적인 표준 영상을 처리하는데 적합한 기법은 아니라는 것을 알아두어야 한다.












Gamma Correction Image 
 0\.1 1\.4 2\.4

음극선관을 사용하여 전체적인 밝기를 조정하는 방법이다. Gamma 값을 사용자가 지정함으로써 자신이 보는 화면의 밝기를 전체적으로 어둡게 혹은 밝게 만드는 영상 사진을 볼 수 있다.
위 영상 사진을 보면 0\.1 Gamma를 줌으로써 어두운 것을 볼 수 있고 오른쪽 영상 사진을 갈수록 Gamma 값이 커지면서 밝게 만들어지는 것을 볼 수 있다.



위 코드는 gamma를 커지면서 평균 127\~129 사이의 값을 찾는 것이다. 찾았다면 그 평균값을 보여주고 그에 알맞은 gamma를 보여주도록 만들었다.


위 영상 사진은 cmd에서 나온 결과물이다. 평균을 찾고 있었고 조건에 만족하여 평균과 gamma를 보여주었고 그에 알맞은 영상 사진까지 출력하였다.
BitPlane Image

Lena 사진을 이용하여 1bit\~8bit까지 비트 슬라이싱을 주며 변화한 과정의 영상 사진이다.

face 사진을 이용하여 1bit\~8bit까지 비트 슬라이싱을 주며 변화한 과정의 영상 사진이다.

표준 디지털 영상은 8 bits로 구성되어 0부터 255까지의 값을 가지고 있는 구조이다. 256레벨의 그레이 스케일 값이라고 표현하기도 하는데 이진 영상을 구성한 것과 같은 각 bit 위치에서의 영상을 독립된 영상으로 표현하는 방법을 알아보았다. 영상은 8개의 bit plane으로 구성되어 있으며 최상위 비트가 1인 경우 128보다 큰 값을 가지게 되고 바로 하위 비트가 1인 경우는 64보다 큰 값을 갖는 구조를 가지고 있다. 각 위치에서의 비트가 1인 경우 영상 값이 존재하는 표시인 255로 표시하여 화면에 보여주고 0인 경우 0으로 표현하여 이진 영상으로 각 bit plane을 표현한다.



Imagefusion
 3 3 3 2
위 영상의 사진은 사진을 여러 가지 방법으로 합친 것이다. 위 숫자는 합친 개수이고 영상 기법을 중첩으로 합친 것도 있고 다른 사진과의 조합으로 합친 것도 있다.

AddNoise Image

가우시안 랜덤 잡음에 Lena 영상에 대하여 얻은 잡음가산 영상을 보여준다. 영상의 원 값에 화소별로 랜덤 잡음을 더하여 그 결과 값을 얻는 것으로 잡음이 심한 가우시안 잡음을 제거하는 것은 매우 어렵다. 잡음의 사진을 더함으로써 그 결과 영상을 보여주고 있다.영상이 좀 더 밝아진 결과로 보이는 것을 알 수 있다. 잡음 영상을 단순한 가산 연산으로 처리하는 방식이 좋은 결과를 나타내는 경우가 많이 있다.



cmd에서의 영상 사진의 잡음이 얼마인지 수치로 볼 수 있다.

Circle Image

위 영상 사진은 원이다. diameter를 사용하여 원의 크기를 줄였다 늘렸다 할 수 있다. 그리고 circle 함수를 건드리면 원의 모양을 사용자가 원하는 위치에 이동할 수 있다. Logical Image에서 좀 더 응용해서 보여주겠다.
Logical Image

circle을 이용하여 논리연산 and, or를 사용해 만든 영상 사진이다. 원을 이동하는 모습을 볼 수 있고 다양한 기법 Nagative, mosaic, binary, bitplane을 사용하여 circle과 적용하여 사용한 것을 볼 수 있다. 영상처리에서 논리연산은 원하는 영상의 모양을 얻거나 제거하기 위한 기법으로 사용하며 얼굴에서 차이나 눈 등 특별히 보고 싶은 부분이 있다면 특정한 영상을 이용하여 원하는 결과 영상을 얻을 수 있다.

결론
이 프로젝트를 통해 영상처리와 컴퓨터 비전 분야에 대한 기본적인 이해를 높이고, OpenCV 및 C\+\+을 사용하여 다양한 영상처리 기법을 익히는 기회를 가졌다. 다양한 이미지 처리 기법을 통해 이미지의 특징을 도출하고 시각적으로 향상할 수 있는 능력을 키웠으며, 영상처리 기술의 다양한 응용 가능성에 대한 통찰력을 얻었다. 또한, 이미지의 크기와 해상도의 변화가 영상처리에 미치는 영향을 이해하고, 이미지 처리 작업에 있어 영상의 크기 선택의 중요성을 깨달았다. 이러한 경험과 지식은 다른 프로젝트에서도 활용될 것이다.












