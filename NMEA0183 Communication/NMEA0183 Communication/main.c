#include <stdio.h>
#include <stdlib.h>
#include <stddef.h.>
#include <string.h>
#include <windows.h>
#include "idParsing.h"

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[256]= "\0";
    char* result = &buffer[0];

    //COM 포트 열기
    hSerial = CreateFileW(L"COM5", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    //시리얼 포트 설정
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;    //전송 속도 설정
    dcbSerialParams.ByteSize = 8;           //데이터 비트 설정
    dcbSerialParams.StopBits = ONESTOPBIT;  //정지 비트 설정
    dcbSerialParams.Parity = NOPARITY;      //패리티 설정

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    //타임아웃 설정
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }

    //데이터 지속적으로 읽기
    while (1) {
        DWORD bytesRead;
        //시리얼 통신을 통해 읽은 데이터가 있으면
        if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, '\0')) {
            if (bytesRead > 0) {
                //시리얼 통신을 통해 읽은 데이터 중 GNRMC 문장 찾기
                if (strstr(buffer, "$GNRMC") != '\0') {

                    //GNRMC 문장을 찾은 결과 저장
                    *result = malloc(sizeof(strstr((char*)"$GNRMC", '$')));
                    //GNRMC 문장을 찾은 결과가 있으면
                    if (*result != '\0') {
                        //문자열 끝에 널 문자 추가
                        result[strcspn(result, "\n")] = '\0';

                        //GNRMC 문장을 파싱하여 각 데이터 출력하는 idParsing 함수 호출(idParsing.h)
                        idParsing(result);
                    }
                    //GNRMC 문장을 찾은 결과가 없으면
                    else {
                        printf("GNRMC match sentence is not found.\n\n");
                    }
                }
                else
                    continue;
            }
        }
        //시리얼 통신을 통해 읽은 데이터가 없으면
        else {
            fprintf(stderr, "Error reading from serial port\n");
            break;
        }
    }

    //COM 포트 닫기
    CloseHandle(hSerial);

    return 0;
}