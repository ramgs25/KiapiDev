#pragma once

//main에서 findGNRMC() 함수를 다음과 인자값 findGNRMC(buffer, "$GNRMC", '$')로 호출할 것임

void findGNRMC(const char* input, const char* match, char delimiter) {
	//문자열input에서 match문자열을 찾음
	char* result = strstr((char*)input, match);

	if (*result != NULL) {
		//문자열 끝에 널 문자 추가
		result[strcspn(result, "\n")] = '\0';
		//이 주석 제거 찾은 위치부터 다음 NMEA0183 ID($XXXXX..)가 나타날 때까지의 부분 문자열을 출력
		//이 코드 제거 printf("found match: %s\n\n", result);
		return result;
	}
	else {
		printf("match not found.\n");
	}
}