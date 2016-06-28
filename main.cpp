//main.cpp
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <queue>
#include <list>
#include "Rectangle.h"

#define EXTEND_RATIO 5

using namespace std;

void main() {
	IplImage *originalImg = 0; //original image
	IplImage *imgHistogram = 0; //histgram of original image
	IplImage *binaryImg = 0; //original to binary image

							 //image Load and create "imaHistogram" and "binaryImg"
	originalImg = cvLoadImage("images\\number\\3.jpg", 0);
	imgHistogram = cvCreateImage(cvGetSize(originalImg), IPL_DEPTH_8U, 1);
	binaryImg = cvCreateImage(cvGetSize(originalImg), IPL_DEPTH_8U, 1);

	//Threshold = 200, set 255(white)
	cvThreshold(originalImg, binaryImg, 200, 255, CV_THRESH_BINARY); //dst�� ����ȭ �Ͽ� dst�� ����


																	 //������׷� ���� ����,���ο� ���̺� ����
	int *ImgHorizonNum = new int[binaryImg->height]; //�� �ະ�� ������ ��������
	int *ImgVerticalNum = new int[binaryImg->width]; //�� ������ ������ ��������
	memset(ImgHorizonNum, 0, (binaryImg->height)*sizeof(int));
	memset(ImgVerticalNum, 0, (binaryImg->width)*sizeof(int));

	//horizontal projection 
	for (int i = 0; i < binaryImg->height; i++) {
		for (int j = 0; j < binaryImg->width; j++) {
			if (cvGetReal2D(binaryImg, i, j) == 0)
				ImgHorizonNum[i]++;
		}
	}
	//vertical projection
	for (int j = 0; j < binaryImg->width; j++) {
		for (int i = 0; i < binaryImg->height; i++) {
			if (cvGetReal2D(binaryImg, i, j) == 0)
				ImgVerticalNum[j]++;
		}
	}

	/*
	//(test) horizontal projection check
	for (int j = 0; j < binaryImg->height; j++){
	cout << ImgHorizonNum[j] << endl;
	}
	//(test) vertical projection check
	for (int i = 0; i < binaryImg->width; i++){
	cout << ImgVerticalNum[i] << endl;

	}
	*/

	//view histogram
	IplImage *horizonProjView, *verticalProjView;
	horizonProjView = cvCreateImage(cvSize(binaryImg->width, binaryImg->height), 8, 1);
	verticalProjView = cvCreateImage(cvSize(binaryImg->width, binaryImg->height), 8, 1);

	//set histogram background 255(white)
	for (int i = 0; i < binaryImg->height; i++) {
		for (int j = 0; j < binaryImg->width; j++) {
			cvSetReal2D(horizonProjView, i, j, 255);
		}
	}
	for (int i = 0; i < binaryImg->height; i++) {
		for (int j = 0; j < binaryImg->width; j++) {
			cvSetReal2D(verticalProjView, i, j, 255);
		}
	}

	//set histogram point 0(black)
	int tempWidth = 0;
	for (int i = 0; i < binaryImg->height; i++) {
		tempWidth = binaryImg->width;
		for (int j = ImgHorizonNum[i]; j > 0; --j) {
			cvSetReal2D(horizonProjView, i, --tempWidth, 0);
		}
	}
	int tempHeight = 0;
	for (int i = 0; i < binaryImg->width; i++) {
		tempHeight = binaryImg->height;
		for (int j = ImgVerticalNum[i]; j > 0; --j) {
			cvSetReal2D(verticalProjView, --tempHeight, i, 0);
		}
	}

	//show histogram
	cvShowImage("Horizontal projection", horizonProjView);
	cvShowImage("Vertical projection", verticalProjView);


	list<Rectangle> stringList;	//set of string in Image
	list<Rectangle> wordList;	//set of word in string
	Rectangle rectangle;
	bool flag = false;

	//�̹������� �� ���ڿ��� ��ġ�� LIst�� ����
	for (int i = 0; i < binaryImg->height; i++) {
		//ù��°�� ������ ������ y��ǥ ����
		if (ImgHorizonNum[i] != 0 && flag == false) {
			rectangle.setYTop(i);
			flag = true;
		}
		//���������� ������ ������ y��ǥ ����
		else if (ImgHorizonNum[i] == 0 && flag == true) {
			rectangle.setYBottom(i);
			flag = false;
			stringList.push_back(rectangle);
			rectangle.clearRectangle();	//�ݺ������� ���������� �÷��ǿ� �ֱ�����
		}
	}

	//stringList���� �� ���ڷ� �и�
	 ImgHorizonNum = new int[binaryImg->height]; //�� �ະ�� ������ ��������
	ImgVerticalNum = new int[binaryImg->width]; //�� ������ ������ ��������
	memset(ImgHorizonNum, 0, (binaryImg->height)*sizeof(int));
	memset(ImgVerticalNum, 0, (binaryImg->width)*sizeof(int));

	for (list<Rectangle>::iterator it = stringList.begin(); it != stringList.end(); ++it) {
		//vertical projection

		for (int i = it->getXLeft(); i < it->getXRight(); i++) {
			for (int j = it->getYTop(); j < it->getYBottom(); j++) {
				if (cvGetReal2D(binaryImg, j, i) == 0)
					ImgVerticalNum[i]++;
			}
		}
	}
	for (int j = 0; j < binaryImg->width; j++) {
		for (int i = 0; i < binaryImg->height; i++) {
			if (cvGetReal2D(binaryImg, i, j) == 0)
				ImgVerticalNum[j]++;
		}
	}





	cvWaitKey(0);

}
