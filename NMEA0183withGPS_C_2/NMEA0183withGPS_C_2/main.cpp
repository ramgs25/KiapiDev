#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "findid.h"

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[256];

    // COM 포트 열기
    hSerial = CreateFileW(L"COM5", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    // 시리얼 포트 설정
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;   // 전송 속도 설정
    dcbSerialParams.ByteSize = 8;          // 데이터 비트 설정
    dcbSerialParams.StopBits = ONESTOPBIT;  // 정지 비트 설정
    dcbSerialParams.Parity = NOPARITY;      // 패리티 설정

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // 타임아웃 설정
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

    // 데이터 지속적으로 읽기
    while (1) {
        DWORD bytesRead;
        if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
            if (bytesRead > 0) {
                //printf("\nRead %lu bytes from serial port: \n%.*s\n", bytesRead, bytesRead, buffer);
                if (strstr(buffer, "$GNRMC") != NULL) {
                    //printSubstringAfterMatch(buffer, "$GNRMC");
                    findGNRMC(buffer, "$GNRMC", '$');
                }
                //printSubstringAfterMatch(buffer, "$GNRMC"); // "printsam.h에서 정의된 함수를 사용"
                else
                    continue;
            }
        }
        else {
            fprintf(stderr, "Error reading from serial port\n");
            break;
        }
        // 데이터를 추가로 읽기 위해 한 번 읽은 데이터는 비우기
        //memset(buffer, 0, sizeof(buffer));
    }

    // COM 포트 닫기
    CloseHandle(hSerial);
  
    return 0;
}