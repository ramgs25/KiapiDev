#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "findid.h"

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[256];

    // COM ��Ʈ ����
    hSerial = CreateFileW(L"COM5", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    // �ø��� ��Ʈ ����
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;   // ���� �ӵ� ����
    dcbSerialParams.ByteSize = 8;          // ������ ��Ʈ ����
    dcbSerialParams.StopBits = ONESTOPBIT;  // ���� ��Ʈ ����
    dcbSerialParams.Parity = NOPARITY;      // �и�Ƽ ����

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    // Ÿ�Ӿƿ� ����
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

    // ������ ���������� �б�
    while (1) {
        DWORD bytesRead;
        if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, NULL)) {
            if (bytesRead > 0) {
                //printf("\nRead %lu bytes from serial port: \n%.*s\n", bytesRead, bytesRead, buffer);
                if (strstr(buffer, "$GNRMC") != NULL) {
                    //printSubstringAfterMatch(buffer, "$GNRMC");
                    findGNRMC(buffer, "$GNRMC", '$');
                }
                //printSubstringAfterMatch(buffer, "$GNRMC"); // "printsam.h���� ���ǵ� �Լ��� ���"
                else
                    continue;
            }
        }
        else {
            fprintf(stderr, "Error reading from serial port\n");
            break;
        }
        // �����͸� �߰��� �б� ���� �� �� ���� �����ʹ� ����
        //memset(buffer, 0, sizeof(buffer));
    }

    // COM ��Ʈ �ݱ�
    CloseHandle(hSerial);
  
    return 0;
}