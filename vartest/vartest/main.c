#include <stdio.h>
#include <stdint.h>

//NMEA0183 문장에서 읽은 데이터(위도, 경도, 방향, 속도)를 변환하여 나타내는 코드

int main() {
	//NMEA0183 Sentence ID: GNRMC 중 위도값 변환
	float testLati = 3538.843506; //GNRMC 위도값 예시

	//SAE J2735 표준 -900,000,000 <= latitude < 900,000,001
	if (-900000000 <= testLati && testLati < 900000001) {
		int calcLatiPart1 = ((int)testLati / 100); 	//GNRMC 위도값 중 '도'
		int calcLatiPart2 = ((int)testLati % 100);
		float calcLatiPart3 = (testLati - (int)testLati);
		float calcLatiPart4 = ((float)calcLatiPart2 + calcLatiPart3); //GNRMC 위도값 중 '분'

		int calcLatiDMS_d = calcLatiPart1; //DMS 형태 중 '도'
		float calcLatiDMS_m = calcLatiPart4; //DNS 형태 중 '분'
		float calcLatiDMS_s = 0; //DMS 형태 중 '초'
		float calcLatiDegree = calcLatiPart1 + (calcLatiPart4 / 60); //degree 형태

		printf("---위도---\n");
		printf("GNRMC 위도값 예시: %f\n", testLati);
		printf("GNRMC 위도값 중 '도': %d\n", calcLatiPart1);
		printf("GNRMC 위도값 중 '분': %f\n\n", calcLatiPart4);

		printf("Latitude(DMS): %d도 %f분 %f초\n", calcLatiDMS_d, calcLatiDMS_m, calcLatiDMS_s);
		printf("Latitude(degree): %f\n\n", calcLatiDegree);
	}
	else if (-900000000 > testLati || testLati >= 900000001) {
		printf("비정상적인 데이터입니다.\n");
	}

	//NMEA0183 Sentence ID: GNRMC 중 경도값 변환
	float testLong = 12824.019531; //GNRMC 경도값 예시

	//SAE J2735 표준 -1,799,999,999 <= longitude < 1,800,000,001
	if (-1799999999 <= testLong && testLong < 1800000001) {
		int calcLongPart1 = ((int)testLong / 100); //GNRMC 경도값 중 '도'
		int calcLongPart2 = ((int)testLong % 100);
		float calcLongPart3 = (testLong - (int)testLong);
		float calcLongPart4 = ((float)calcLongPart2 + calcLongPart3); //GNRMC 경도값 중 '분'

		int calcLongDMS_d = calcLongPart1; //DMS 형태 중 '도'
		float calcLongDMS_m = calcLongPart4; //DMS 형태 중 '분'
		float calcLongDMS_s = 0; //DMS 형태 중 '초'
		float calcLongDegree = calcLongPart1 + (calcLongPart4 / 60); //degree 형태

		printf("---경도---\n");
		printf("GNRMC 경도값 예시: %f\n", testLong);
		printf("GNRMC 경도값 중 '도': %d\n", calcLongPart1);
		printf("GNRMC 경도값 중 '분': %f\n\n", calcLongPart4);

		printf("Longitude(DMS): %d도 %f분 %f초\n", calcLongDMS_d, calcLongDMS_m, calcLongDMS_s);
		printf("Longitude(degree): %f\n\n", calcLongDegree);
	}
	else if (-1799999999 > testLong || testLong >= 1800000001) {
		printf("비정상적인 데이터입니다.\n");
	}

	//NMEA0183 Sentence ID: GNRMC 중 방향값 변환
	float testCourse = 0;

	//SAE J2735 표준 0 <= heading < 28800
	if (0 <= testCourse && testCourse < 28800) {
		float calcCourse = testCourse * 0.0125;

		printf("---방향---\n");
		printf("GNRMC 방향값 예시: %f\n\n", testCourse);

		printf("heading: %f\n\n", calcCourse);
	}
	else if (0 > testCourse || testCourse >= 28800) {
		printf("비정상적인 데이터입니다.\n");
	}

	//NMEA0183 Sentence ID: GNRMC 중 속도값 변환
	float testSpeed = 0.077000;

	//SAE J2735 표준 0 <= speed < 8,191
	if (0 <= testSpeed && testSpeed < 8191) {
		float calcSpeed = testSpeed * 0.02; //Knots 단위의 값을 km/h로 변환

		printf("---속도---\n");
		printf("GNRMC 속도값 예시: %fKnots\n\n", testSpeed);

		printf("Speed: %fkm/h", calcSpeed);
	}
	else if (0 > testSpeed || testSpeed >= 8191) {
		printf("비정상적인 데이터입니다.\n");
	}



	return 0;
}