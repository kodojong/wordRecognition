//Rectangle.h
#pragma once

class Rectangle {

private:
	int xLeft, xRight; //x의 좌,우좌표
	int yTop, yBottom; //y의 상,하좌표
	int centerX, centerY; //중심위치
	bool **radianMatrix;

public:
	Rectangle();
	~Rectangle();
	Rectangle(int radian, int circularNumber);
	void clearRectangle();
	//중심좌표 계산
	void calcCenter();


	void setXLeft(int);
	void setXRight(int);
	void setYTop(int);
	void setYBottom(int);
	void setCenterX(int);
	void setCenterY(int);

	int getXLeft();
	int getXRight();
	int getYTop();
	int getYBottom();
	int getCenterX();
	int getCenterY();
};

