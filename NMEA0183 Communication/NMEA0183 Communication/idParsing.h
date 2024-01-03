#ifndef IDPARSING_H
#define IDPARSING_H
#pragma warning(disable:4996)

typedef struct gnrmcdata {
    char gnrmcdataName[20];            //NMEA0183 문장 ID
    float gnrmcdataUTCtime;            //UTC 시각
    char gnrmcdataStatus[10];          //상태 A = Active, V = Void. V이면 신뢰할 수 없는 상태
    float gnrmcdataLatitude;           //위도
    char gnrmcdataNSindicator[10];     //북위 또는 남위 N = North, S = South
    float gnrmcdataLongitude;          //경도
    char gnrmcdataEWindicator[10];     //동경 또는 서경 E = East, W = West
    float gnrmcdataSpeed;              //속도. Knots단위. km/h로 변환하려면 해당 값에서 1.852를 곱한다
    float gnrmcdataCourse;             //진행방향. 진북(WGS-84 타원체를 중심으로)을 중심으로 시계방향으로 0보다 같거나 크고, 360보다 작은 범위의 각도값
    int gnrmcdataDate;                 //UTC 날짜. DDMMYY형식
    float gnrmcdataMVvalue;            //degree. '자북'과 '진북'의 차이의 각도. 통상적인 GPS에서는 대개 NULL임
    char gnrmcdataMVEWindicator[10];   //
    char gnrmcdataModeindicator[50];   //checksum
}data;

void idParsing(const char* input) {
    data inputdata;
    //이 코드 제거 char buffer[256] = "$GNRMC,233843.698,A,4830.5778,N,04433.5867,E,0.972006,,270719,,,D*6F";
    char inputInsertBlank[256];
    int blankIndex = 0;

    for (int i = 0; i < strlen(input); i++) {
        inputInsertBlank[blankIndex++] = input[i];

        if (input[i] == ',' && input[i + 1] == ',') {
            inputInsertBlank[blankIndex++] = '@';
        }
    }
    //이 코드 제거 strcpy(bufferCopy, buffer);

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
            //이 코드 제거 inputdata.gnrmcdataStatus = (ptr[0] == '@') ? 'none' : *ptr;
            //이 코드 제거 printf("Status: %c\n", inputdata.gnrmcdataStatus); break;
            strncpy(inputdata.gnrmcdataStatus, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataStatus));
            printf("Status: %s\n", ptr); break;
        case 3:
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Latitude: %f\n", inputdata.gnrmcdataLatitude); break;
        case 4:
            //이 코드 제거 inputdata.gnrmcdataNSindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //이 코드 제거 printf("North/South Indicator: %s\n", inputdata.gnrmcdataNSindicator); break;
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            printf("North/South Indicator: %s\n", ptr); break;
        case 5:
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : atof(ptr);
            printf("Longitude: %f\n", inputdata.gnrmcdataLongitude); break;
        case 6:
            //이 코드 제거 inputdata.gnrmcdataEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //이 코드 제거 printf("East/West Indicator: %s\n", inputdata.gnrmcdataEWindicator); break;
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
            //이 코드 제거 inputdata.gnrmcdataMVEWindicator = (ptr[0] == '@') ? 'none' : *ptr;
            //이 코드 제거 printf("Magnetic Variation East/West Indicator: %s\n", inputdata.gnrmcdataMVEWindicator); break;
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