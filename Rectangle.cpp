//Rectangle.cpp
#include "Rectangle.h"


Rectangle::Rectangle() {
	centerX = centerY = xLeft = xRight = yTop = yBottom = 0;
}

Rectangle::Rectangle(int radian, int circularNumber) {
	centerX = centerY = xLeft = xRight = yTop = yBottom = 0;
	**radianMatrix = new bool*[circularNumber];
	for (int i = 0; i < circularNumber; i++) {
		radianMatrix[i] = new bool[(360 / radian)];
	}
}

void Rectangle::clearRectangle() {
	centerX = centerY = xLeft = xRight = yTop = yBottom = 0;
}

void Rectangle::calcCenter() {
	centerX = ((xRight - xLeft) / 2) + xLeft;
	centerY = ((yBottom - yTop) / 2) + yTop;
}

int Rectangle::getXLeft() {
	return xLeft;
}
int Rectangle::getXRight() {
	return xRight;
}
int Rectangle::getYTop() {
	return yTop;
}
int Rectangle::getYBottom() {
	return yBottom;
}
int Rectangle::getCenterX() {
	return centerX;
}
int Rectangle::getCenterY() {
	return centerY;
}

void Rectangle::setXLeft(int xLeft) {
	this->xLeft = xLeft;
}
void Rectangle::setXRight(int xRight) {
	this->xRight = xRight;
}
void Rectangle::setYTop(int yTop) {
	this->yTop = yTop;
}
void Rectangle::setYBottom(int yBottom) {
	this->yBottom = yBottom;
}
void Rectangle::setCenterX(int centerX) {
	this->centerX = centerX;
}
void Rectangle::setCenterY(int centerY) {
	this->centerY = centerY;
}

Rectangle::~Rectangle()
{
}
