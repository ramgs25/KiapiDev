#pragma once

void findGNRMC(const char* input, const char* match, char delimiter) {
	// 문자열input에서 match문자열을 찾음
	char* result = strstr(input, match);

	if (result != NULL) {
		//찾은 위치부터 다음 NMEA0183 ID($XXXXX..)가 나타날 때까지의 부분 문자열을 출력
		printf("found match: ");
		while (*result != delimiter) {
			printf("%s", result);
			result++;
		}
		printf("\n");
	}
	else {
		printf("match not found.\n");
	}
}