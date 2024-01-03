#include <stdio.h>
#include <stdlib.h>
#include <stddef.h.>
#include <string.h>
#include <windows.h>
//�� �ڵ� ���� #include "findid.h"
#include "idParsing.h"

int main() {
    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[256];
    char* result = &buffer[0];

    //COM ��Ʈ ����
    hSerial = CreateFileW(L"COM7", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening serial port\n");
        return 1;
    }

    //�ø��� ��Ʈ ����
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;    //���� �ӵ� ����
    dcbSerialParams.ByteSize = 8;           //������ ��Ʈ ����
    dcbSerialParams.StopBits = ONESTOPBIT;  //���� ��Ʈ ����
    dcbSerialParams.Parity = NOPARITY;      //�и�Ƽ ����

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    //Ÿ�Ӿƿ� ����
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

    //������ ���������� �б�
    while (1) {
        DWORD bytesRead;
        //�ø��� ����� ���� ���� �����Ͱ� ������
        if (ReadFile(hSerial, buffer, sizeof(buffer), &bytesRead, '\0')) {
            if (bytesRead > 0) {
                //�� �ڵ� ���� printf("\nRead %lu bytes from serial port: \n%.*s\n", bytesRead, bytesRead, buffer);
                //�ø��� ����� ���� ���� ������ �� GNRMC ���� ã��
                if (strstr(buffer, "$GNRMC") != '\0') {
                    //�� �ڵ� ���� printSubstringAfterMatch(buffer, "$GNRMC");
                    //�� �ּ� ���� GNRMC ���常�� ã�� ��ȯ�ϴ� findGNRMC �Լ� ȣ��(findID.h)
                    //�� �ڵ� ���� findGNRMC(buffer, "$GNRMC", '$');

                    //GNRMC ������ ã�� ��� ����
                    *result = malloc(sizeof(strstr((char*)"$GNRMC", '$')));
                    //GNRMC ������ ã�� ����� ������
                    if (*result != '\0') {
                        //���ڿ� ���� �� ���� �߰�
                        result[strcspn(result, "\n")] = '\0';
                        //�� �ּ� ���� ã�� ��ġ���� ���� NMEA0183 ID($XXXXX..)�� ��Ÿ�� �������� �κ� ���ڿ��� ���
                        //�� �ڵ� ���� printf("found match: %s\n\n", result);

                        //GNRMC ������ �Ľ��Ͽ� �� ������ ����ϴ� idParsing �Լ� ȣ��(idParsing.h)
                        idParsing(result);
                    }
                    //GNRMC ������ ã�� ����� ������
                    else {
                        printf("GNRMC match sentence is not found.\n\n");
                    }
                }
                //�� �ڵ� ���� printSubstringAfterMatch(buffer, "$GNRMC"); // "printsam.h���� ���ǵ� �Լ��� ���"
                else
                    continue;
            }
        }
        //�ø��� ����� ���� ���� �����Ͱ� ������
        else {
            fprintf(stderr, "Error reading from serial port\n");
            break;
        }
        //�����͸� �߰��� �б� ���� �� �� ���� �����ʹ� ����
        //�� �ڵ� ���� memset(buffer, 0, sizeof(buffer));
    }

    //COM ��Ʈ �ݱ�
    CloseHandle(hSerial);
  
    return 0;
}