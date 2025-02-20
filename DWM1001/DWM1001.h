#ifndef DWM1001_H
#define DWM1001_H

#include <Arduino.h>

// 최대 데이터 개수 정의
#define MAX_DATA 4

// 데이터 구조체 정의
struct PTD {
    char id[10];
    float x, y, z, d;
    double value[4];
};

class DWM1001 {
private:
    HardwareSerial& serialPort;
    
    int point_Count;
    String buffer;

public:
    PTD Point[MAX_DATA];

    // 생성자 (사용할 Serial 포트를 인자로 받음)
    DWM1001(HardwareSerial& port);

    // Serial 통신 시작
    void begin(long baudRate);

    // Serial 데이터 읽기
    int readSerialData();

    // 추출된 데이터 출력
    void printData();

    // 카운트
    int CountData();

    // 첫 시작 함수
    void InitialStep();

private:
    // 데이터 추출 함수
    int extractData(const char* input);

    int parseEstValues(const char* input, double* values, int* count);

    bool isValidEstBlock(const char* start, const char* end);
};

#endif // DWM1001_H
