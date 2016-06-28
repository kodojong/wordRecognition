//main2.cpp

#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <queue>
#include <list>
#include <cmath>

using namespace std;

#define EXTEND_RATIO 10
#define PI 3.14/180

//최소인접사각형을 구하기위함
class Rectangle {
public:
	Rectangle() {
		cX = cY = xL = xR = yT = yB = 0;
	}
	Rectangle(int radian, int circularNumber) {
		cX = cY = xL = xR = yT = yB = 0;
		**radianMatrix = new bool*[circularNumber];
		for (int i = 0; i < circularNumber; i++) {
			radianMatrix[i] = new bool[(360 / radian)];
		}
	}
	void clearRectangle() {
		cX = cY = xL = xR = yT = yB = 0;
	}
	int xL, xR; //x의 좌,우좌표
	int yT, yB; //y의 상,하좌표
	int cX, cY; //중심위치
	bool **radianMatrix;

	//중심좌표 계산
	void calcCenter() {
		cX = ((xR - xL) / 2) + xL;
		cY = ((yB - yT) / 2) + yT;
	}
};

class wordCharacter {

public:

	wordCharacter(int circularNumber, int checkRadian, int offset) {
		radianMatrix = new bool*[circularNumber];
		for (int i = 0; i < circularNumber; i++) {
			radianMatrix[i] = new bool[((360 / checkRadian) / offset)];
		}
	}

	bool **radianMatrix;

};
int main() {

	list<wordCharacter> wordList;

	for (int wordnum = 0; wordnum < 1; ++wordnum) { //테스트할 단어 jpg개수
		IplImage *originalImg = 0; //original img
		IplImage *imgHistogram = 0; //histgram img
		IplImage *binaryImg = 0; //binary img

		int bins = 256;
		int sizes[] = { bins };

		float max_value = 0, min_value = 0;

		//ranges-grayscale 0 to 255
		float xranges[] = { 0, 255 };
		float*ranges[] = { xranges };

		//string filename = "images\\\\number\\\\";
		//filename += (char)(wordnum + 48);
		//filename += ".jpg";
		//original image set gray
		originalImg = cvLoadImage("images\\number\\6.jpg", 0);
		//originalImg = cvLoadImage(filename.c_str(), 0);
		imgHistogram = cvCreateImage(cvGetSize(originalImg), IPL_DEPTH_8U, 1);
		binaryImg = cvCreateImage(cvGetSize(originalImg), IPL_DEPTH_8U, 1);

		//cvThreshold 원본이미지 이진화
		cvThreshold(originalImg, binaryImg, 200, 255, CV_THRESH_BINARY); //dst를 이진화 하여 dst에 넣음


																		 //2차원 픽셀테이블 생성
		double **binaryImgMatrix = new double*[binaryImg->height];
		for (int i = 0; i < binaryImg->height; i++) {
			binaryImgMatrix[i] = new double[binaryImg->width];
		}

		//히스토그램 위한 가로,세로열 테이블 생성
		double *binaryImgHorizonNum = new double[binaryImg->height]; //각 행별로 값누적 수평투영
		double *binaryImgVerticalNum = new double[binaryImg->width]; //각 열별로 값누적 수직투영

		memset(binaryImgHorizonNum, 0, (binaryImg->height)*sizeof(double));
		memset(binaryImgVerticalNum, 0, (binaryImg->width)*sizeof(double));

		//cvShowImage("Original", originalImg);		//원본이미지
		//cvShowImage("threshold", binaryImg);		//이진화이미지


		int sum = 0;
		int sum1 = 0;

		//픽셀데이터 저장
		for (int i = 0; i < binaryImg->height; i++) {
			for (int j = 0; j < binaryImg->width; j++) {
				binaryImgMatrix[i][j] = cvGetReal2D(binaryImg, i, j);
			}
		}


		//======================이미지 전체에대한 수평수직 프로젝션Start====================================
		//행렬 단위로 0기준으로 horizontal projection 
		for (int i = 0; i < binaryImg->height; i++) {
			for (int j = 0; j < binaryImg->width; j++) {
				if (binaryImgMatrix[i][j] == 0)
					binaryImgHorizonNum[i]++;
			}
		}
		//행렬 단위로 0기준으로 vertical projection
		for (int j = 0; j < binaryImg->width; j++) {
			for (int i = 0; i < binaryImg->height; i++) {
				if (binaryImgMatrix[i][j] == 0)
					binaryImgVerticalNum[j]++;
			}
		}

		//====================================수평수직 프로젝션End====================================



		//====================================최소인접사각형(수직투영) Start==============================
		/*
		vercical projection 을 통해 문자열에서의 문자단위로 구분하고 해당 x좌표를 통해
		horizontal projection을  시행한다.
		*/

		Rectangle rectangle;
		bool flag = false;

		for (int i = 0; i < binaryImg->width; i++) {
			//첫번째로 만나는 지점의 x좌표 저장
			if (binaryImgVerticalNum[i] != 0 && flag == false) {
				rectangle.xL = i;
				flag = true;
			}
			//마지막으로 만나는 지점의 x좌표 저장
			else if (binaryImgVerticalNum[i] == 0 && flag == true) {
				rectangle.xR = i;
				flag = false;
			}
		}

		//wordqueue가 empty인경우 예외처리 해줘야함 

		flag = false;
		int tempCount = 0;
		bool  findFlag = false;
		for (int j = 0; j < binaryImg->height; j++) {
			//tempCount = 0;
			findFlag = false;
			for (int k = rectangle.xL; k <= rectangle.xR; k++) {
				if (binaryImgMatrix[j][k] == 0) { //검은점을 만나면 즉 문자의 시작점을 만나면
												  //tempCount++;
					findFlag = true;
					break;
				}
			}
			//첫 시작점을 찾았으면 해당 높이를 yT에 저장
			if (findFlag == true && flag == false) {
				rectangle.yT = j;
				flag = true;
			}
			//마지막 점을 찾았으면 해당 높이를 yB에 저장 
			else if (findFlag == false && flag == true) {
				rectangle.yB = j - 1;
				flag = false;
			}
			if (j == binaryImg->height - 1 && flag == true) {
				rectangle.yB = j;
			}
		}

		//각 문자에 대한 중심좌표 계산
		rectangle.calcCenter();
		//=========================최소인접사각형 그림으로 나타낸것 Start============================
		//Queue로 구현한게 문제 리스트로 바꿔서 next를 쓰자 
		IplImage * minRectangle;
		minRectangle = cvCreateImage(cvSize(rectangle.xR - rectangle.xL, rectangle.yB - rectangle.yT), 8, 1);
		for (int i = rectangle.yT; i < rectangle.yB; ++i) {
			for (int j = rectangle.xL; j < rectangle.xR; ++j) {
				cvSetReal2D(minRectangle, i - rectangle.yT, j - rectangle.xL, 255);
			}
		}

		for (int i = rectangle.yT; i < rectangle.yB; ++i) {
			for (int j = rectangle.xL; j < rectangle.xR; ++j) {
				if (binaryImgMatrix[i][j] == 0)
					cvSetReal2D(minRectangle, i - rectangle.yT, j - rectangle.xL, 0);
			}
		}
		//=========================최소인접사각형 그림으로 나타낸것 End============================

		//문자를 10배로 늘리는 작업(원형백터시 정확도를 높히기 위함)
		int tempExtendWidth, tempExtendHeight;
		bool **tempExtendRectangle = NULL;
		tempExtendHeight = rectangle.yB - rectangle.yT;
		tempExtendWidth = rectangle.xR - rectangle.xL;

		tempExtendRectangle = new bool*[tempExtendHeight*EXTEND_RATIO];
		for (int i = 0; i < tempExtendHeight*EXTEND_RATIO; i++) {
			tempExtendRectangle[i] = new bool[(tempExtendWidth*EXTEND_RATIO)];
		}
		for (int i = 0; i < tempExtendHeight*EXTEND_RATIO; ++i) {
			for (int j = 0; j < tempExtendWidth*EXTEND_RATIO; ++j) {
				tempExtendRectangle[i][j] = false;
			}
		}
		int tempExtendi = 0;
		int tempExtendj = 0;
		for (int i = 0; i < tempExtendHeight; ++i) {
			for (int j = 0; j < tempExtendWidth; ++j) {
				if (cvGetReal2D(minRectangle, i, j) == 0) {
					tempExtendi = i * EXTEND_RATIO;
					tempExtendj = j * EXTEND_RATIO;
					for (int a = 0; a < EXTEND_RATIO; ++a) {
						for (int b = 0; b < EXTEND_RATIO; ++b) {
							tempExtendRectangle[tempExtendi + a][tempExtendj + b] = true;
						}
					}

				}
			}
		}
		//====================================최소인접사각형(수평투영) End==============================

		IplImage * extendRectangle;
		extendRectangle = cvCreateImage(cvSize(tempExtendWidth * EXTEND_RATIO, tempExtendHeight * EXTEND_RATIO), 8, 1);
		for (int i = 0; i < tempExtendHeight*EXTEND_RATIO; ++i) {
			for (int j = 0; j < tempExtendWidth*EXTEND_RATIO; ++j) {
				if (tempExtendRectangle[i][j] == true) {
					cvSetReal2D(extendRectangle, i, j, 0);
				}
				else {
					cvSetReal2D(extendRectangle, i, j, 255);
				}

			}
		}
		//====================================원형패턴백터 Start==============================
		//x=r sin 세타  y = r cos 세타
		//
		int circularNumber = 3;	//생성할 원형의 개수    개수가 많을수록 촘촘히검사
		int checkRadian = 3;	//체크할 라디안 간격    간격이 좁을수록 촘촘히검사  일반적으로 1~3도 사용
								//임시 예외처리 1,2,3 도만 사용할것임
		if (checkRadian < 1 || checkRadian >3) {
			checkRadian = 3;
		}

		//원형패턴백터를 이용하여 일치하는 점의 각도를 저장할 배열 생성
		bool **radianMatrix = new bool*[circularNumber];
		for (int i = 0; i < circularNumber; i++) {
			radianMatrix[i] = new bool[(360 / checkRadian)];
		}

		for (int i = 0; i < circularNumber; ++i) {
			for (int j = 0; j < (360 / checkRadian); ++j) {
				radianMatrix[i][j] = false;
			}
		}


		int tempCenterX = (tempExtendWidth*EXTEND_RATIO) / 2;
		int tempCenterY = (tempExtendHeight*EXTEND_RATIO) / 2;
		int minCenter = min(tempCenterX, tempCenterY);

		//각 반지름 길이 
		int *radius = new int[circularNumber];
		for (int i = 0; i < circularNumber; ++i) {
			radius[i] = (i + 1)*(minCenter / 3);
		}
		int tempX = 0;
		int tempY = 0;
		int count = 0;

		//원형패턴 이용 좌표통한 포인트 확인 및 배열저장
		for (int i = 360; i > 0;) {
			for (int j = 0; j < 3; ++j) {
				tempX = tempCenterX + radius[j] * cos(i*PI);
				tempY = tempCenterY + radius[j] * sin(i*PI);
				//해당좌표에 포인트가 있으면
				if (tempExtendRectangle[tempY][tempX] == true) {

					//원형 view test지워지워
					cvSetReal2D(extendRectangle, tempY, tempX, 255);
					radianMatrix[j][count] = true;
					//	cout << tempX << " " << tempY <<"  <---"<< endl;
				}
				else {
					//원형 view test 지워지워
					cvSetReal2D(extendRectangle, tempY, tempX, 0);
					//	cout << tempX << " " << tempY << endl;
				}
			}
			count++;
			i -= checkRadian;
		}

		//확장이미지 확인
		cvShowImage("extendRectangle", extendRectangle);

		//배열 간략화 3개 옵셋을 기준으로 줄인다
		int offset = 3;
		wordCharacter wordcahracter(circularNumber, checkRadian, offset);

		for (int j = 0; j < circularNumber; ++j) {
			count = 0;
			for (int i = 0; i < 120;) {
				if (radianMatrix[j][i] + radianMatrix[j][i + 1] + radianMatrix[j][i + 2] >= 2) {
					wordcahracter.radianMatrix[j][count] = true;
				}
				else {
					wordcahracter.radianMatrix[j][count] = false;
				}
				i += 3;
				count++;
			}
		}
		wordList.push_back(wordcahracter);


	}

	list<wordCharacter>::iterator it;
	for (it = wordList.begin(); it != wordList.end(); ++it) {
		for (int j = 0; j < 3; ++j) {
			for (int i = 1; i <= 40; ++i) {
				if (it->radianMatrix[j][i - 1] == true) {
					cout << "1";
				}
				else {
					cout << "0";
				}
				if (i % 10 == 0 && i != 0) {
					cout << " ";
				}
			}
			cout << endl;
		}
		cout << endl;
	}



	//====================================원형패턴백터 End==============================


	cvWaitKey(0);

	//cvReleaseImage(&originalImg);
	//cvReleaseImage(&imgHistogram);


	return 0;


}
