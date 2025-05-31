#include "MinecraftVersion.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <urlmon.h>
#include "Funtions/funtion.h"
#include <regex>
#include <chrono>
#include <thread>
#pragma comment(lib, "urlmon.lib")

namespace fs = std::filesystem;

VersionCategory getVersionCategory(const std::string& filename) {
    std::regex versionPattern(R"((\d+)\.(\d+)(?:\.(\d+))?\.jar)");
    std::smatch match;

    if (std::regex_search(filename, match, versionPattern)) {
        int minor = std::stoi(match[2].str());
        int patch = match[3].length() > 0 ? std::stoi(match[3].str()) : 0;  // 패치 버전 (없으면 0으로 설정)

        // 1.14 ~ 1.15.1까지는 DOWN_115
        if (minor == 14) return DOWN_115;  // 1.14 버전
        if (minor == 15 && patch <= 1) return DOWN_115;  // 1.15.1 이하

        // 1.15.2 ~ 1.16.5까지는 IS_116
        if (minor == 15 && patch >= 2) return IS_116;  // 1.15.2 이상
        if (minor == 16 && patch <= 5) return IS_116;  // 1.16.5 이하

        if (minor == 17) return IS_117;  // 1.17 인가
        if (minor >= 18 && minor <= 20) return UP_118_20;    // 1.18 이상
        if (minor >= 21) return UP_21;    // 1.18 이상
    }
}

int MC(int ram, std::string subver, std::string url) {
    fs::path currentPath = fs::current_path();

    // 다운로드할 파일 저장 경로 (부모 디렉토리에 저장)
    std::string tempFilePath = (currentPath / "IlIlIllIllIIllIIllIIlllIIIlllIIIllIIIIllIIlll.tmp").string();
    std::string destinationPath = (currentPath / subver).string();
    //https://api.papermc.io/v2/projects/paper/versions/1.21.4/builds/185/downloads/paper-1.21.4-185.jar

    // 파일 다운로드
    HRESULT hr = URLDownloadToFileA(NULL, url.c_str(), tempFilePath.c_str(), 0, NULL);
    if (FAILED(hr)) {
        Error(12, "파일 다운로드에 실패하였습니다.");
        return -1;
    }

    setTextColor(10);
    std::cout << "파일 다운로드 완료: " << tempFilePath << std::endl;

    // 파일 이름 변경
    fs::rename(tempFilePath, destinationPath);
    std::cout << "파일 이름 변경 성공: " << destinationPath << std::endl;

    // 상위 디렉토리에 파일 생성
    std::string filePath = (currentPath / "start.bat").string();

    // 파일 생성 및 쓰기
    std::ofstream outFile(filePath);
    if (outFile) {
        switch (getVersionCategory(subver)) {
        case DOWN_115:
            // 배치 파일 작성

            outFile << "@echo off" << std::endl;
            outFile << std::endl;
            outFile << "SET JDK_PATH_64=\"C:\\Program Files\\Java\"" << std::endl;
            outFile << "SET JDK_PATH_32=\"C:\\Program Files (x86)\\Java\"" << std::endl;
            outFile << "for /d %%i in (%JDK_PATH_64%\\jdk1.8.0_*) do (" << std::endl;
            outFile << "    if exist \"%%i\\bin\\java.exe\" (" << std::endl;
            outFile << "        SET JAVA_EXE=%%i\\bin\\java.exe" << std::endl;
            outFile << "        echo 64비트 JDK 발견: %%i\\bin\\java.exe" << std::endl;
            outFile << "        goto :startMinecraft" << std::endl;
            outFile << "    )" << std::endl;
            outFile << ")" << std::endl;
            outFile << "for /d %%i in (%JDK_PATH_32%\\jdk1.8.0_*) do (" << std::endl;
            outFile << "    if exist \"%%i\\bin\\java.exe\" (" << std::endl;
            outFile << "        SET JAVA_EXE=%%i\\bin\\java.exe" << std::endl;
            outFile << "        echo 32비트 JDK 발견: %%i\\bin\\java.exe" << std::endl;
            outFile << "        goto :startMinecraft" << std::endl;
            outFile << "    )" << std::endl;
            outFile << ")" << std::endl;
            outFile << "echo 자바 경로를 찾을 수 없습니다. 8버전 JDK를 설치하십시오." << std::endl;
            outFile << "pause" << std::endl;
            outFile << "exit /b" << std::endl;
            outFile << std::endl;
            outFile << ":startMinecraft" << std::endl;
            // ram과 subver 변수 사용
            outFile << "\"%JAVA_EXE%\" -Xms" << ram << "G -Xmx" << ram << "G -jar " << subver << std::endl;
            outFile << "pause" << std::endl;
            outFile << "" << std::endl;

            setTextColor(10);
            std::cout << "JVM 생성 완료(일반, 1.15이하 용): " << filePath << std::endl;
            outFile.close();
            break;
        case IS_116:
            // 배치 파일 작성

            outFile << "@echo off" << std::endl;
            outFile << std::endl;
            outFile << "SET JDK_PATH_64=\"C:\\Program Files\\Java\"" << std::endl;
            outFile << "SET JDK_PATH_32=\"C:\\Program Files (x86)\\Java\"" << std::endl;
            outFile << "for /d %%i in (%JDK_PATH_64%\\jdk1.8.0_*) do (" << std::endl;
            outFile << "    if exist \"%%i\\bin\\java.exe\" (" << std::endl;
            outFile << "        SET JAVA_EXE=%%i\\bin\\java.exe" << std::endl;
            outFile << "        echo 64비트 JDK 발견: %%i\\bin\\java.exe" << std::endl;
            outFile << "        goto :startMinecraft" << std::endl;
            outFile << "    )" << std::endl;
            outFile << ")" << std::endl;
            outFile << "for /d %%i in (%JDK_PATH_32%\\jdk1.8.0_*) do (" << std::endl;
            outFile << "    if exist \"%%i\\bin\\java.exe\" (" << std::endl;
            outFile << "        SET JAVA_EXE=%%i\\bin\\java.exe" << std::endl;
            outFile << "        echo 32비트 JDK 발견: %%i\\bin\\java.exe" << std::endl;
            outFile << "        goto :startMinecraft" << std::endl;
            outFile << "    )" << std::endl;
            outFile << ")" << std::endl;
            outFile << "echo 자바 경로를 찾을 수 없습니다. 8버전 JDK를 설치하십시오." << std::endl;
            outFile << "pause" << std::endl;
            outFile << "exit /b" << std::endl;
            outFile << std::endl;
            outFile << ":startMinecraft" << std::endl;
            // ram과 subver 변수 사용
            outFile << "\"%JAVA_EXE%\" -Xms" << ram << "G -Xmx" << ram << "G -jar " << subver << " -nogui" << std::endl;
            outFile << "pause" << std::endl;
            outFile << "" << std::endl;

            setTextColor(10);
            std::cout << "JVM 생성 완료(일반, 1.16 용): " << filePath << std::endl;
            outFile.close();
            break;
        case IS_117:
            // 배치 파일 작성

            outFile << "@echo off" << std::endl;
            outFile << std::endl;
            outFile << "SET JDK_PATH_64=\"C:\\Program Files\\Java\"" << std::endl;
            outFile << "SET JDK_PATH_32=\"C:\\Program Files (x86)\\Java\"" << std::endl;
            outFile << "if exist \"%JDK_PATH_64%\\jdk-16\\bin\\java.exe\" (" << std::endl; // 64비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_64%\\jdk-16\\bin\\java.exe" << std::endl;
            outFile << "    echo 64비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            outFile << "if exist \"%JDK_PATH_32%\\jdk-16\\bin\\java.exe\" (" << std::endl; // 32비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_32%\\jdk-16\\bin\\java.exe" << std::endl;
            outFile << "    echo 32비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            outFile << "echo 자바 경로를 찾을 수 없습니다. 16버전 JDK를 설치하십시오." << std::endl;
            outFile << "pause" << std::endl;
            outFile << "exit /b" << std::endl;
            outFile << std::endl;

            outFile << ":startMinecraft" << std::endl;

            // ram과 subver 변수 사용
            outFile << "%JAVA_EXE% -Xms" << ram << "G -Xmx" << ram << "G -jar " << subver << " -nogui" << std::endl;
            outFile << "pause" << std::endl;
            outFile << "" << std::endl;

            setTextColor(10);
            std::cout << "JVM 생성 완료(일반, 1.17 용): " << filePath << std::endl;
            outFile.close();
            break;
        case UP_118_20:
            outFile << "@echo off" << std::endl;
            outFile << std::endl;
            outFile << "SET JDK_PATH_64=\"C:\\Program Files\\Java\"" << std::endl;
            outFile << "SET JDK_PATH_32=\"C:\\Program Files (x86)\\Java\"" << std::endl;
            outFile << "if exist \"%JDK_PATH_64%\\jdk-17\\bin\\java.exe\" (" << std::endl; // 64비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_64%\\jdk-17\\bin\\java.exe" << std::endl;
            outFile << "    echo 64비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            outFile << "if exist \"%JDK_PATH_32%\\jdk-17\\bin\\java.exe\" (" << std::endl; // 32비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_32%\\jdk-17\\bin\\java.exe" << std::endl;
            outFile << "    echo 32비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            //
            outFile << "echo 자바 경로를 찾을 수 없습니다. 17버전 JDK를 설치하십시오." << std::endl;
            outFile << "pause" << std::endl;
            outFile << "exit /b" << std::endl;
            outFile << std::endl;
            outFile << ":startMinecraft" << std::endl;
            outFile << "%JAVA_EXE% -Xms" << ram << "G -Xmx" << ram << "G -jar " << "-XX:+UseG1GC -XX:+ParallelRefProcEnabled -XX:MaxGCPauseMillis=130 -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+DisableExplicitGC -XX:+AlwaysPreTouch -XX:G1NewSizePercent=28 -XX:G1MaxNewSizePercent=40 -XX:G1HeapRegionSize=16M -XX:G1ReservePercent=20 -XX:G1MixedGCCountTarget=3 -XX:InitiatingHeapOccupancyPercent=10 -XX:G1MixedGCLiveThresholdPercent=90 -XX:G1RSetUpdatingPauseTimePercent=0 -XX:SurvivorRatio=32 -XX:+PerfDisableSharedMem -XX:G1SATBBufferEnqueueingThresholdPercent=30 -XX:G1ConcMarkStepDurationMillis=5.0 -XX:G1ConcRSHotCardLimit=16 -XX:MaxTenuringThreshold=1 -XX:G1ConcRefinementServiceIntervalMillis=150 -XX:UseAVX=3 -XX:+UseStringDeduplication -XX:+UseFastUnorderedTimeStamps -XX:+UseAES -XX:GCTimeRatio=99 -XX:+UseAESIntrinsics -XX:UseSSE=4 -XX:+UseFMA -XX:AllocatePrefetchStyle=3 -XX:+UseLoopPredicate -XX:+RangeCheckElimination -XX:+EliminateLocks -XX:+DoEscapeAnalysis -XX:+UseCodeCacheFlushing -XX:+SegmentedCodeCache -XX:+UseFastJNIAccessors -XX:+OptimizeStringConcat -XX:+UseCompressedOops -XX:+UseThreadPriorities -XX:+OmitStackTraceInFastThrow -XX:+TrustFinalNonStaticFields -XX:ThreadPriorityPolicy=1 -XX:+UseInlineCaches -XX:+RewriteBytecodes -XX:+RewriteFrequentPairs -XX:+UseNUMA -XX:-DontCompileHugeMethods -XX:+UseFPUForSpilling -XX:+UseFastStosb -XX:+UseNewLongLShift -XX:+UseVectorCmov -XX:+UseXMMForArrayCopy -XX:+UseXmmI2D -XX:+UseXmmI2F -XX:+UseXmmLoadAndClearUpper -XX:+UseXmmRegToRegMoveAll -XX:+UseLargePages -XX:LargePageSizeInBytes=2m -Xlog:gc+init -Dfile.encoding=UTF-8 -Xlog:async -Djava.security.egd=file:/dev/urandom -XX:NmethodSweepActivity=1 -XX:+AlwaysActAsServerClassMachine -XX:ReservedCodeCacheSize=400M -XX:NonNMethodCodeHeapSize=12M -XX:ProfiledCodeHeapSize=194M -XX:NonProfiledCodeHeapSize=194M -XX:MaxNodeLimit=240000 -XX:NodeLimitFudgeFactor=8000 -XX:+UseCriticalJavaThreadPriority --add-modules jdk.incubator.vector " << subver << " -nogui" << std::endl;
            setTextColor(10);
            std::cout << "JVM 생성 완료(최적화, 1.18이상 1.20이하 용): " << filePath << std::endl;
            outFile.close();
            break;
        case UP_21:
            outFile << "@echo off" << std::endl;
            outFile << std::endl;
            outFile << "SET JDK_PATH_64=\"C:\\Program Files\\Java\"" << std::endl;
            outFile << "SET JDK_PATH_32=\"C:\\Program Files (x86)\\Java\"" << std::endl;
            outFile << "if exist \"%JDK_PATH_64%\\jdk-21\\bin\\java.exe\" (" << std::endl; // 64비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_64%\\jdk-21\\bin\\java.exe" << std::endl;
            outFile << "    echo 64비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            outFile << "if exist \"%JDK_PATH_32%\\jdk-21\\bin\\java.exe\" (" << std::endl; // 32비트 JDK 경로 확인
            outFile << "    SET JAVA_EXE=%JDK_PATH_32%\\jdk-21\\bin\\java.exe" << std::endl;
            outFile << "    echo 32비트 JDK 발견: \"%JAVA_EXE%\"" << std::endl;
            outFile << "    goto :startMinecraft" << std::endl;
            outFile << ")" << std::endl;
            outFile << "echo 자바 경로를 찾을 수 없습니다. 21버전 JDK를 설치하십시오." << std::endl;
            outFile << "pause" << std::endl;
            outFile << "exit /b" << std::endl;
            outFile << std::endl;
            outFile << ":startMinecraft" << std::endl;
            // ram과 subver 변수 사용
            outFile << "%JAVA_EXE% -Xms" << ram << "G -Xmx" << ram << "G -jar " << "-XX:+UseG1GC -XX:+ParallelRefProcEnabled -XX:MaxGCPauseMillis=130 -XX:+UnlockExperimentalVMOptions -XX:+UnlockDiagnosticVMOptions -XX:+DisableExplicitGC -XX:+AlwaysPreTouch -XX:G1NewSizePercent=28 -XX:G1MaxNewSizePercent=40 -XX:G1HeapRegionSize=16M -XX:G1ReservePercent=20 -XX:G1MixedGCCountTarget=3 -XX:InitiatingHeapOccupancyPercent=10 -XX:G1MixedGCLiveThresholdPercent=90 -XX:G1RSetUpdatingPauseTimePercent=0 -XX:SurvivorRatio=32 -XX:+PerfDisableSharedMem -XX:G1SATBBufferEnqueueingThresholdPercent=30 -XX:G1ConcMarkStepDurationMillis=5.0 -XX:G1ConcRSHotCardLimit=16 -XX:MaxTenuringThreshold=1 -XX:G1ConcRefinementServiceIntervalMillis=150 -XX:UseAVX=3 -XX:+UseStringDeduplication -XX:+UseFastUnorderedTimeStamps -XX:+UseAES -XX:GCTimeRatio=99 -XX:+UseAESIntrinsics -XX:UseSSE=4 -XX:+UseFMA -XX:AllocatePrefetchStyle=3 -XX:+UseLoopPredicate -XX:+RangeCheckElimination -XX:+EliminateLocks -XX:+DoEscapeAnalysis -XX:+UseCodeCacheFlushing -XX:+SegmentedCodeCache -XX:+UseFastJNIAccessors -XX:+OptimizeStringConcat -XX:+UseCompressedOops -XX:+UseThreadPriorities -XX:+OmitStackTraceInFastThrow -XX:+TrustFinalNonStaticFields -XX:ThreadPriorityPolicy=1 -XX:+UseInlineCaches -XX:+RewriteBytecodes -XX:+RewriteFrequentPairs -XX:+UseNUMA -XX:-DontCompileHugeMethods -XX:+UseFPUForSpilling -XX:+UseFastStosb -XX:+UseNewLongLShift -XX:+UseVectorCmov -XX:+UseXMMForArrayCopy -XX:+UseXmmI2D -XX:+UseXmmI2F -XX:+UseXmmLoadAndClearUpper -XX:+UseXmmRegToRegMoveAll -XX:+UseLargePages -XX:LargePageSizeInBytes=2m -Xlog:gc+init -Dfile.encoding=UTF-8 -Xlog:async -Djava.security.egd=file:/dev/urandom -XX:NmethodSweepActivity=1 -XX:+AlwaysActAsServerClassMachine -XX:ReservedCodeCacheSize=400M -XX:NonNMethodCodeHeapSize=12M -XX:ProfiledCodeHeapSize=194M -XX:NonProfiledCodeHeapSize=194M -XX:MaxNodeLimit=240000 -XX:NodeLimitFudgeFactor=8000 -XX:+UseCriticalJavaThreadPriority --add-modules jdk.incubator.vector " << subver << " -nogui" << std::endl;
            outFile << "pause" << std::endl;
            outFile << "" << std::endl;

            setTextColor(10);
            std::cout << "JVM 생성 완료(최적화, 1.21이상 용): " << filePath << std::endl;
            outFile.close();
            break;
        }
    }
    else {
        Error(12, "JVM 생성에 실패하였습니다.");
        return -1;
    }

    // 상위 디렉토리에 파일 생성
    std::string txtfilePath = (currentPath / "eula.txt").string();

    // 파일 생성 및 쓰기
    std::ofstream outFiletxt(txtfilePath);
    if (outFiletxt) {
        outFiletxt << "eula=true" << std::endl;
        outFiletxt.close();
        setTextColor(10);
        std::cout << "eula 자동 완성 완료: " << txtfilePath << std::endl;
    }
    else {
        Error(12, "eula 자동 완성에 실패 하였습니다.");
        return -1;
    }

    std::cout << "" << std::endl;
    ending();
    return 0;
}

/*std::string sourcePath = (currentPath / "resourse" / subver).string();  // 원본 파일 경로
    std::string destinationPath = (parentPath / subver).string();  // 목적지 경로 (file 디렉토리로 복사)

    try {
        fs::copy(sourcePath, destinationPath, fs::copy_options::overwrite_existing);
        setTextColor(10);
        std::cout << "\n파일 복사 성공: " << destinationPath << std::endl;
    }
    catch (const std::exception& e) {
        Error(12, "파일 복사중 오류가 발생했습니다.");
    }*/ //파일 복사 시스템(old)

int Ver(std::string ver, int ram) {

    std::string shit = "https://api.papermc.io/v2/projects/paper/versions/";

    if (ver == "1.21.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/185/downloads/paper-" + ver + "-185.jar");
    }
    else if (ver == "1.21.3") {
        MC(ram, ver + ".jar", shit + ver + "/builds/82/downloads/paper-" + ver + "-82.jar");
    }
    else if (ver == "1.21.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/132/downloads/paper-" + ver + "-132.jar");
    }
    else if (ver == "1.21") {
        MC(ram, ver + ".jar", shit + ver + "/builds/130/downloads/paper-" + ver + "-130.jar");
    }
    else if (ver == "1.20.6") {
        MC(ram, ver + ".jar", shit + ver + "/builds/151/downloads/paper-" + ver + "-151.jar");
    }
    else if (ver == "1.20.5") {
        MC(ram, ver + ".jar", shit + ver + "/builds/22/downloads/paper-" + ver + "-22.jar");
    }
    else if (ver == "1.20.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/499/downloads/paper-" + ver + "-499.jar");
    }
    else if (ver == "1.20.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/318/downloads/paper-" + ver + "-318.jar");
    }
    else if (ver == "1.20.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/151/downloads/paper-" + ver + "-151.jar");
    }
    else if (ver == "1.20") {
        MC(ram, ver + ".jar", shit + ver + "/builds/17/downloads/paper-" + ver + "-17.jar");
    }
    else if (ver == "1.19.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/550/downloads/paper-" + ver + "-550.jar");
    }
    else if (ver == "1.19.3") {
        MC(ram, ver + ".jar", shit + ver + "/builds/448/downloads/paper-" + ver + "-448.jar");
    }
    else if (ver == "1.19.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/307/downloads/paper-" + ver + "-307.jar");
    }
    else if (ver == "1.19.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/111/downloads/paper-" + ver + "-111.jar");
    }
    else if (ver == "1.19") {
        MC(ram, ver + ".jar", shit + ver + "/builds/81/downloads/paper-" + ver + "-81.jar");
    }
    else if (ver == "1.18.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/388/downloads/paper-" + ver + "-388.jar");
    }
    else if (ver == "1.18.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/216/downloads/paper-" + ver + "-216.jar");
    }
    else if (ver == "1.18") {
        MC(ram, ver + ".jar", shit + ver + "/builds/66/downloads/paper-" + ver + "-66.jar");
    }
    else if (ver == "1.17.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/411/downloads/paper-" + ver + "-411.jar");
    }
    else if (ver == "1.17") {
        MC(ram, ver + ".jar", shit + ver + "/builds/79/downloads/paper-" + ver + "-79.jar");
    }
    else if (ver == "1.16.5") {
        MC(ram, ver + ".jar", shit + ver + "/builds/794/downloads/paper-" + ver + "-794.jar");
    }
    else if (ver == "1.16.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/416/downloads/paper-" + ver + "-416.jar");
    }
    else if (ver == "1.16.3") {
        MC(ram, ver + ".jar", shit + ver + "/builds/253/downloads/paper-" + ver + "-253.jar");
    }
    else if (ver == "1.16.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/189/downloads/paper-" + ver + "-189.jar");
    }
    else if (ver == "1.16.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/138/downloads/paper-" + ver + "-138.jar");
    }
    else if (ver == "1.15.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/393/downloads/paper-" + ver + "-393.jar");
    }
    else if (ver == "1.15.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/62/downloads/paper-" + ver + "-62.jar");
    }
    else if (ver == "1.15") {
        MC(ram, ver + ".jar", shit + ver + "/builds/21/downloads/paper-" + ver + "-21.jar");
    }
    else if (ver == "1.14.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/245/downloads/paper-" + ver + "-245.jar");
    }
    else if (ver == "1.14.3") {
        MC(ram, ver + ".jar", shit + ver + "/builds/134/downloads/paper-" + ver + "-134.jar");
    }
    else if (ver == "1.14.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/107/downloads/paper-" + ver + "-107.jar");
    }
    else if (ver == "1.14.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/50/downloads/paper-" + ver + "-50.jar");
    }
    else if (ver == "1.14") {
        MC(ram, ver + ".jar", shit + ver + "/builds/17/downloads/paper-" + ver + "-17.jar");
    }
    else if (ver == "1.13.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/657/downloads/paper-" + ver + "-657.jar");
    }
    else if (ver == "1.13.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/386/downloads/paper-" + ver + "-386.jar");
    }
    else if (ver == "1.13") {
        MC(ram, ver + ".jar", shit + ver + "/builds/173/downloads/paper-" + ver + "-173.jar");
    }
    else if (ver == "1.12.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/1620/downloads/paper-" + ver + "-1620.jar");
    }
    else if (ver == "1.12.1") {
        MC(ram, ver + ".jar", shit + ver + "/builds/1204/downloads/paper-" + ver + "-1204.jar");
    }
    else if (ver == "1.12") {
        MC(ram, ver + ".jar", shit + ver + "/builds/1169/downloads/paper-" + ver + "-1169.jar");
    }
    else if (ver == "1.11.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/1106/downloads/paper-" + ver + "-1106.jar");
    }
    else if (ver == "1.10.2") {
        MC(ram, ver + ".jar", shit + ver + "/builds/918/downloads/paper-" + ver + "-918.jar");
    }
    else if (ver == "1.9.4") {
        MC(ram, ver + ".jar", shit + ver + "/builds/775/downloads/paper-" + ver + "-775.jar");
    }
    else if (ver == "1.8.8") {
        MC(ram, ver + ".jar", shit + ver + "/builds/445/downloads/paper-" + ver + "-445.jar");
    }
    else {
        Error(12, "지원하지 않거나 없는 버전입니다.");
        return -1;
    }
}
