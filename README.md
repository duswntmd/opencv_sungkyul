# 서론
영상처리 및 컴퓨터 비전은 현대 기술 분야에서 광범위하게 활용되며, 다양한 응용 분야에서 사용된다. OpenCV와 Visual Studio를 결합하여 C\+\+ 언어를 활용하여 영상처리에 관한 다양한 기법을 적용하고, 결과를 분석해보고자 한다. 이 프로젝트에서는 다양한 영상처리 기법을 활용하여 여러 영상의 사진을 분석하고, 각각의 크기를 조사하여 결과를 살펴보고자 한다. 이러한 프로젝트를 통해, 영상처리 기술을 이해하고 다양한 응용 분야에서 활용할 수 있는 실력과 능력을 향상시키는 것이 목표이다. OpenCV와 C\+\+을 활용하여 영상처리에 대한 기본적인 지식을 습득하고, 다른 프로젝트에서도 활용할 수 있는 중요한 기술과 도구에 대한 이해를 증진시킬 것이다.

# 본론
이 프로젝트를 시작하기 전에, OpenCV 및 Visual Studio를 설치하고 프로젝트를 설정해야 한다. Visual Studio에서 C\+\+ 프로젝트를 생성하고, OpenCV 라이브러리를 이 프로젝트에 연동한다. OpenCV는 이미지 및 비디오 처리에 유용한 다양한 함수와 도구를 제공하여 영상처리 작업을 효율적으로 수행할 수 있게 도와준다. 다음으로, 여러 영상의 사진을 선택하고 OpenCV를 사용하여 이러한 영상을 불러온다. 이미지를 불러올 때는 이미지 파일의 경로를 지정하고, OpenCV의 함수를 사용하여 이미지 데이터를 읽어와야 한다. 이를 수행하기 위해서 간단한 준비가 필요하다. 준비는 계속 쓰이는 코드준비과정과 속성시트이다. 이제 영상처리 기법을 적용할 차례다. 여러 다양한 기법을 사용할 수 있으며, 본문에서는 10가지인 Nagative, Mosaic, Binary, Adaptive Binary, Gamma, BitPlane, AddNoise, Circle, Logical을 사용해 보겠다.



# 코드준비과정

![image00002](https://github.com/user-attachments/assets/97f80e3c-3796-4df4-99e7-30b3e9aa9ecc)

uchar라는 사용자 지정 데이터 타입을 정의한다. 이 데이터 타입은 부호 없는 8bit 정수를 나타낸다. 2차원 부호 없는 8bit 정수 uchar 배열을 동적으로 할당하는 역할을 한다. 이 함수는 배열의 크기를 받아들이고 해당 크기로 메모리를 할당한 다음 이를 반환한다.

![image00003](https://github.com/user-attachments/assets/927b0a42-0a28-4b4d-a7c6-3bf284c179cf)

이 함수는 파일에서 데이터를 읽어 2차원 uchar 배열에 저장한다. 파일 이름과 배열 크기를 입력으로 받아 파일을 열고 데이터를 배열에 읽어온다.

![image00004](https://github.com/user-attachments/assets/b8480b0b-508b-428f-9f8a-721a46e443f8)

이 함수는 2차원 uchar 배열의 데이터를 파일에 쓴다. 파일 이름과 배열 크기를 입력으로 받아 파일을 생성하고 배열의 데이터를 파일에 쓴다.

![image00005](https://github.com/user-attachments/assets/8ecf8071-c952-4379-a0ab-0963de757970)

![image00006](https://github.com/user-attachments/assets/b6c24c9c-1f66-41f1-a814-b369af317ad2)
![image00007](https://github.com/user-attachments/assets/d65aeac2-f55d-4eb7-969f-9b1f5a9cd57d)
![image00008](https://github.com/user-attachments/assets/80095209-0705-4cfe-b4ff-d86958d6a9ae)

경로를 알맞게 사용했다면 프로젝트 원본 영상 사진과 Col, Row를 cmd에 작성하고 Enter를 치면 원본 영상 사진이 나오는 것을 볼 수 있다.

![image00009](https://github.com/user-attachments/assets/bca16bb1-8267-41f1-9eda-38985139d341)

![image00010](https://github.com/user-attachments/assets/879f1c7f-14cc-493f-9157-37fb7128bdef)
![image00011](https://github.com/user-attachments/assets/87075085-352a-48e0-90f1-aa2361bcf70d)


argc\[4]를 추가해서 write\_ucmatrix를 만들었다. cmd 마지막 줄에 파일 이름을 추가하면 위 영상 사진처럼 이름이 바뀌고 영상처리 기법을 저장하여 데이터 파일을 쓴다.

![image00012](https://github.com/user-attachments/assets/926b413a-063f-4fd1-8ed1-1e3986f94fa1)

이 함수는 uc\_alloc 함수로 할당된 메모리를 해제하는 역할을 한다. 동적으로 할당된 배열을 해제하여 메모리 누수를 방지한다.

# 속성 시트
OpenCV를 사용하기 위해서 속성 설정이 필요하다. 속성 시트는 여러 프로젝트에서 번거롭게 OpenCV 속성을 다시 설정하지 않도록 편하게 만들어 준다.

![image00013](https://github.com/user-attachments/assets/6e251a30-6093-441e-8f1b-a7a251d0cf44)

환경변수를 클릭해 설정에 들어간다. 새로 만들기에서 C:\\opencv\\build\\x64\\vc15\\bin를 추가한다. 추가를 다 했으면 Visual Studio 와서 보기 다른 창 속성 관리자를 들어간다.
추가하는 예시를 보여주겠다.

![image00014](https://github.com/user-attachments/assets/37111f0e-9f0e-4d3d-badd-abe80f6cf5a1)

프로젝트를 생성한다면 오른쪽에 솔루션 탐색기를 볼 수 있다. 속성 관리창에 들어갔다면 위 사진처럼 나올 것이다.

![image00015](https://github.com/user-attachments/assets/c7d40b9f-c40b-4f5a-80b6-3f4cb9f1ba40)

위 예시 같은 경우 mosaic를 오른쪽 버튼을 누르면 나온다. 기존에 사용한 dispcv에서 새 프로젝트 속성 시트 추가를 하였다면 이번에는 새로 만들었던 mosaic에 기존 속성 시트 추가를 클릭한다.

![image00016](https://github.com/user-attachments/assets/1051787c-c70b-46cb-9c17-995d141ea677)
![image00017](https://github.com/user-attachments/assets/6f1f2cc9-88b2-449f-8294-aaf7def84ddc)


그러면 왼쪽 사진과 오른쪽 사진에 openCV가 들어간 것을 볼 수 있다.

이제 OpenCV 속성 설정되었다. 








# Image
<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/4e7631a3-0bef-4deb-a87e-1e814a992ec2" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/b4713a44-4951-441f-8a4f-8fb713b1ae8b" width="150"></td>
   <td><img src="https://github.com/user-attachments/assets/e5c965d1-dfa7-41ce-8de0-f30b58648a86" width="150"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/299661b8-f9ff-45d1-9ba0-7f238ea99fb8" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/236892c6-8e2d-47b7-9fbb-30510593741c" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/fb944626-8fcc-4bd3-8b81-720e015e7454" width="150"></td>
  </tr>
</table>

위 사진은 face, cman, baboon, barbara, lena, jet 원본 사진이다. 사진은 Row와 Col의 크기를 가지고 있고 그중 cman과 face는 Row 256 Col 256이고 baboon, barbara, lena, jet는 Row 512 Col 512 크기를 가지고 있다.

# Nagative Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/146f35c2-edb4-42d7-ab0c-cc4c5174099c" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/337da4cd-17fa-4650-bad2-477697d32336" width="150"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/b5dbb1f5-0d7d-427b-9d17-4ad385429cf2" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/9ef78ead-8dc1-4d1d-a022-80c8a78bdf12" width="150"></td>
  </tr>
</table>


위 왼쪽 사진 2장은 125만큼 Negative 값을 준 사진이다.
오른쪽 2장의 사진은 255만큼의 Negative 값을 준 사진이다.

![image00028](https://github.com/user-attachments/assets/911c614e-5e81-46d5-8c63-1a91b8203e6c)

원본 영상에서 흐릿하게 보였던 검은색과 하얀색 계통의 영상들이 변하면서 눈에 더운 잘 띄는 형상을 볼 수 있다. 인간의 시각체계가 정상적인 영상에서 자 인식하지 못하는 특정 부분들이 존재할 수 있으므로 반대되는 영상에 대해서도 관찰하여 보이지 않던 부분도 볼 경우가 있다. Nagative 영상처럼 X\-Ray 영상의 경우에는 Nagative 영상을 적용함으로써 더 정확하게 영상을 검사하는 것이 가능하다. 

# Mosaic Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/d6848fe2-4aa9-4651-8d46-1a25b6c9142a" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/1ad0d9ee-53fa-48bf-a32e-41a948fad64b" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/392a775c-86d3-4e2f-9e2e-a7037de7afc6" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/3c309d1d-129b-4c15-aff6-2ee5a9b0c7a3" width="150"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/68926b46-8678-4ee8-b481-7d39783b19b2" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/84716cb1-3c87-4c9c-8b2f-70088b8a269a" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/0f458a51-f9d8-4656-b748-7550c3f41f94" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/fe5e0f7e-9864-4ab6-848e-9d619153aa86" width="150"></td>
  </tr>
</table>


위 결과 영상에서 볼 수 있듯 블록의 평균 크기를 점점 기운 것이다. 영상의 크기에 따라 블록이 달라지고 모자이크 효과를 극대화할 수 있지만 모자이크한 영상은 복원할 수 없는 손실이 발생하여 원 영상으로 복구할 수 없게 된다.

![image00037](https://github.com/user-attachments/assets/6600e373-913f-47eb-9792-489808fd64e9)


영상의 사진에 평균과 분산을 계산해 cmd에 출력하는 코드이다.

![image00038](https://github.com/user-attachments/assets/df1c0356-177a-4c77-b69f-73c4a6146be8)



cmd창에서 나오는 결과값이다.


# Binary Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/daf59c91-6861-41d1-aaa5-ad73c6d7c4dc" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/5418d312-62bf-4a6b-be02-2fb56cc89695" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/a2e12a1a-3739-452e-879f-b629d6cc8f8b" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/40dca407-9741-4d1e-b88a-8386f01e6172" width="150"></td>
  </tr>
</table>

이진 영상을 만든 표준 영상 cman, lena, jet, face을 보여준다. 영상은 평균보다 큰 경우 하얀색으로, 작은 경우 검은색으로 설정하였다. 보여주는 색은 다양하게 조절할 수 있으나 일반적인 이진 영상의 경우 검은색과 하얀색으로 나타낸다. 

![image00043](https://github.com/user-attachments/assets/384a574e-fb86-43cc-8176-54c24dd7ba2e)

사진의 평균을 cmd창에서 볼 수 있게 만들었다.

![image00044](https://github.com/user-attachments/assets/aaaf4535-64fd-4cd5-a49e-6bfc2245a189)

img가 avg보다 크다면 결과가 255이고 아니라면 0으로 나오게 만든 코드이다.

![image00045](https://github.com/user-attachments/assets/1002d7b4-2948-428d-8cdd-229990a9ab21)

cmd에서 평균 나온 결과값이다.






# Adaptive Binary Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/14b8662c-fcf8-4155-97eb-26e5135bb0bd" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/7d5115b1-da91-45e0-a7c2-cb4cb3b6ef54" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/47f0066b-1931-456e-ab96-d457d1f9c0a0" width="150"></td>
  </tr>
</table>


A와 B의 특정 값을 정해서 만든 여러 개의 영상 사진이다.

![image00049](https://github.com/user-attachments/assets/643642fe-2416-43f3-b77f-400fc5d4d014)

이미지의 특정 값을 정해서 나오는 함수 코드이다.

![image00050](https://github.com/user-attachments/assets/69111677-a380-4fef-94e0-284db8ebd4da)

밝기변환 함수이다. A와 B 사이의 값은 특별한 값으로 정해주고 나머지는 0에 가까운 값을 주어 값을 배제하는 것이다. 특정한 목적을 가지고 영상을 변환하고자 할 때 적용할 수 있는 영상처리 방법이다. A와 B라는 특정한 값이 목적에 맞는 어떤 값이어야 하며 이 값이 의미가 있으려면 그 결과를 목적으로 하는 특별한 작업이 필요할 때뿐이다. 따라서 일반적인 표준 영상을 처리하는데 적합한 기법은 아니라는 것을 알아두어야 한다.












# Gamma Correction Image 

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/8c0e726b-5aaa-4a0d-8898-8d63a86ad445" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/4babefdc-5368-447b-875d-532a6d8b8635" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/9a2c113a-db40-4be2-8fee-c8cd2e3d7c3c" width="150"></td>
  </tr>
</table>

음극선관을 사용하여 전체적인 밝기를 조정하는 방법이다. Gamma 값을 사용자가 지정함으로써 자신이 보는 화면의 밝기를 전체적으로 어둡게 혹은 밝게 만드는 영상 사진을 볼 수 있다.
위 영상 사진을 보면 0\.1 Gamma를 줌으로써 어두운 것을 볼 수 있고 오른쪽 영상 사진을 갈수록 Gamma 값이 커지면서 밝게 만들어지는 것을 볼 수 있다.

![image00054](https://github.com/user-attachments/assets/e5326e7a-44b0-4365-a77b-cdf7ae57b5a9)

위 코드는 gamma를 커지면서 평균 127\~129 사이의 값을 찾는 것이다. 찾았다면 그 평균값을 보여주고 그에 알맞은 gamma를 보여주도록 만들었다.

![image00055](https://github.com/user-attachments/assets/4922b750-e3ee-4c1c-b0b4-d028c15e3917)

위 영상 사진은 cmd에서 나온 결과물이다. 평균을 찾고 있었고 조건에 만족하여 평균과 gamma를 보여주었고 그에 알맞은 영상 사진까지 출력하였다.

# BitPlane Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/ab7cf051-a09b-4cf8-a654-fde018d34032" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/c1382134-f39f-4fee-ad56-1291e6885374" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/abf8f2fe-2ef8-4969-981f-1922b94bedca" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/934c8a4d-602b-46a7-86ec-ba81168a334d" width="150"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/06276d09-a35a-4ee4-a4e5-81e7532857a3" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/48d4624d-6da1-402e-92b5-e8c3434a55af" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/4061e406-4365-4abd-a867-ca06c7d74101" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/6075eee7-bc97-4470-b018-3811d39e6f5d" width="150"></td>
  </tr>
</table>


Lena 사진을 이용하여 1bit\~8bit까지 비트 슬라이싱을 주며 변화한 과정의 영상 사진이다.

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/e7dc4e0a-7b97-4247-bda8-10db0a9fde11" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/6fd72d3f-03a8-40a5-b89f-9bb13dc88736" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/f6fb9219-07eb-4837-95aa-b8860801c57e" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/b74e6b42-532d-4708-91a9-35d18b23b362" width="150"></td>
    
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/3ef510fb-2267-40cc-8196-ce2a7f94b496" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/86f8ef00-c55d-45e9-8e51-22fe4a947bc4" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/72485956-d0b8-43a3-993a-ab689f322472" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/4783be83-cc44-4209-a9ff-5e77c12ca8be" width="150"></td>
    
  </tr>
</table>


face 사진을 이용하여 1bit\~8bit까지 비트 슬라이싱을 주며 변화한 과정의 영상 사진이다.

표준 디지털 영상은 8 bits로 구성되어 0부터 255까지의 값을 가지고 있는 구조이다. 256레벨의 그레이 스케일 값이라고 표현하기도 하는데 이진 영상을 구성한 것과 같은 각 bit 위치에서의 영상을 독립된 영상으로 표현하는 방법을 알아보았다. 영상은 8개의 bit plane으로 구성되어 있으며 최상위 비트가 1인 경우 128보다 큰 값을 가지게 되고 바로 하위 비트가 1인 경우는 64보다 큰 값을 갖는 구조를 가지고 있다. 각 위치에서의 비트가 1인 경우 영상 값이 존재하는 표시인 255로 표시하여 화면에 보여주고 0인 경우 0으로 표현하여 이진 영상으로 각 bit plane을 표현한다.



# Imagefusion

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/bd44041d-774a-4732-a485-7f284c258083" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/c0294a41-90a2-45d6-8a33-5e6c76f55b8e" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/0dc08751-58ce-4c4e-a9ef-e4281fcf4c6c" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/dec13e55-82b1-4e05-8286-078d3921fa89" width="150"></td>
  </tr>
</table>


위 영상의 사진은 사진을 여러 가지 방법으로 합친 것이다. 위 숫자는 합친 개수이고 영상 기법을 중첩으로 합친 것도 있고 다른 사진과의 조합으로 합친 것도 있다.

# AddNoise Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/3e385b73-aa85-41a9-9404-e44a21372d42" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/987e3506-04e3-462d-87b7-3c614463b135" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/9a2e32bf-667f-4e27-b9c4-4330556fb310" width="150"></td>
  </tr>
</table>


가우시안 랜덤 잡음에 Lena 영상에 대하여 얻은 잡음가산 영상을 보여준다. 영상의 원 값에 화소별로 랜덤 잡음을 더하여 그 결과 값을 얻는 것으로 잡음이 심한 가우시안 잡음을 제거하는 것은 매우 어렵다. 잡음의 사진을 더함으로써 그 결과 영상을 보여주고 있다.영상이 좀 더 밝아진 결과로 보이는 것을 알 수 있다. 잡음 영상을 단순한 가산 연산으로 처리하는 방식이 좋은 결과를 나타내는 경우가 많이 있다.

![image00079](https://github.com/user-attachments/assets/84545f43-daa0-40a4-830e-f3b2e0306a91)

cmd에서의 영상 사진의 잡음이 얼마인지 수치로 볼 수 있다.

# Circle Image

<table>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/a4828e76-12ec-4e17-825d-f54866b7e434" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/87b98573-da00-49bb-b6cd-859e00b06bdd" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/adb25df8-851e-45da-b7ac-8bca668b7025" width="150"></td>
  </tr>
</table>

위 영상 사진은 원이다. diameter를 사용하여 원의 크기를 줄였다 늘렸다 할 수 있다. 그리고 circle 함수를 건드리면 원의 모양을 사용자가 원하는 위치에 이동할 수 있다. Logical Image에서 좀 더 응용해서 보여주겠다.

# Logical Image

<table>
  <tr>
     <td><img src="https://github.com/user-attachments/assets/9bc49c1c-f3b5-4de9-bb18-5848907b17b5" width="150"></td>
     <td><img src="https://github.com/user-attachments/assets/2b88f732-bfb8-4c97-b4de-e496b2f49eba" width="150"></td>
     <td><img src="https://github.com/user-attachments/assets/fbb9d168-7cc1-4145-a7fa-5c61fe526e8d" width="150"></td>
  </tr>
  <tr>
    <td><img src="https://github.com/user-attachments/assets/c6a19e8f-7d22-413d-ac65-55370d003a01" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/e3f75d23-6377-4863-93b0-6df6e7d0defc" width="150"></td>
    <td><img src="https://github.com/user-attachments/assets/5ede0ad7-436c-4c35-a9e3-ad4d4122d1de" width="150"></td>
  </tr>
</table>


circle을 이용하여 논리연산 and, or를 사용해 만든 영상 사진이다. 원을 이동하는 모습을 볼 수 있고 다양한 기법 Nagative, mosaic, binary, bitplane을 사용하여 circle과 적용하여 사용한 것을 볼 수 있다. 영상처리에서 논리연산은 원하는 영상의 모양을 얻거나 제거하기 위한 기법으로 사용하며 얼굴에서 차이나 눈 등 특별히 보고 싶은 부분이 있다면 특정한 영상을 이용하여 원하는 결과 영상을 얻을 수 있다.

# 결론

이 프로젝트를 통해 영상처리와 컴퓨터 비전 분야에 대한 기본적인 이해를 높이고, OpenCV 및 C\+\+을 사용하여 다양한 영상처리 기법을 익히는 기회를 가졌다. 다양한 이미지 처리 기법을 통해 이미지의 특징을 도출하고 시각적으로 향상할 수 있는 능력을 키웠으며, 영상처리 기술의 다양한 응용 가능성에 대한 통찰력을 얻었다. 또한, 이미지의 크기와 해상도의 변화가 영상처리에 미치는 영향을 이해하고, 이미지 처리 작업에 있어 영상의 크기 선택의 중요성을 깨달았다. 이러한 경험과 지식은 다른 프로젝트에서도 활용될 것이다.












