#pragma once

//main에서 findGNRMC() 함수를 다음과 같은 인자와 함께 호출할 것임
//findGNRMC(buffer, "$GNRMC", '$');

void findGNRMC(const char* input, const char* match, char delimiter) {
	// 문자열input에서 match문자열을 찾음
	char* result = strstr((char*)input, match);

	if (result != NULL) {
		//찾은 위치부터 다음 NMEA0183 ID($XXXXX..)가 나타날 때까지의 부분 문자열을 출력
		printf("found match: ");
		while (*result != delimiter) {
			printf("%s", result);
		}
		printf("\n");
	}
	else {
		printf("match not found.\n");
	}
}