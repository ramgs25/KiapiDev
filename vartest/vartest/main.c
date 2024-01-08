#include <stdio.h>
#include <stdint.h>

//NMEA0183 ���忡�� ���� ������(����, �浵, ����, �ӵ�)�� ��ȯ�Ͽ� ��Ÿ���� �ڵ�

int main() {
	//NMEA0183 Sentence ID: GNRMC �� ������ ��ȯ
	float testLati = 3538.843506; //GNRMC ������ ����

	//SAE J2735 ǥ�� -900,000,000 <= latitude < 900,000,001
	if (-900000000 <= testLati && testLati < 900000001) {
		int calcLatiPart1 = ((int)testLati / 100); 	//GNRMC ������ �� '��'
		int calcLatiPart2 = ((int)testLati % 100);
		float calcLatiPart3 = (testLati - (int)testLati);
		float calcLatiPart4 = ((float)calcLatiPart2 + calcLatiPart3); //GNRMC ������ �� '��'

		int calcLatiDMS_d = calcLatiPart1; //DMS ���� �� '��'
		float calcLatiDMS_m = calcLatiPart4; //DNS ���� �� '��'
		float calcLatiDMS_s = 0; //DMS ���� �� '��'
		float calcLatiDegree = calcLatiPart1 + (calcLatiPart4 / 60); //degree ����

		printf("---����---\n");
		printf("GNRMC ������ ����: %f\n", testLati);
		printf("GNRMC ������ �� '��': %d\n", calcLatiPart1);
		printf("GNRMC ������ �� '��': %f\n\n", calcLatiPart4);

		printf("Latitude(DMS): %d�� %f�� %f��\n", calcLatiDMS_d, calcLatiDMS_m, calcLatiDMS_s);
		printf("Latitude(degree): %f\n\n", calcLatiDegree);
	}
	else if (-900000000 > testLati || testLati >= 900000001) {
		printf("���������� �������Դϴ�.\n");
	}

	//NMEA0183 Sentence ID: GNRMC �� �浵�� ��ȯ
	float testLong = 12824.019531; //GNRMC �浵�� ����

	//SAE J2735 ǥ�� -1,799,999,999 <= longitude < 1,800,000,001
	if (-1799999999 <= testLong && testLong < 1800000001) {
		int calcLongPart1 = ((int)testLong / 100); //GNRMC �浵�� �� '��'
		int calcLongPart2 = ((int)testLong % 100);
		float calcLongPart3 = (testLong - (int)testLong);
		float calcLongPart4 = ((float)calcLongPart2 + calcLongPart3); //GNRMC �浵�� �� '��'

		int calcLongDMS_d = calcLongPart1; //DMS ���� �� '��'
		float calcLongDMS_m = calcLongPart4; //DMS ���� �� '��'
		float calcLongDMS_s = 0; //DMS ���� �� '��'
		float calcLongDegree = calcLongPart1 + (calcLongPart4 / 60); //degree ����

		printf("---�浵---\n");
		printf("GNRMC �浵�� ����: %f\n", testLong);
		printf("GNRMC �浵�� �� '��': %d\n", calcLongPart1);
		printf("GNRMC �浵�� �� '��': %f\n\n", calcLongPart4);

		printf("Longitude(DMS): %d�� %f�� %f��\n", calcLongDMS_d, calcLongDMS_m, calcLongDMS_s);
		printf("Longitude(degree): %f\n\n", calcLongDegree);
	}
	else if (-1799999999 > testLong || testLong >= 1800000001) {
		printf("���������� �������Դϴ�.\n");
	}

	//NMEA0183 Sentence ID: GNRMC �� ���Ⱚ ��ȯ
	float testCourse = 0;

	//SAE J2735 ǥ�� 0 <= heading < 28800
	if (0 <= testCourse && testCourse < 28800) {
		float calcCourse = testCourse * 0.0125;

		printf("---����---\n");
		printf("GNRMC ���Ⱚ ����: %f\n\n", testCourse);

		printf("heading: %f\n\n", calcCourse);
	}
	else if (0 > testCourse || testCourse >= 28800) {
		printf("���������� �������Դϴ�.\n");
	}

	//NMEA0183 Sentence ID: GNRMC �� �ӵ��� ��ȯ
	float testSpeed = 0.077000;

	//SAE J2735 ǥ�� 0 <= speed < 8,191
	if (0 <= testSpeed && testSpeed < 8191) {
		float calcSpeed = testSpeed * 0.02; //Knots ������ ���� km/h�� ��ȯ

		printf("---�ӵ�---\n");
		printf("GNRMC �ӵ��� ����: %fKnots\n\n", testSpeed);

		printf("Speed: %fkm/h", calcSpeed);
	}
	else if (0 > testSpeed || testSpeed >= 8191) {
		printf("���������� �������Դϴ�.\n");
	}



	return 0;
}