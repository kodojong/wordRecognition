//Rectangle.h
#pragma once

class Rectangle {

private:
	int xLeft, xRight; //x�� ��,����ǥ
	int yTop, yBottom; //y�� ��,����ǥ
	int centerX, centerY; //�߽���ġ
	bool **radianMatrix;

public:
	Rectangle();
	~Rectangle();
	Rectangle(int radian, int circularNumber);
	void clearRectangle();
	//�߽���ǥ ���
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

