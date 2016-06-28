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
	cvThreshold(originalImg, binaryImg, 200, 255, CV_THRESH_BINARY); //dst를 이진화 하여 dst에 넣음


																	 //히스토그램 위한 가로,세로열 테이블 생성
	int *ImgHorizonNum = new int[binaryImg->height]; //각 행별로 값누적 수평투영
	int *ImgVerticalNum = new int[binaryImg->width]; //각 열별로 값누적 수직투영
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

	//이미지에서 각 문자열의 위치를 LIst에 저장
	for (int i = 0; i < binaryImg->height; i++) {
		//첫번째로 만나는 지점의 y좌표 저장
		if (ImgHorizonNum[i] != 0 && flag == false) {
			rectangle.setYTop(i);
			flag = true;
		}
		//마지막으로 만나는 지점의 y좌표 저장
		else if (ImgHorizonNum[i] == 0 && flag == true) {
			rectangle.setYBottom(i);
			flag = false;
			stringList.push_back(rectangle);
			rectangle.clearRectangle();	//반복문에서 연속적으로 컬렉션에 넣기위함
		}
	}

	//stringList에서 각 문자로 분리
	 ImgHorizonNum = new int[binaryImg->height]; //각 행별로 값누적 수평투영
	ImgVerticalNum = new int[binaryImg->width]; //각 열별로 값누적 수직투영
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
