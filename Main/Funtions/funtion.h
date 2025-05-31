#pragma once

#ifndef function_H
#define function_H

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

void setTextColor(int color);

void ending();

void Error(int color, std::string message);  // 함수 선언

#endif