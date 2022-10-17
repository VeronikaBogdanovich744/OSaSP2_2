#pragma once
#define _USE_MATH_DEFINES
#include <string>
#include "windows.h"
#include <math.h>
class CircleDrawer
{
	std::wstring str;
	int fontHeight;
	RECT windowSize;
	int circleRadius;
	int circleLength;
	int CircleX;
	int CircleY;

public:
	CircleDrawer(std::wstring str_) {
		str = str_;
		fontHeight = 0;
	}

	void DrawCircle(HDC hDc)
	{ 
		DrawCircleLine(hDc);
		setFont(hDc);
		PrintPhrase(hDc);
	}

	void SetClientSize(RECT clientSize)
	{
		windowSize.bottom = clientSize.bottom;
		windowSize.top = clientSize.top;
		windowSize.left = clientSize.left;
		windowSize.right = clientSize.right;
	}

private:
	void DrawCircleLine(HDC hDc) {
		RECT temp_shape;
		int length = min(abs(windowSize.bottom - windowSize.top), abs(windowSize.right - windowSize.left));
		circleRadius = length / 2;
		circleLength = 2 * circleRadius * M_PI;

		temp_shape.left = (windowSize.right - windowSize.left) / 2 - circleRadius;
		temp_shape.right = temp_shape.left + length;
		temp_shape.top = (windowSize.bottom - windowSize.top) / 2 - length / 2;
		temp_shape.bottom = temp_shape.top + length;

		CircleX = (windowSize.right - windowSize.left)/2;
		CircleY = (windowSize.bottom - windowSize.top) / 2;
		
		Ellipse(hDc, temp_shape.left, temp_shape.top, temp_shape.right, temp_shape.bottom);
	}

	void setFont(HDC hDc) {
		fontHeight = 70;
		
		RECT rect;
		rect.top = 0;
		rect.left = 0;
		auto font = CreateFont(fontHeight, 0, 0, 0, FW_NORMAL, false, false, false, 0, 0, 0, 0, FF_ROMAN, nullptr);
		SelectObject(hDc, font);
		DrawText(hDc, str.c_str(), str.length() , &rect, DT_CALCRECT);
		DeleteObject(font);
		auto letterWidth = rect.right / fontHeight;
		fontHeight = circleLength / letterWidth;
		fontHeight =  fontHeight / 2;
	}

	void PrintPhrase(HDC hDc) {

		RECT rect;
		double angle = 360.0 / (str.length() -1); 

		for (int i = 1; i < str.length()-1; i++) {
			RECT rect;

			double sin1 = sin(M_PI * 2 * angle * (i - 1) / 360.0);
			double cos1 = cos(M_PI * 2 * angle * (i - 1) / 360.0);
			rect.left = CircleX + (circleRadius - fontHeight) * sin1;
			rect.top = CircleY  + (circleRadius - fontHeight) * cos1;
			
			auto font = CreateFont(fontHeight, 0, angle * 10 * (i - 1), 0, FW_NORMAL, false, false, false, 0, 0, 0, 0, FF_ROMAN, nullptr);
			SelectObject(hDc, font);
			DrawText(hDc, str.c_str()+i, 1, &rect, NULL);
			DeleteObject(font);
		}
	}

};

