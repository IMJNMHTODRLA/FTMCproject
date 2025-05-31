#include "funtion.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <string>
#include <iostream>
#include <cctype>  // isdigit 함수가 포함된 헤더
#include <string>  // 이 헤더 파일이 필요합니다.
#include <limits>  // std::numeric_limits
#include <conio.h>  // _kbhit(), _getch()

void setTextColor(int color) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // 콘솔 핸들 얻기
	SetConsoleTextAttribute(hConsole, color);  // 색상 변경
}

void ending() {
	setTextColor(10);
	std::cout << "Enter 키를 눌러 종료..." << std::endl;

	char key;
	do {
		key = _getch();  // 키 입력 대기
	} while (key != '\r' && key != '\n');  // 'q'를 누르면 종료
}

void Error(int color, std::string message) {
	setTextColor(color);
	std::cout << "\n" << message << std::endl;
	ending();
}