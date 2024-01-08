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
    char inputInsertBlank[256] = "\0"; //'@'���� ������ ���� �迭
    int blankIndex = 0;

    //���ڿ����� ������ ��� �� ��ȯ�� ���� ����
    int temp1Lati = 0, temp2Lati = 0;
    float temp3Lati = 0, temp4Lati = 0;

    //���ڿ����� �浵�� ��� �� ��ȯ�� ���� ����
    int temp1Long = 0, temp2Long = 0;
    float temp3Long = 0, temp4Long = 0;

    //�ӵ��� ��� �� ��ȯ�� ���� ����
    float kmhSpeed = 0;

    //���Ⱚ ��� �� ��ȯ�� ���� ����
    float calcHeading = 0;

    //�����Ͱ� �� ��� ������ ���� '@'���� ����
    for (int i = 0; i < strlen(input); i++) {
        inputInsertBlank[blankIndex++] = input[i];

        //���ŵ� GPS ��ȣ���� ������ �ڸ��� �����Ͱ� ������ �ʾ��� ��� ó���� ���� '@'���� ����
        if (input[i] == ',' && input[i + 1] == ',') {
            inputInsertBlank[blankIndex++] = '@';
        }
    }

    //','���ڸ� �������� �Ľ�
    char* ptr = strtok(inputInsertBlank, ",");
    int k = 0;

    char cpyptrforLati[256] = "\0", cpyptrforLong[256] = "\0";
    char* PcpyptrforLati = &cpyptrforLati[0], PcpyptrforLong = &cpyptrforLong[0];

    //���ڿ� ��ҵ��� �ϳ��� �и��Ͽ� ���
    while (ptr != NULL) {
        switch (k) {

        //case 0. NMEA0183 ���� ID
        case 0:
            strcpy(inputdata.gnrmcdataName, ptr);
            break;

        //case 1. UTC �ð�
        case 1:
            inputdata.gnrmcdataUTCtime = (ptr[0] == '@') ? 0 : atof(ptr);
            //printf("UTC time: %f\n", inputdata.gnrmcdataUTCtime);
            break;

        //case 2. ���� A = Active, V = Void. V�̸� �ŷ��� �� ���� ����
        case 2:
            strncpy(inputdata.gnrmcdataStatus, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataStatus));
            //printf("Status: %s\n", ptr);
            break;

        //case 3. ����
        case 3:
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 ǥ�� -900,000,000 <= latitude < 900,000,001
            if (-900000000 <= inputdata.gnrmcdataLatitude && inputdata.gnrmcdataLatitude < 900000001) {
                temp3Lati = inputdata.gnrmcdataLatitude - (int)inputdata.gnrmcdataLatitude;
                temp1Lati = (int)(inputdata.gnrmcdataLatitude / 100);
                temp2Lati = ((int)inputdata.gnrmcdataLatitude % 100);
                temp4Lati = (float)temp2Lati + temp3Lati;
                printf("Latitude: %f\n", temp1Lati + ((temp4Lati) / 60));
            }
            else if (-900000000 > inputdata.gnrmcdataLatitude || inputdata.gnrmcdataLatitude >= 900000001) {
                printf("���������� �������Դϴ�.\n");
            }
            break;

        //case 4. ���� �Ǵ� ���� N = North, S = South
        case 4:
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            //printf("North/South Indicator: %s\n", ptr);
            break;

        //case 5. �浵
        case 5:
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 ǥ�� -1,799,999,999 <= longitude < 1,800,000,001
            if (-1799999999 <= inputdata.gnrmcdataLongitude && inputdata.gnrmcdataLongitude < 1800000001) {
                temp3Long = inputdata.gnrmcdataLongitude - (int)inputdata.gnrmcdataLongitude;
                temp1Long = (int)(inputdata.gnrmcdataLongitude / 100);
                temp2Long = ((int)inputdata.gnrmcdataLongitude % 100);
                temp4Long = (float)temp2Long + temp3Long;
                printf("Longitude: %f\n", temp1Long + ((temp4Long) / 60));
            }
            else if (-1799999999 > inputdata.gnrmcdataLongitude || inputdata.gnrmcdataLongitude >= 1800000001) {
                printf("���������� �������Դϴ�.\n");
            }
            break;

        //case 6. ���� �Ǵ� ���� E = East, W = West
        case 6:
            strncpy(inputdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataEWindicator));
            //printf("East/West Indicator: %s\n", ptr);
            break;

        //case 7. �ӵ�. Knots����. km/h�� ��ȯ�Ϸ��� �ش� ������ 1.852�� ���Ѵ�
        case 7:
            inputdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 ǥ�� 0 <= speed < 8,191
            if (0 <= inputdata.gnrmcdataSpeed && inputdata.gnrmcdataSpeed < 8191) {
                kmhSpeed = inputdata.gnrmcdataSpeed * 0.1852; //Knots ������ ���� km/h ������ ��ȯ
                printf("Speed: %f km/h\n", kmhSpeed);
            }
            else if (0 > inputdata.gnrmcdataSpeed || inputdata.gnrmcdataSpeed >= 8191) {
                printf("���������� �������Դϴ�.\n");
            }
            break;

        //case 8. �������. ����(WGS-84 Ÿ��ü�� �߽�����)�� �߽����� �ð�������� 0���� ���ų� ũ��, 360���� ���� ������ ������
        case 8:
            inputdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : atof(ptr);

            //SAE J2735 ǥ�� 0 <= heading < 28800
            if (0 <= inputdata.gnrmcdataCourse && inputdata.gnrmcdataCourse < 28800) {
                calcHeading = inputdata.gnrmcdataCourse * 1.8;
                printf("heading: %f\n\n", calcHeading);
            }
            else if (0 > inputdata.gnrmcdataCourse || inputdata.gnrmcdataCourse >= 28800) {
                printf("���������� �������Դϴ�.\n");
            }
            break;

        //case 9. UTC ��¥. DDMMYY����
        case 9:
            inputdata.gnrmcdataDate = (ptr[0] == '@') ? 0 : atoi(ptr);
            //printf("Date: %d\n", inputdata.gnrmcdataDate);
            break;

        //case 10. degree. '�ں�'�� '����'�� ������ ����. ������� GPS������ �밳 NULL��
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