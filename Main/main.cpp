#include <stdio.h>
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
#include <cstdlib>  // system() 함수 사용을 위한 헤더
#include "Funtions/funtion.h"
#include "Start/MinecraftVersion.h"

int main() {
    std::string MinecraftServerVerValue = "1.21";
    int MinecraftServerBat = 1;

    std::cout << "사용할 마인크래프트 서버 버전을 입력해주세요.: ";
    std::cin >> MinecraftServerVerValue;

    if (std::cin.fail()) {
        Error(12, "유효하지 않은 string 값입니다.");
        return -1;
    }

    std::cout << "램 용량을 입력해주세요.(단위는 G입니다.): ";
    std::cin >> MinecraftServerBat;

    if (std::cin.fail()) {
        Error(12, "유효하지 않은 int 값입니다.");
        return -1;
    }

    Ver(MinecraftServerVerValue, MinecraftServerBat);
}