#ifndef IDPARSING_H
#define IDPARSING_H
#pragma warning(disable:4996)

typedef struct gnrmcdata {
    char gnrmcdataName[20];            //NMEA0183 ���� ID
    float gnrmcdataUTCtime;            //UTC �ð�
    char gnrmcdataStatus[10];          //���� A = Active, V = Void. V�̸� �ŷ��� �� ���� ����
    int gnrmcdataLatitude;             //����
    char gnrmcdataNSindicator[10];     //���� �Ǵ� ���� N = North, S = South
    int gnrmcdataLongitude;            //�浵
    char gnrmcdataEWindicator[10];     //���� �Ǵ� ���� E = East, W = West
    unsigned short gnrmcdataSpeed;     //�ӵ�. units of 0.02m/s
    unsigned short gnrmcdataCourse;    //�������. ����(WGS-84 Ÿ��ü�� �߽�����)�� �߽����� �ð�������� 0���� ���ų� ũ��, 360���� ���� ������ ������
    int gnrmcdataDate;                 //UTC ��¥. DDMMYY����
    float gnrmcdataMVvalue;            //degree. '�ں�'�� '����'�� ������ ����. ������� GPS������ �밳 NULL��
    char gnrmcdataMVEWindicator[10];   //
    char gnrmcdataModeindicator[50];   //checksum
}data;

void idParsing(const char* input) {
    data inputdata;
    char inputInsertBlank[256] = "\0"; //'@'���� ������ ���� �迭
    int blankIndex = 0;

    //������ ��� �� ��ȯ�� ���� ����
    float temp1Lati = 0; //������ ���·� '��'�� ����
    float temp2Lati = 0; 
    float temp3Lati = 0; //������ ���·� '��'�� ����
    float calcLati = 0; //������(DMS)���� ��(degree)������ ����Ͽ� ����

    //�浵�� ��� �� ��ȯ�� ���� ����
    float temp1Long = 0; //������ ���·� '��'�� ����
    float temp2Long = 0;
    float temp3Long = 0; //������ ���·� '��'�� ����
    float calcLong = 0;

    //�ӵ��� ��� �� ��ȯ�� ���� ����
    float calcSpeedms = 0;
    float calcSpeedDegree = 0;
    float insertKnots = 0;

    //���Ⱚ ��� �� ��ȯ�� ���� ����
    float insertDegree = 0;

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
            //������ ��ȣ�� �ŷ��� �� ���� ��� �޽��� ���
            if (inputdata.gnrmcdataStatus == 'V') {
                printf("Status: %s(Unreliable signal)\n", inputdata.gnrmcdataStatus);
            }
            else if (inputdata.gnrmcdataStatus == 'A') {
                //printf("Status: %s(Reliable signal)\n", inputdata.gnrmcdataSatus);
            }
            break;

        //case 3. ����
        case 3:
            //�������� �����Ͱ� ������ 0, �����Ͱ� ������ �� �������� �Ҽ��� ���� ���ڰ� ������ ����ȭ(100,000�� ����)�� ��
            inputdata.gnrmcdataLatitude = (ptr[0] == '@') ? 0 : (atof(ptr) * 100000);
            
            //SAE J2735 ǥ�� -900,000,000 <= latitude < 900,000,001 �� ���ϴ��� Ȯ��
            if (-900000000 <= inputdata.gnrmcdataLatitude && inputdata.gnrmcdataLatitude < 900000001) {
                temp1Lati = inputdata.gnrmcdataLatitude / 10000000; //������ ���·� '��'�� ���� - �տ������� �� �ڸ� ��
                temp2Lati = inputdata.gnrmcdataLatitude % ((int)temp1Lati * 10000000);
                temp3Lati = temp2Lati / 100000; //������ ���·� '��'�� ����
                calcLati = temp1Lati + (temp3Lati / 60); //������(DMS)���� ��(degree)������ ����Ͽ� ����
                
                printf("Latitude: %d\n", inputdata.gnrmcdataLatitude);
            }
            //������ ������ �ʴ´ٸ�
            else if (-900000000 > inputdata.gnrmcdataLatitude || inputdata.gnrmcdataLatitude >= 900000001) {
                printf("Latitude is not standard data.\n");
            }
            break;

        //case 4. ���� �Ǵ� ���� N = North, S = South
        case 4:
            strncpy(inputdata.gnrmcdataNSindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataNSindicator));
            //printf("North/South Indicator: %s\n", ptr);
            break;

        //case 5. �浵
        case 5:
            //�浵���� �����Ͱ� ������ 0, �����Ͱ� ������ �� �������� �Ҽ��� ���� ���ڰ� ������ ����ȭ(100,000�� ����)�� ��
            inputdata.gnrmcdataLongitude = (ptr[0] == '@') ? 0 : (atof(ptr) * 100000);

            //SAE J2735 ǥ�� -1,799,999,999 <= longitude < 1,800,000,001 �� ���ϴ��� Ȯ��
            if (-1799999999 <= inputdata.gnrmcdataLongitude && inputdata.gnrmcdataLongitude < 1800000001) {
                temp1Long = inputdata.gnrmcdataLongitude / 10000000; //������ ���·� '��'�� ���� - �տ������� �� �ڸ� ��
                temp2Long = inputdata.gnrmcdataLongitude % ((int)temp1Long * 10000000);
                temp3Long = temp2Long / 100000; //������ ���·� '��'�� ����
                calcLong = temp1Long + (temp3Long / 60); //������(DMS)���� ��(degree)������ ����Ͽ� ����

                printf("Longitude: %d\n", inputdata.gnrmcdataLongitude);
            }
            //������ ������ �ʴ´ٸ�
            else if (-1799999999 > inputdata.gnrmcdataLongitude || inputdata.gnrmcdataLongitude >= 1800000001) {
                printf("Longitude is not standard data.\n");
            }
            break;

        //case 6. ���� �Ǵ� ���� E = East, W = West
        case 6:
            strncpy(inputdata.gnrmcdataEWindicator, (ptr[0] == '@') ? "none" : ptr, sizeof(inputdata.gnrmcdataEWindicator));
            //printf("East/West Indicator: %s\n", ptr);
            break;

        //case 7. �ӵ�. units of 0.02m/s
        case 7:
            //ublox�� GNRMC�� �ӵ� ������ ���� Knots������.
            //1Knots = 0.514m/s. �����Ϳ� 0.514�� ���Ͽ� m/s������ ���� ����
            //SAE J2735�� ���� Speed�� 1value = 0.02m/s. �ٲ� ���ϸ� 1m/s = 50(Value) �����Ϳ� 50�� ���Ͽ� value������ ���� ����
            inputdata.gnrmcdataSpeed = (ptr[0] == '@') ? 0 : (atof(ptr)) * 0.514 * 50;

            //1000value�� 72km/h�� �´��� Ȯ���ϱ� ���� �ڵ�
            //insertKnots = 38.92;
            //inputdata.gnrmcdataSpeed = insertKnots * 0.514 * 50;

            //SAE J2735 ǥ�� 0 <= speed < 8,191 �� ���ϴ��� Ȯ��
            if (0 <= inputdata.gnrmcdataSpeed && inputdata.gnrmcdataSpeed < 8191) {

                //1000value�� 72km/h�� �´��� Ȯ���ϱ� ���� �ڵ�
                printf("Speed: %u(value)\n", inputdata.gnrmcdataSpeed);
                //printf("Speed: %d(knots)\n", (int)((inputdata.gnrmcdataSpeed / 0.514) / 50));
                //printf("Speed: %d(m/s)\n", (int)((inputdata.gnrmcdataSpeed / 50)));
                printf("Speed: %d(km/h)\n", (int)((inputdata.gnrmcdataSpeed / 50) * 3.6));
            }
            //������ ������ �ʴ´ٸ�
            else if (0 > inputdata.gnrmcdataSpeed || inputdata.gnrmcdataSpeed >= 8191) {
                printf("Speed is not standard data.\n");
            }
            break;

        //case 8. �������. ����(WGS-84 Ÿ��ü�� �߽�����)�� �߽����� �ð�������� 0���� ���ų� ũ��, 360���� ���� ������ ������
        case 8:
            //���Ⱚ�� ������ 0 ���Ⱚ�� ������ SAE J2735�� ���� 80�� ���� ���� ����
            //GNRMC format���� course���� degree����
            //1value (SAE J2735) = 0.0125degree
            //1degree = 80value
            inputdata.gnrmcdataCourse = (ptr[0] == '@') ? 0 : ((unsigned short)atof(ptr) * 80);
            
            //������ ���¿��� �����Ͱ� ������ �ʴ� ����� ������ ���� �������־� Ȯ���ϱ� ���� �ڵ�
            insertDegree = 250;
            inputdata.gnrmcdataCourse = insertDegree * 80;

            //SAE J2735 ǥ�� 0 <= heading <= 28800 �� ���ϴ��� Ȯ��
            if (0 <= inputdata.gnrmcdataCourse && inputdata.gnrmcdataCourse < 28800) {

                //������ ���¿��� �����Ͱ� ������ �ʴ� ����� ������ ���� �������־� Ȯ���ϱ� ���� �ڵ�
                printf("Course: %u(value)\n", inputdata.gnrmcdataCourse);

                printf("Course: %d(degree)\n\n", inputdata.gnrmcdataCourse / 80);
            }
            //������ ������ �ʴ´ٸ�
            else if (0 > inputdata.gnrmcdataCourse || inputdata.gnrmcdataCourse >= 28800) {
                printf("Course is not standard data.\n");
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