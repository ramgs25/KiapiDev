#ifndef IDPARSING_H
#define IDPARSING_H
#pragma warning(disable:4996)

typedef struct gnrmcdata {
    char gnrmcdataName[20];            //NMEA0183 문장 ID
    float gnrmcdataUTCtime;            //UTC 시각
    char gnrmcdataStatus[10];          //상태 A = Active, V = Void. V이면 신뢰할 수 없는 상태
    int gnrmcdataLatitude;             //위도
    char gnrmcdataNSindicator[10];     //북위 또는 남위 N = North, S = South
    int gnrmcdataLongitude;            //경도
    char gnrmcdataEWindicator[10];     //동경 또는 서경 E = East, W = West
    unsigned short gnrmcdataSpeed;     //속도. units of 0.02m/s
    unsigned short gnrmcdataCourse;    //진행방향. 진북(WGS-84 타원체를 중심으로)을 중심으로 시계방향으로 0보다 같거나 크고, 360보다 작은 범위의 각도값
    int gnrmcdataDate;                 //UTC 날짜. DDMMYY형식
    float gnrmcdataMVvalue;            //degree. '자북'과 '진북'의 차이의 각도. 통상적인 GPS에서는 대개 NULL임
    char gnrmcdataMVEWindicator[10];   //
    char gnrmcdataModeindicator[50];   //checksum
}data;

void idParsing(const char* input) {
    data inputdata;
    char inputInsertBlank[256] = "\0"; //'@'문자 삽입을 위한 배열
    int blankIndex = 0;

    //위도값 계산 및 변환을 위한 변수
    float temp1Lati = 0; //도분초 형태로 '도'값 저장
    float temp2Lati = 0; 
    float temp3Lati = 0; //도분초 형태로 '분'값 저장
    float calcLati = 0; //도분초(DMS)에서 도(degree)값으로 계산하여 저장

    //경도값 계산 및 변환을 위한 변수
    float temp1Long = 0; //도분초 형태로 '도'값 저장
    float temp2Long = 0;
    float temp3Long = 0; //도분초 형태로 '분'값 저장
    float calcLong = 0;

    //속도값 계산 및 변환을 위한 변수
    float calcSpeedms = 0;
    float calcSpeedDegree = 0;
    float insertKnots = 0;

    //방향값 계산 및 변환을 위한 변수
    float insertDegree = 0;

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
            //수신한 신호가 신뢰할 수 없는 경우 메시지 출력
            if (inputdata.gnrmcdataStatus == 'V') {
                printf("Status: %s(Unreliable signal)\n", inputdata.gnrmcdataStatus);
            }
            else if (inputdata.gnrmcdataStatus == 'A') {
                //printf("Status: %s(Reliable signal)\n", inputdata.gnrmcdataSatus);
            }
            break;

        //case 3. 위도
        case 3:
            //위도값은 데이터가 없으면 0, 데이터가 있으면 그 데이터의 소수점 이하 숫자가 없도록 정수화(100,000을 곱셈)한 수
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : (atof(ptr) * 100000);
            
            //SAE J2735 표준 -900,000,000 <= latitude < 900,000,001 에 속하는지 확인
            if (-900000000 <= inputdata.gnrmcdataLatitude && inputdata.gnrmcdataLatitude < 900000001) {
                temp1Lati = inputdata.gnrmcdataLatitude / 10000000; //도분초 형태로 '도'값 저장 - 앞에서부터 두 자리 수
                temp2Lati = inputdata.gnrmcdataLatitude % ((int)temp1Lati * 10000000);
                temp3Lati = temp2Lati / 100000; //도분초 형태로 '분'값 저장
                calcLati = temp1Lati + (temp3Lati / 60); //도분초(DMS)에서 도(degree)값으로 계산하여 저장
                
                printf("Latitude: %d\n", inputdata.gnrmcdataLatitude);
            }
            //범위에 속하지 않는다면
            else if (-900000000 > inputdata.gnrmcdataLatitude || inputdata.gnrmcdataLatitude >= 900000001) {
                printf("Latitude is not standard data.\n");
            }
            break;

        //case 4. 북위 또는 남위 N = North, S = South
        case 4:
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            //printf("North/South Indicator: %s\n", ptr);
            break;

        //case 5. 경도
        case 5:
            //경도값은 데이터가 없으면 0, 데이터가 있으면 그 데이터의 소수점 이하 숫자가 없도록 정수화(100,000을 곱셈)한 수
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : (atof(ptr) * 100000);

            //SAE J2735 표준 -1,799,999,999 <= longitude < 1,800,000,001 에 속하는지 확인
            if (-1799999999 <= inputdata.gnrmcdataLongitude && inputdata.gnrmcdataLongitude < 1800000001) {
                temp1Long = inputdata.gnrmcdataLongitude / 10000000; //도분초 형태로 '도'값 저장 - 앞에서부터 세 자리 수
                temp2Long = inputdata.gnrmcdataLongitude % ((int)temp1Long * 10000000);
                temp3Long = temp2Long / 100000; //도분초 형태로 '분'값 저장
                calcLong = temp1Long + (temp3Long / 60); //도분초(DMS)에서 도(degree)값으로 계산하여 저장

                printf("Longitude: %d\n", inputdata.gnrmcdataLongitude);
            }
            //범위에 속하지 않는다면
            else if (-1799999999 > inputdata.gnrmcdataLongitude || inputdata.gnrmcdataLongitude >= 1800000001) {
                printf("Longitude is not standard data.\n");
            }
            break;

        //case 6. 동경 또는 서경 E = East, W = West
        case 6:
            strncpy(inputdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataEWindicator));
            //printf("East/West Indicator: %s\n", ptr);
            break;

        //case 7. 속도. units of 0.02m/s
        case 7:
            //ublox의 GNRMC의 속도 데이터 값은 Knots단위임.
            //1Knots = 0.514m/s. 데이터에 0.514를 곱하여 m/s단위의 값을 만듦
            //SAE J2735에 따라 Speed의 1value = 0.02m/s. 바꿔 말하면 1m/s = 50(Value) 데이터에 50을 곱하여 value단위의 값을 만듦
            inputdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : (atof(ptr)) * 0.514 * 50;

            //1000value는 72km/h가 맞는지 확인하기 위한 코드
            //insertKnots = 38.92;
            //inputdata.gnrmcdataSpeed = insertKnots * 0.514 * 50;

            //SAE J2735 표준 0 <= speed < 8,191 에 속하는지 확인
            if (0 <= inputdata.gnrmcdataSpeed && inputdata.gnrmcdataSpeed < 8191) {

                //1000value는 72km/h가 맞는지 확인하기 위한 코드
                printf("Speed: %u(value)\n", inputdata.gnrmcdataSpeed);
                //printf("Speed: %d(knots)\n", (int)((inputdata.gnrmcdataSpeed / 0.514) / 50));
                //printf("Speed: %d(m/s)\n", (int)((inputdata.gnrmcdataSpeed / 50)));
                printf("Speed: %d(km/h)\n", (int)((inputdata.gnrmcdataSpeed / 50) * 3.6));
            }
            //범위에 속하지 않는다면
            else if (0 > inputdata.gnrmcdataSpeed || inputdata.gnrmcdataSpeed >= 8191) {
                printf("Speed is not standard data.\n");
            }
            break;

        //case 8. 진행방향. 진북(WGS-84 타원체를 중심으로)을 중심으로 시계방향으로 0보다 같거나 크고, 360보다 작은 범위의 각도값
        case 8:
            //방향값이 없으면 0 방향값이 있으면 SAE J2735에 따라 80을 곱한 값을 대입
            //GNRMC format에서 course값은 degree단위
            //1value (SAE J2735) = 0.0125degree
            //1degree = 80value
            inputdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : ((unsigned short)atof(ptr) * 80);
            
            //정지한 상태에서 데이터가 변하지 않는 관계로 임의의 값을 지정해주어 확인하기 위한 코드
            insertDegree = 250;
            inputdata.gnrmcdataCourse = insertDegree * 80;

            //SAE J2735 표준 0 <= heading <= 28800 에 속하는지 확인
            if (0 <= inputdata.gnrmcdataCourse && inputdata.gnrmcdataCourse < 28800) {

                //정지한 상태에서 데이터가 변하지 않는 관계로 임의의 값을 지정해주어 확인하기 위한 코드
                printf("Course: %u(value)\n", inputdata.gnrmcdataCourse);

                printf("Course: %d(degree)\n\n", inputdata.gnrmcdataCourse / 80);
            }
            //범위에 속하지 않는다면
            else if (0 > inputdata.gnrmcdataCourse || inputdata.gnrmcdataCourse >= 28800) {
                printf("Course is not standard data.\n");
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