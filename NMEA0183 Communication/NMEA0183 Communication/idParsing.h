#ifndef IDPARSING_H
#define IDPARSING_H
#pragma warning(disable:4996)

typedef struct gnrmcdata {
    char gnrmcdataName[20];            //NMEA0183 ���� ID
    float gnrmcdataUTCtime;            //UTC �ð�
    char gnrmcdataStatus[10];          //���� A = Active, V = Void. V�̸� �ŷ��� �� ���� ����
    float gnrmcdataLatitude;           //����
    char gnrmcdataNSindicator[10];     //���� �Ǵ� ���� N = North, S = South
    float gnrmcdataLongitude;          //�浵
    char gnrmcdataEWindicator[10];     //���� �Ǵ� ���� E = East, W = West
    float gnrmcdataSpeed;              //�ӵ�. Knots����. km/h�� ��ȯ�Ϸ��� �ش� ������ 1.852�� ���Ѵ�
    float gnrmcdataCourse;             //�������. ����(WGS-84 Ÿ��ü�� �߽�����)�� �߽����� �ð�������� 0���� ���ų� ũ��, 360���� ���� ������ ������
    int gnrmcdataDate;                 //UTC ��¥. DDMMYY����
    float gnrmcdataMVvalue;            //degree. '�ں�'�� '����'�� ������ ����. ������� GPS������ �밳 NULL��
    char gnrmcdataMVEWindicator[10];   //
    char gnrmcdataModeindicator[50];   //checksum
}data;

void idParsing(const char* input) {
    data inputdata;
    //�� �ڵ� ���� char buffer[256] = "$GNRMC,233843.698,A,4830.5778,N,04433.5867,E,0.972006,,270719,,,D*6F";
    char inputInsertBlank[256];
    int blankIndex = 0;

    for (int i = 0; i < strlen(input); i++) {
        inputInsertBlank[blankIndex++] = input[i];

        if (input[i] == ',' && input[i + 1] == ',') {
            inputInsertBlank[blankIndex++] = '@';
        }
    }
    //�� �ڵ� ���� strcpy(bufferCopy, buffer);

    char* ptr = strtok(inputInsertBlank, ",");
    int k = 0;

    while (ptr != NULL) {
        switch (k) {
        case 0:
            strcpy(inputdata.gnrmcdataName, ptr); break;
        case 1:
            inputdata.gnrmcdataUTCtime = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("UTC time: %f\n", inputdata.gnrmcdataUTCtime); break;
        case 2:
            //�� �ڵ� ���� inputdata.gnrmcdataStatus = (ptr[0] == '@') ? 'none' : *ptr;
            //�� �ڵ� ���� printf("Status: %c\n", inputdata.gnrmcdataStatus); break;
            strncpy(inputdata.gnrmcdataStatus, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataStatus));
            printf("Status: %s\n", ptr); break;
        case 3:
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Latitude: %f\n", inputdata.gnrmcdataLatitude); break;
        case 4:
            //�� �ڵ� ���� inputdata.gnrmcdataNSindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //�� �ڵ� ���� printf("North/South Indicator: %s\n", inputdata.gnrmcdataNSindicator); break;
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            printf("North/South Indicator: %s\n", ptr); break;
        case 5:
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Longitude: %f\n", inputdata.gnrmcdataLongitude); break;
        case 6:
            //�� �ڵ� ���� inputdata.gnrmcdataEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //�� �ڵ� ���� printf("East/West Indicator: %s\n", inputdata.gnrmcdataEWindicator); break;
            strncpy(inputdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataEWindicator));
            printf("East/West Indicator: %s\n", ptr);  break;
        case 7:
            inputdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Speed: %f\n", inputdata.gnrmcdataSpeed); break;
        case 8:
            inputdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Course: %f\n", inputdata.gnrmcdataCourse); break;
        case 9:
            inputdata.gnrmcdataDate = (ptr[0] == '@') ? 0 : atoi(ptr);
            printf("Date: %d\n", inputdata.gnrmcdataDate); break;
        case 10:
            inputdata.gnrmcdataMVvalue = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Magnetic Variation value: %f\n", inputdata.gnrmcdataMVvalue); break;
        case 11:
            //�� �ڵ� ���� inputdata.gnrmcdataMVEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //�� �ڵ� ���� printf("Magnetic Variation East/West Indicator: %s\n", inputdata.gnrmcdataMVEWindicator); break;
            strncpy(inputdata.gnrmcdataMVEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataMVEWindicator));
            printf("Magnetic Variation East/West Indicator: %s\n\n", ptr); break;
        case 12:
            strncpy(inputdata.gnrmcdataModeindicator, (ptr[0] == '@') ? "NULL" : ptr, sizeof(inputdata.gnrmcdataModeindicator)); break;
        }

        ptr = strtok(NULL, ",");
        k++;
    }
}

#endif