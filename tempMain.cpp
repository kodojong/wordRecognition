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

//�ּ������簢���� ���ϱ�����
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
	int xL, xR; //x�� ��,����ǥ
	int yT, yB; //y�� ��,����ǥ
	int cX, cY; //�߽���ġ
	bool **radianMatrix;

	//�߽���ǥ ���
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

	for (int wordnum = 0; wordnum < 1; ++wordnum) { //�׽�Ʈ�� �ܾ� jpg����
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

		//cvThreshold �����̹��� ����ȭ
		cvThreshold(originalImg, binaryImg, 200, 255, CV_THRESH_BINARY); //dst�� ����ȭ �Ͽ� dst�� ����


																		 //2���� �ȼ����̺� ����
		double **binaryImgMatrix = new double*[binaryImg->height];
		for (int i = 0; i < binaryImg->height; i++) {
			binaryImgMatrix[i] = new double[binaryImg->width];
		}

		//������׷� ���� ����,���ο� ���̺� ����
		double *binaryImgHorizonNum = new double[binaryImg->height]; //�� �ະ�� ������ ��������
		double *binaryImgVerticalNum = new double[binaryImg->width]; //�� ������ ������ ��������

		memset(binaryImgHorizonNum, 0, (binaryImg->height)*sizeof(double));
		memset(binaryImgVerticalNum, 0, (binaryImg->width)*sizeof(double));

		//cvShowImage("Original", originalImg);		//�����̹���
		//cvShowImage("threshold", binaryImg);		//����ȭ�̹���


		int sum = 0;
		int sum1 = 0;

		//�ȼ������� ����
		for (int i = 0; i < binaryImg->height; i++) {
			for (int j = 0; j < binaryImg->width; j++) {
				binaryImgMatrix[i][j] = cvGetReal2D(binaryImg, i, j);
			}
		}


		//======================�̹��� ��ü������ ������� ��������Start====================================
		//��� ������ 0�������� horizontal projection 
		for (int i = 0; i < binaryImg->height; i++) {
			for (int j = 0; j < binaryImg->width; j++) {
				if (binaryImgMatrix[i][j] == 0)
					binaryImgHorizonNum[i]++;
			}
		}
		//��� ������ 0�������� vertical projection
		for (int j = 0; j < binaryImg->width; j++) {
			for (int i = 0; i < binaryImg->height; i++) {
				if (binaryImgMatrix[i][j] == 0)
					binaryImgVerticalNum[j]++;
			}
		}

		//====================================������� ��������End====================================



		//====================================�ּ������簢��(��������) Start==============================
		/*
		vercical projection �� ���� ���ڿ������� ���ڴ����� �����ϰ� �ش� x��ǥ�� ����
		horizontal projection��  �����Ѵ�.
		*/

		Rectangle rectangle;
		bool flag = false;

		for (int i = 0; i < binaryImg->width; i++) {
			//ù��°�� ������ ������ x��ǥ ����
			if (binaryImgVerticalNum[i] != 0 && flag == false) {
				rectangle.xL = i;
				flag = true;
			}
			//���������� ������ ������ x��ǥ ����
			else if (binaryImgVerticalNum[i] == 0 && flag == true) {
				rectangle.xR = i;
				flag = false;
			}
		}

		//wordqueue�� empty�ΰ�� ����ó�� ������� 

		flag = false;
		int tempCount = 0;
		bool  findFlag = false;
		for (int j = 0; j < binaryImg->height; j++) {
			//tempCount = 0;
			findFlag = false;
			for (int k = rectangle.xL; k <= rectangle.xR; k++) {
				if (binaryImgMatrix[j][k] == 0) { //�������� ������ �� ������ �������� ������
												  //tempCount++;
					findFlag = true;
					break;
				}
			}
			//ù �������� ã������ �ش� ���̸� yT�� ����
			if (findFlag == true && flag == false) {
				rectangle.yT = j;
				flag = true;
			}
			//������ ���� ã������ �ش� ���̸� yB�� ���� 
			else if (findFlag == false && flag == true) {
				rectangle.yB = j - 1;
				flag = false;
			}
			if (j == binaryImg->height - 1 && flag == true) {
				rectangle.yB = j;
			}
		}

		//�� ���ڿ� ���� �߽���ǥ ���
		rectangle.calcCenter();
		//=========================�ּ������簢�� �׸����� ��Ÿ���� Start============================
		//Queue�� �����Ѱ� ���� ����Ʈ�� �ٲ㼭 next�� ���� 
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
		//=========================�ּ������簢�� �׸����� ��Ÿ���� End============================

		//���ڸ� 10��� �ø��� �۾�(�������ͽ� ��Ȯ���� ������ ����)
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
		//====================================�ּ������簢��(��������) End==============================

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
		//====================================�������Ϲ��� Start==============================
		//x=r sin ��Ÿ  y = r cos ��Ÿ
		//
		int circularNumber = 3;	//������ ������ ����    ������ �������� �������˻�
		int checkRadian = 3;	//üũ�� ���� ����    ������ �������� �������˻�  �Ϲ������� 1~3�� ���
								//�ӽ� ����ó�� 1,2,3 ���� ����Ұ���
		if (checkRadian < 1 || checkRadian >3) {
			checkRadian = 3;
		}

		//�������Ϲ��͸� �̿��Ͽ� ��ġ�ϴ� ���� ������ ������ �迭 ����
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

		//�� ������ ���� 
		int *radius = new int[circularNumber];
		for (int i = 0; i < circularNumber; ++i) {
			radius[i] = (i + 1)*(minCenter / 3);
		}
		int tempX = 0;
		int tempY = 0;
		int count = 0;

		//�������� �̿� ��ǥ���� ����Ʈ Ȯ�� �� �迭����
		for (int i = 360; i > 0;) {
			for (int j = 0; j < 3; ++j) {
				tempX = tempCenterX + radius[j] * cos(i*PI);
				tempY = tempCenterY + radius[j] * sin(i*PI);
				//�ش���ǥ�� ����Ʈ�� ������
				if (tempExtendRectangle[tempY][tempX] == true) {

					//���� view test��������
					cvSetReal2D(extendRectangle, tempY, tempX, 255);
					radianMatrix[j][count] = true;
					//	cout << tempX << " " << tempY <<"  <---"<< endl;
				}
				else {
					//���� view test ��������
					cvSetReal2D(extendRectangle, tempY, tempX, 0);
					//	cout << tempX << " " << tempY << endl;
				}
			}
			count++;
			i -= checkRadian;
		}

		//Ȯ���̹��� Ȯ��
		cvShowImage("extendRectangle", extendRectangle);

		//�迭 ����ȭ 3�� �ɼ��� �������� ���δ�
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



	//====================================�������Ϲ��� End==============================


	cvWaitKey(0);

	//cvReleaseImage(&originalImg);
	//cvReleaseImage(&imgHistogram);


	return 0;


}
