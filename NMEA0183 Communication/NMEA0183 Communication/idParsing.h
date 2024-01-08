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
    char inputInsertBlank[256] = "\0"; //'@'문자 삽입을 위한 배열
    int blankIndex = 0;

    //문자열에서 위도값 계산 및 변환을 위한 변수
    int temp1Lati = 0, temp2Lati = 0;
    float temp3Lati = 0, temp4Lati = 0;

    //문자열에서 경도값 계산 및 변환을 위한 변수
    int temp1Long = 0, temp2Long = 0;
    float temp3Long = 0, temp4Long = 0;

    //속도값 계산 및 변환을 위한 변수
    float kmhSpeed = 0;

    //방향값 계산 및 변환을 위한 변수
    float calcHeading = 0;

    //데이터가 빈 경우 구분을 위해 '@'문자 삽입
    for (int i = 0; i < strlen(input); i++) {
        inputInsertBlank[blankIndex++] = input[i];

        //수신된 GPS 신호에서 지정된 자리의 데이터가 들어오지 않았을 경우 처리를 위해 '@'문자 삽입
        if (input[i] == ',' && input[i + 1] == ',') {
            inputInsertBlank[blankIndex++] = '@';
        }
    }

    //','문자를 기준으로 파싱
    char* ptr = strtok(inputInsertBlank, ",");
    int k = 0;

    char cpyptrforLati[256] = "\0", cpyptrforLong[256] = "\0";
    char* PcpyptrforLati = &cpyptrforLati[0], PcpyptrforLong = &cpyptrforLong[0];

    //문자열 요소들을 하나씩 분리하여 출력
    while (ptr != NULL) {
        switch (k) {

        //case 0. NMEA0183 문장 ID
        case 0:
            strcpy(inputdata.gnrmcdataName, ptr);
            break;

        //case 1. UTC 시각
        case 1:
            inputdata.gnrmcdataUTCtime = (ptr[0] == '@') ? 0 : atof(ptr);
            //printf("UTC time: %f\n", inputdata.gnrmcdataUTCtime);
            break;

        //case 2. 상태 A = Active, V = Void. V이면 신뢰할 수 없는 상태
        case 2:
            strncpy(inputdata.gnrmcdataStatus, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataStatus));
            //printf("Status: %s\n", ptr);
            break;

        //case 3. 위도
        case 3:
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 표준 -900,000,000 <= latitude < 900,000,001
            if (-900000000 <= inputdata.gnrmcdataLatitude && inputdata.gnrmcdataLatitude < 900000001) {
                temp3Lati = inputdata.gnrmcdataLatitude - (int)inputdata.gnrmcdataLatitude;
                temp1Lati = (int)(inputdata.gnrmcdataLatitude / 100);
                temp2Lati = ((int)inputdata.gnrmcdataLatitude % 100);
                temp4Lati = (float)temp2Lati + temp3Lati;
                printf("Latitude: %f\n", temp1Lati + ((temp4Lati) / 60));
            }
            else if (-900000000 > inputdata.gnrmcdataLatitude || inputdata.gnrmcdataLatitude >= 900000001) {
                printf("비정상적인 데이터입니다.\n");
            }
            break;

        //case 4. 북위 또는 남위 N = North, S = South
        case 4:
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            //printf("North/South Indicator: %s\n", ptr);
            break;

        //case 5. 경도
        case 5:
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 표준 -1,799,999,999 <= longitude < 1,800,000,001
            if (-1799999999 <= inputdata.gnrmcdataLongitude && inputdata.gnrmcdataLongitude < 1800000001) {
                temp3Long = inputdata.gnrmcdataLongitude - (int)inputdata.gnrmcdataLongitude;
                temp1Long = (int)(inputdata.gnrmcdataLongitude / 100);
                temp2Long = ((int)inputdata.gnrmcdataLongitude % 100);
                temp4Long = (float)temp2Long + temp3Long;
                printf("Longitude: %f\n", temp1Long + ((temp4Long) / 60));
            }
            else if (-1799999999 > inputdata.gnrmcdataLongitude || inputdata.gnrmcdataLongitude >= 1800000001) {
                printf("비정상적인 데이터입니다.\n");
            }
            break;

        //case 6. 동경 또는 서경 E = East, W = West
        case 6:
            strncpy(inputdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataEWindicator));
            //printf("East/West Indicator: %s\n", ptr);
            break;

        //case 7. 속도. Knots단위. km/h로 변환하려면 해당 값에서 1.852를 곱한다
        case 7:
            inputdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 표준 0 <= speed < 8,191
            if (0 <= inputdata.gnrmcdataSpeed && inputdata.gnrmcdataSpeed < 8191) {
                kmhSpeed = inputdata.gnrmcdataSpeed * 0.1852; //Knots 단위의 값을 km/h 단위로 변환
                printf("Speed: %f km/h\n", kmhSpeed);
            }
            else if (0 > inputdata.gnrmcdataSpeed || inputdata.gnrmcdataSpeed >= 8191) {
                printf("비정상적인 데이터입니다.\n");
            }
            break;

        //case 8. 진행방향. 진북(WGS-84 타원체를 중심으로)을 중심으로 시계방향으로 0보다 같거나 크고, 360보다 작은 범위의 각도값
        case 8:
            inputdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 표준 0 <= heading < 28800
            if (0 <= inputdata.gnrmcdataCourse && inputdata.gnrmcdataCourse < 28800) {
                calcHeading = inputdata.gnrmcdataCourse * 1.8;
                printf("heading: %f\n\n", calcHeading);
            }
            else if (0 > inputdata.gnrmcdataCourse || inputdata.gnrmcdataCourse >= 28800) {
                printf("비정상적인 데이터입니다.\n");
            }
            break;

        //case 9. UTC 날짜. DDMMYY형식
        case 9:
            inputdata.gnrmcdataDate = (ptr[0] == '@') ? 0 : atoi(ptr);
            //printf("Date: %d\n", inputdata.gnrmcdataDate);
            break;

        //case 10. degree. '자북'과 '진북'의 차이의 각도. 통상적인 GPS에서는 대개 NULL임
        case 10:
            inputdata.gnrmcdataMVvalue = (ptr[0] == '@') ? 0 : atof(ptr);
            //printf("Magnetic Variation value: %f\n", inputdata.gnrmcdataMVvalue);
            break;

        //case 11.
        case 11:
            strncpy(inputdata.gnrmcdataMVEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataMVEWindicator));
            //printf("Magnetic Variation East/West Indicator: %s\n\n", ptr);
            break;

        //case 12. checksum
        case 12:
            strncpy(inputdata.gnrmcdataModeindicator, (ptr[0] == '@') ? "NULL" : ptr, sizeof(inputdata.gnrmcdataModeindicator));
            break;
        }

        ptr = strtok(NULL, ",");
        k++;
    }
}

#endif