#pragma once

#ifndef MCVersion_H
#define MCVersion_H

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
#include <regex>

enum VersionCategory {
    DOWN_115,    // 1.17.x
    IS_116,    // 1.17.x
    IS_117,
    UP_118_20,      // 1.18 이상
    UP_21,      // 1.18 이상
};

int MC(int ram, std::string subver, std::string url); //1.21.~
int Ver(std::string ver, int ram);
VersionCategory getVersionCategory(const std::string& filename);

#endif